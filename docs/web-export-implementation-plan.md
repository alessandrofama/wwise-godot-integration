# Web (Emscripten) エクスポート対応 実装プラン

## 概要

Wwise 2025.1.3 SDK の Emscripten プラットフォームライブラリを使用して、Godot の Web エクスポートに対応する。

### 動作環境前提

| 要素 | バージョン / パス |
|---|---|
| Wwise SDK | `/Applications/AudioKinetic/Wwise2025.1.3.9039/SDK` |
| Emscripten | `emsdk/` (プロジェクトルート、emcc 4.0.23) |
| Godot | 4.3 〜 4.5 |
| スレッドモード | `threads=yes`（Emscripten_mt）のみ初期対応 |

### Emscripten 環境の有効化

ビルド前に毎回実行が必要:

```bash
source emsdk/emsdk_env.sh
```

---

## Wwise SDK Web ライブラリ構成

```
SDK/
  Emscripten_mt/          # マルチスレッド (threads=yes, SharedArrayBuffer)
    Debug/lib/*.a
    Profile/lib/*.a
    Release/lib/*.a
  Emscripten_st/          # シングルスレッド (初期実装スコープ外)
    Debug/lib/*.a
    Profile/lib/*.a
    Release/lib/*.a
  samples/SoundEngine/Emscripten/
    WwiseAudioWorklet.processor.js   # 実行時に必要な JS ファイル
    WwiseProfilerServer/             # プロファイラー用 Node.js WebSocket プロキシ
  include/AK/SoundEngine/Platforms/Emscripten/
    AkEmscriptenSoundEngine.h        # AkPlatformInitSettings 定義
```

### `AkPlatformInitSettings` (Emscripten 固有フィールド)

```cpp
struct AkPlatformInitSettings {
    AkThreadProperties threadLEngine;
    AkThreadProperties threadOutputMgr;
    AkThreadProperties threadBankManager;
    AkThreadProperties threadMonitor;
    AkUInt32           uSampleRate;          // デフォルト 48000
    AkUInt16           uNumRefillsInVoice;   // デフォルト 4
    const char*        szAudioWorkletProcessorUrl;  // デフォルト "WwiseAudioWorklet.processor.js"
    bool               bVerboseSystemOutput;
};
```

### プラットフォーム検出マクロ

`AkTypes.h` が `__EMSCRIPTEN__` → `AK_EMSCRIPTEN` を定義する。
コード内では `#ifdef AK_EMSCRIPTEN` で分岐する。

---

## 変更ファイル一覧

### 既存ファイルの変更（7ファイル）

| ファイル | 変更内容 |
|---|---|
| `addons/Wwise/native/SConstruct` | `web` プラットフォームブランチ、Emscripten_mt ライブラリパス、`.wasm` 出力 |
| `addons/Wwise/native/src/core/wwise_gdextension.cpp` | `#elif defined(AK_EMSCRIPTEN)` 追加、`szAudioWorkletProcessorUrl` 設定 |
| `addons/Wwise/native/src/core/wwise_settings.h` | Web 用プラットフォーム設定フィールド追加 |
| `addons/Wwise/native/src/core/wwise_settings.cpp` | Web 用設定キー追加、`get_setting()` の `#error` 解消 |
| `addons/Wwise/native/src/editor/plugins/ak_editor_export_plugin.cpp` | web プラットフォーム対応追加 |
| `addons/Wwise/native/src/editor/plugins/ak_editor_export_plugin.h` | web ハンドラーメソッド宣言追加 |
| `addons/Wwise/native/wwise.gdextension` | `web.debug` / `web.release` エントリ追加 |

### 新規作成ファイル（1ファイル）

| ファイル | 内容 |
|---|---|
| `addons/Wwise/native/src/editor/plugins/ak_web_export_plugin.cpp` | `WwiseAudioWorklet.processor.js` のエクスポートディレクトリへのデプロイ処理 |
| `addons/Wwise/native/src/editor/plugins/ak_web_export_plugin.h` | 上記の宣言 |

---

## Phase 1: ビルドシステムの確立

### 1-1. `SConstruct` への `web` プラットフォーム追加

**ファイル拡張子の追加:**

```python
elif env["platform"] == "web":
    file_extension = ".wasm"
```

**Wwise SDK ライブラリパスの設定:**

```python
if env["platform"] == "web":
    wwise_soundengine_sample_path = env["wwise_sdk"] + "/samples/SoundEngine/Common/"
    threads_suffix = "Emscripten_mt" if env.get("threads", True) else "Emscripten_st"
    if env["wwise_config"] == "debug":
        wwise_sdk_libs_path = env["wwise_sdk"] + "/{}/Debug/lib/".format(threads_suffix)
    elif env["wwise_config"] == "profile":
        wwise_sdk_libs_path = env["wwise_sdk"] + "/{}/Profile/lib/".format(threads_suffix)
    else:
        wwise_sdk_libs_path = env["wwise_sdk"] + "/{}/Release/lib/".format(threads_suffix)
```

**Web ビルド設定ブロック（他プラットフォームと同様の場所に追加）:**

- `target_path` を `lib/web/` サブディレクトリに設定
- `CommunicationCentral` のリンクは `template_debug` / `editor` ターゲットに限定（既存の条件をそのまま利用可能）
- `AkAutobahn`（WAAPI）は `platform in ("windows", "macos")` 条件で除外されているため変更不要

**出力ファイル（SharedLibrary として `.wasm`）:**

```python
elif env["platform"] == "web":
    library = env.SharedLibrary(
        target=env["target_path"]
        + "libwwise.{}.{}.{}.wasm".format(env["platform"], env["target"], env["wwise_config"]),
        source=sources,
    )
```

godot-cpp の `web.py` が emcc/em++ を CC/CXX にセットするため、SConstruct 側での上書きは不要。

### 1-2. `wwise_gdextension.cpp` — `initialize_wwise_systems()` の修正

現在は末尾の `#else #error "Platform not supported" #endif` で Web ビルドが失敗する。
以下のブランチを追加:

```cpp
#elif defined(AK_EMSCRIPTEN)
    // WwiseAudioWorklet.processor.js の URL を設定
    // ⚠️ szAudioWorkletProcessorUrl は const char* なので static 変数に格納してポインタを渡す
    static std::string worklet_url_static;
    worklet_url_static = wwise_settings->get_setting(
        wwise_settings->platform_settings.web_audio_worklet_processor_url,
        String("WwiseAudioWorklet.processor.js")
    ).utf8().get_data();
    platform_init_settings.szAudioWorkletProcessorUrl = worklet_url_static.c_str();

    platform_init_settings.bVerboseSystemOutput =
        static_cast<bool>(wwise_settings->get_setting(
            wwise_settings->platform_settings.web_verbose_output, false));
```

> **⚠️ ポインタ生存期間の注意**
> `szAudioWorkletProcessorUrl` に渡す `const char*` は `AK::SoundEngine::Init()` 内部でキャッシュされる可能性がある。
> `static std::string` に格納してから `.c_str()` を渡すこと。スタック変数から取得した場合はダングリングポインタになる。

### 1-3. `wwise_settings.h/.cpp` — Web 用設定キーの追加

`wwise_settings.h` の `PlatformSettings` 構造体に以下を追加:

```cpp
// Web
String web_audio_worklet_processor_url;
bool   web_verbose_output;
```

`wwise_settings.cpp` の `get_setting()` に `#elif defined(AK_EMSCRIPTEN)` ブランチを追加して `#error` を解消する。

### 1-4. `wwise_gdextension_main.cpp` — Web 向け条件分岐

`Waapi` は Web 非対応のため除外:

```cpp
#if defined(AK_WIN) || defined(AK_MAC_OS_X)
// Waapi 登録は Web では行わない
```

この条件はすでに存在するため追加変更不要。ただし `AK_EMSCRIPTEN` が他の条件と干渉しないか確認する。

---

## Phase 2: エクスポートプラグイン

### 2-1. `ak_web_export_plugin.h/.cpp` の新規作成

Android の `ak_android_export_plugin` と同様のパターン。

**`ak_web_export_plugin.h`:**

```cpp
#pragma once

#include <godot_cpp/classes/editor_export_plugin.hpp>

using namespace godot;

class AkWebExportPlugin : public EditorExportPlugin {
    GDCLASS(AkWebExportPlugin, EditorExportPlugin);

protected:
    static void _bind_methods() {}

public:
    virtual String _get_name() const override;
    virtual bool _supports_platform(const Ref<EditorExportPlatform>& p_platform) const override;
    virtual void _export_begin(const PackedStringArray& p_features, bool p_is_debug,
                               const String& p_path, uint32_t p_flags) override;
};
```

**`_export_begin()` 内の処理:**

1. Wwise SDK の `samples/SoundEngine/Emscripten/WwiseAudioWorklet.processor.js` を読み込む
2. `add_file()` でエクスポートに含める（PCK には含まれず HTML と同じディレクトリに配置される形が理想だが、GDExtension API の制約に応じて調整）

> **⚠️ アーキテクチャ上の注意**
> `AkWebExportPlugin` はデスクトップ（mac/linux/windows）上の Godot エディターで動作するコード。
> `#ifdef __EMSCRIPTEN__` でガードしてはいけない。`#if defined(TOOLS_ENABLED)` ブロック内に置く。

### 2-2. `wwise_gdextension_main.cpp` への登録追加

```cpp
#if defined(TOOLS_ENABLED)
// 既存の AkEditorPlugin 登録の近くに追加
ClassDB::register_class<AkWebExportPlugin>();
EditorPlugins::add_by_type<AkWebExportPlugin>();
#endif
```

### 2-3. `wwise_gdextension_main.cpp` への `#include` 追加

```cpp
#if defined(TOOLS_ENABLED)
#include "editor/plugins/ak_web_export_plugin.cpp"
// ... 既存の include ...
#endif
```

---

## Phase 3: gdextension と動作確認

### 3-1. `wwise.gdextension` への Web エントリ追加

```ini
web.debug = "res://addons/Wwise/native/lib/web/template_debug/profile/libwwise.web.template_debug.profile.wasm"
web.release = "res://addons/Wwise/native/lib/web/template_release/release/libwwise.web.template_release.release.wasm"
```

> **注意**: Web には `editor` ターゲットは存在しない（Godot の Web エクスポートはデスクトップエディターから実行する）。

### 3-2. Web ビルドコマンド

```bash
source emsdk/emsdk_env.sh

cd addons/Wwise/native

# Debug ビルド
scons platform=web target=template_debug threads=yes \
  wwise_sdk=/Applications/AudioKinetic/Wwise2025.1.3.9039/SDK \
  wwise_config=debug

# Release ビルド
scons platform=web target=template_release threads=yes \
  wwise_sdk=/Applications/AudioKinetic/Wwise2025.1.3.9039/SDK \
  wwise_config=release
```

### 3-3. 動作確認手順

1. Godot エディターで Web エクスポートテンプレートをインストール（Godot 公式、スレッド有効版）
2. テストプロジェクト (`tests/GodotProject/`) を Web エクスポート
3. エクスポートディレクトリに `WwiseAudioWorklet.processor.js` が配置されていることを確認
4. ローカル HTTP サーバーで動作確認（`SharedArrayBuffer` には HTTPS または localhost が必要）

```bash
# ローカルサーバーの起動例
python3 -m http.server 8080 --directory <export_dir>
# ブラウザで http://localhost:8080 を開く
```

> **⚠️ SharedArrayBuffer の要件**
> pthreads（Emscripten_mt）を使用するため、サーバーは以下のヘッダーが必要:
> ```
> Cross-Origin-Opener-Policy: same-origin
> Cross-Origin-Embedder-Policy: require-corp
> ```
> Godot 公式の Python サーバースクリプト (`godot/platform/web/serve.py`) を使うと自動設定される。

---

## 初期実装のスコープ外

| 機能 | 理由 |
|---|---|
| プロファイラー接続（WwiseProfilerServer） | Node.js WebSocket プロキシが別途必要。接続失敗しても crash はしない |
| シングルスレッド（Emscripten_st）対応 | AudioWorklet 設計上スレッドが必要。Godot 公式テンプレートもスレッド有効を推奨 |
| カスタムプラグイン（Reflect, Convolution 等）の Web 対応 | SDK に Emscripten 向け `.a` が存在するか個別確認が必要 |
| Web Audio Input（マイク）対応 | ブラウザの権限 API との統合が必要 |
| IndexedDB ベースのサウンドバンクキャッシュ | パフォーマンス最適化として将来対応 |

---

## 既知のトレードオフと注意事項

### I/O フックは変更不要
`wwise_io_hook.cpp` は Godot の `FileAccess` API のみを使用しており、OS の標準 FILE API に依存していない。
Godot Web エクスポートでは PCK が Emscripten の仮想ファイルシステムにマウントされるため、`FileAccess` が透過的に動作する。

### `static std::string` によるポインタ管理
`AkPlatformInitSettings.szAudioWorkletProcessorUrl` は `const char*` であり、`AK::SoundEngine::Init()` 内部でキャッシュされる可能性がある。
必ず `static std::string` に格納してから `.c_str()` を渡すこと。

### Unity ビルドと `__EMSCRIPTEN__` の自動定義
`wwise_gdextension_main.cpp` が全 `.cpp` を `#include` する Unity ビルド構造のため、emcc でコンパイルすると `__EMSCRIPTEN__` が自動定義される。
`AkTypes.h` がこれを検出して `AK_EMSCRIPTEN` を定義するため、C++ 分岐は `#ifdef AK_EMSCRIPTEN` で統一できる。

### エクスポートプラグインはデスクトップで動作
`AkWebExportPlugin` は `__EMSCRIPTEN__` ではなく `TOOLS_ENABLED` でガードする。
Web 向けの JS ファイルデプロイ処理は、macOS/Linux/Windows のエディター上で実行されるコードである。

# Web (Emscripten) エクスポート運用ガイド

Godot Web エクスポートで Wwise を組み込んだプロジェクトをブラウザ実行するための手順とトラブルシュートをまとめる。

---

## 前提条件

| 要素 | バージョン / パス |
|---|---|
| Wwise SDK | `/Applications/AudioKinetic/Wwise2025.1.3.9039/SDK`（推奨） |
| Emscripten | `emsdk/` 配下、`emcc 4.0.20` 以上（Godot 4.6 要件） |
| Godot | 4.6 系（Thread Support / Extensions Support 対応必須） |
| Wwise Authoring | Web プラットフォームのバンク生成に必要 |

ローカルビルド環境のセットアップは [local-build-setup.md](local-build-setup.md) を参照。

---

## GDExtension 変更時の再ビルドワークフロー

Web 対応の C++ コード（`SConstruct` / `wwise_gdextension.cpp` / `ak_web_export_plugin.*` 等）を変更した場合は、**ターゲット 2 つを両方再ビルド** してから動作確認する必要があります。

### 1. macOS エディタ用 framework

Godot エディタ内で `Web.tres` 自動生成・エクスポート設定確認・GdUnit4 テスト実行に必要:

```bash
cd addons/Wwise/native
uv run scons platform=macos target=editor wwise_sdk=/Applications/AudioKinetic/Wwise2025.1.3.9039/SDK wwise_config=profile
```

出力: `lib/macos/editor/profile/libwwise.macos.editor.profile.framework`

### 2. Web 用 wasm

Web エクスポート時に Godot がロードする側 module:

```bash
source emsdk/emsdk_env.sh
cd addons/Wwise/native
uv run scons platform=web target=template_debug threads=yes wwise_sdk=/Applications/AudioKinetic/Wwise2025.1.3.9039/SDK wwise_config=profile
```

出力: `lib/web/template_debug/profile/libwwise.web.template_debug.profile.wasm`

### 反映手順

C++ 変更ごとに以下を全て実施:

1. 上記 macOS / Web の両ビルドを実行
2. **Godot エディタを再起動**（古い .framework をアンロード）
3. `Project > Export > Web > Export Project` で **再エクスポート**
4. `uv run tools/serve.py` でブラウザ動作確認

> Web 用 `.wasm` だけビルドしてもエディタが古い framework を握り続けるため、エクスポート時に古い挙動が再現することがあります。必ず両方ビルドしてエディタ再起動。

---

## 1. Wwise Authoring 側の準備

1. Wwise Authoring で対象 `.wproj` を開く
2. `Project > Platforms` から **Web** を追加（参照プラットフォーム: Windows）
3. SoundBank Manager で **Web プラットフォームにチェック** → Generate
4. 出力先: `tests/GodotProject/GeneratedSoundBanks/Web/`（リポジトリの構成例）

> `.bnk` ファイルはリポジトリには含めず、各開発者が Authoring で生成する運用です（`GeneratedSoundBanks/` は gitignore 済み）。

---

## 2. Godot 側の設定

### `Web.tres` (WwisePlatformInfo) の自動生成

GDExtension Editor ビルドが新しい場合、Godot エディタで Wwise Browser を開いて SoundBank ディレクトリを再スキャンすると `tests/GodotProject/Wwise/resources/Web.tres` が自動生成される。

内容例:
```
platform_name = "Web"
platform_path = "Web"
```

### Project Settings の登録

自動生成と同時に以下が設定される（手動で確認可）:

- `wwise/project_settings/web_platform_info` → `res://Wwise/resources/Web.tres`

Web 固有の設定:

- `wwise/web_advanced_settings/audio_worklet_processor_url` （デフォルト: `WwiseAudioWorklet.processor.js`）
- `wwise/web_advanced_settings/verbose_system_output` （デフォルト: `false`）

---

## 3. Godot Web エクスポート設定

`Project > Export...` で Web プリセットを作成し、以下を **必ず ON**:

| 項目 | 状態 | 理由 |
|---|---|---|
| **Variant > Thread Support** | ✅ ON | Godot 本体と GDExtension の shared memory を一致させる |
| **Variant > Extensions Support** | ✅ ON | `wwise.gdextension` を読み込むのに必須 |

Thread Support / Extensions Support 対応版の Web エクスポートテンプレートが事前にインストールされている必要があります。

エクスポート時、`AkWebExportPlugin` が `WwiseAudioWorklet.processor.js` をエクスポートディレクトリへ自動コピーします。

---

## 4. ローカル動作確認用 HTTP サーバ

Wwise の AudioWorklet は **`SharedArrayBuffer`** に依存し、ブラウザが以下のヘッダを要求します:

```
Cross-Origin-Opener-Policy: same-origin
Cross-Origin-Embedder-Policy: require-corp
```

`python -m http.server` や VSCode Live Server 拡張ではこれらのヘッダが付かないため動きません。リポジトリ同梱の `tools/serve.py` を使ってください。

```bash
uv run tools/serve.py
# → http://127.0.0.1:8080/ で起動
# → ブラウザで http://127.0.0.1:8080/WwiseGodotTests.html
```

オプション:

```bash
uv run tools/serve.py --root tests/GodotProject/export --port 9090
```

`--root` は省略可（デフォルト `tests/GodotProject/export`）。

---

## 5. 既知の制約

### Emscripten_st (single-thread) ライブラリを使用

`SConstruct` は Web ビルド時に Wwise SDK の **`Emscripten_st/`** ライブラリを参照します。理由:

- Wwise SDK の `Emscripten_mt` 版は Emscripten の `-sAUDIO_WORKLET=1` 機能に依存
- この機能は **メインモジュール（Godot 本体）のリンク時** に指定する必要がある
- Godot 4.6 標準テンプレートは `-sAUDIO_WORKLET=1` 無しでビルドされており、`_mt` 版とは互換性が無い

`_st` 版は AudioWorklet を使わないため、Godot 標準テンプレートと組み合わせて動作します。トレードオフは pthread によるオーディオレンダリングの分離が失われる点。

### 初回ユーザー操作前は AudioContext が suspended

ブラウザの自動再生ポリシーにより、ユーザーがクリック・タップ等を行うまで `AudioContext` は `suspended` 状態で起動します。この間 Wwise は:

- `ErrorCode_AudioThreadSuspended`
- `ErrorCode_AudioSubsystemStoppedResponding`
- `ErrorCode_AudioThreadResumed`（復帰時）

を Monitor に送りますが、Web 上は正常な過渡状態なため GDExtension 側でフィルタしてコンソール表示を抑制しています ([wwise_gdextension.cpp](../addons/Wwise/native/src/core/wwise_gdextension.cpp))。

### `_st` 構成によるメインスレッド競合と Voice Starvation

`Emscripten_st` 版は Wwise のオーディオレンダリングをメインスレッドで実行します。そのため以下の状況で一時的に `ErrorCode_VoiceStarvation` が発生する可能性があります:

- AudioContext が resume した直後（バッファがまだ温まっていない）
- UI 再描画 / GDScript 処理 / `set_rtpc_value_id` 呼び出し等がメインスレッドで集中したタイミング
- 起動直後にスライダー等を素早く操作した場合

`_mt`（マルチスレッド）版なら別スレッドでオーディオを処理できるため発生しませんが、本リポジトリでは Godot 4.6 標準テンプレートとの互換性を優先し `_st` 構成を採用しています（前項参照）。一過性のエラーであり再生は継続するため、現状は許容しています。

### 他の制約

- プロファイラー接続（WwiseProfilerServer）は別途 Node.js プロキシが必要なため未対応
- カスタム DSP プラグイン（Reflect, Convolution 等）は SDK の `Emscripten_st/lib` に存在するもののみ動作

---

## 6. トラブルシュート

### `TypeError: resolved is not a function`

GDExtension が要求するシンボルを Godot 本体が提供できていないときに発生します。失敗シンボル名を特定する手順:

1. エクスポート後、`tests/GodotProject/export/WwiseGodotTests.js` を一時的に編集:
   ```js
   // 既存:
   stubs[prop]=(...args)=>{resolved||=resolveSymbol(prop);return resolved(...args)}
   // パッチ後:
   stubs[prop]=(...args)=>{resolved||=resolveSymbol(prop);if(!resolved){console.error("[Wwise debug] UNRESOLVED SYMBOL:",prop);throw new Error("Unresolved symbol: "+prop);}return resolved(...args)}
   ```
2. ブラウザで再読み込みすると DevTools コンソールに失敗シンボル名が出る
3. C 関数で実装 or EM_JS で JS バインディングを提供して再ビルド
4. 再エクスポートでパッチは上書きされるので、確認後に再エクスポート

### Loading で固まる（エラー無し）

`busy-wait deadlock` の典型。`AK::SoundEngine::Init()` が JS 側の非同期処理（AudioWorklet 等）を待ち続けています。Wwise SDK の `Emscripten_mt` 版を使うと発生するため、`Emscripten_st` 版を使用してください（現状の SConstruct はそうなっています）。

### `imported shared memory but unshared required`

Godot 本体（shared memory）と GDExtension（unshared）の memory model 不一致。SConstruct で `threads=yes` でビルドしているか確認してください。

### `.bnk` ファイルが見つからない

エラー例:
```
Cannot open file Init.bnk in path(s):
```

原因と対処:
- Wwise Authoring で **Web プラットフォームの SoundBank が生成されていない** — `GeneratedSoundBanks/Web/` 配下に `.bnk` があるか確認
- `Web.tres` の `platform_path` が誤っている — `"Web"` になっているか確認
- エクスポート時 `.bnk` がパックされていない — エディタを最新の GDExtension ビルドで再起動してから再エクスポート

---

## 動作確認の手順

Web エクスポートが正しく動くかは、以下の 3 段階で検証します。

### A. GdUnit4 でメタテスト（macOS エディタ上）

Godot エディタで `tests/GodotProject/` を開き、GdUnit Runner から `tests/GodotProject/test/test_web_export.gd` を実行。以下を検証します:

- Web 用 ProjectSettings の登録（`web_audio_worklet_processor_url`, `web_verbose_output`, `web_platform_info`）
- `AkWebExportPlugin` クラスが ClassDB に登録されている
- `wwise.gdextension` に `web.debug` / `web.release` エントリと `.wasm` 拡張子が含まれる

期待結果: 5/5 PASSED。これが通らない場合は GDExtension のビルドが古い・反映されていない可能性。

### B. macOS デバッグでランタイム動作確認

`tests/GodotProject/scenes/main.tscn` がテスト用の最小シーンです:

| ノード | 役割 |
|---|---|
| `Node2D` (root) | コンテナ |
| `AkEvent2D` | `Switch` WwiseEvent を割り当て、ボタンから `post_event()` 呼び出し |
| `AkListener2D` | 2D リスナー（音源との位置関係を解決） |
| `Button` | クリックで `Switch` イベント発火 ([scripts/switch.gd](../tests/GodotProject/scripts/switch.gd)) |
| `HSlider` | `SwitchVolume` RTPC をリアルタイム変更 ([scripts/switch_volume.gd](../tests/GodotProject/scripts/switch_volume.gd)) |

シーンを Godot エディタで実行し、ボタンクリックで音が鳴り、スライダーで音量が変わることを確認。これが通れば Wwise ランタイム呼び出しが正常です。

### C. Web エクスポートでブラウザ動作確認

1. 「GDExtension 変更時の再ビルドワークフロー」を実施
2. Wwise Authoring で Web 用 SoundBank を生成（**1. Wwise Authoring 側の準備** 参照）
3. Godot から Web エクスポート（**3. Godot Web エクスポート設定** 参照）
4. `uv run tools/serve.py` でローカル配信
5. ブラウザで `http://127.0.0.1:8080/WwiseGodotTests.html` を開く
6. ページ上のボタンを **クリック**（初回ユーザー操作で AudioContext が resume）
7. 音が鳴り、スライダーでボリュームが変わることを確認

エラー: Console に `[Wwise debug] UNRESOLVED SYMBOL: ...` 等が出る場合は **6. トラブルシュート** 参照。

---

## 関連ドキュメント

- [local-build-setup.md](local-build-setup.md) — Python / SCons / uv セットアップ
- [../CLAUDE.md](../CLAUDE.md) — プロジェクト全体のビルド情報

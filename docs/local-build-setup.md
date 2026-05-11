# ローカルビルド環境セットアップ

GDExtension (C++) を SCons でビルドするためのローカル環境構築手順。

## バージョン要件

| ツール | バージョン | 備考 |
|---|---|---|
| Python | 3.12.13 | CI は `3.x` だが SCons 4.7.0 公式対応上限が 3.12 |
| SCons | 4.7.0 | `.github/workflows/build_all.yml` でピン留め |
| uv | 0.11+ | venv/パッケージ管理 |
| pyenv | 任意 | anyenv 経由でも可 |

## セットアップ手順

### 1. Python のインストール

anyenv + pyenv を使う場合:

```bash
anyenv install pyenv          # 未インストールの場合のみ
pyenv install 3.12.13
```

リポジトリルートで:

```bash
pyenv local 3.12.13           # .python-version を作成
```

### 2. uv のインストール

```bash
brew install uv               # 未インストールの場合のみ
```

### 3. 仮想環境と SCons

リポジトリルートで:

```bash
uv venv .venv --python 3.12.13
uv pip install --python .venv/bin/python scons==4.7.0
```

動作確認:

```bash
.venv/bin/scons --version
# SCons: v4.7.0... が表示されれば OK
```

### 4. アクティベート

セッションごとに 1 回:

```bash
source .venv/bin/activate
```

以後 `scons` コマンドが直接使えます。

## ビルドコマンド

`addons/Wwise/native/` 内で実行（[CLAUDE.md](../CLAUDE.md) 参照）:

```bash
cd addons/Wwise/native

# macOS エディタビルド
scons platform=macos target=editor wwise_sdk=/Applications/AudioKinetic/Wwise2025.1.3.9039/SDK

# macOS ランタイムビルド
scons platform=macos target=template_debug wwise_sdk=/path/to/SDK wwise_config=debug
scons platform=macos target=template_release wwise_sdk=/path/to/SDK wwise_config=release
```

### Web (Emscripten) ビルド

事前準備 ([CLAUDE.md](../CLAUDE.md) 参照):

```bash
source emsdk/emsdk_env.sh     # リポジトリルートの emsdk/ を読み込む
```

ビルド:

```bash
cd addons/Wwise/native

scons platform=web target=template_debug threads=yes \
  wwise_sdk=/Applications/AudioKinetic/Wwise2025.1.3.9039/SDK \
  wwise_config=debug
```

詳細は [web-export-implementation-plan.md](web-export-implementation-plan.md) を参照。

## .gitignore に含まれるもの

このセットアップで生成される個人環境ファイル:

- `.venv/` — Python 仮想環境
- `.python-version` — pyenv のバージョン指定（個人運用なら無視、チーム共有なら gitignore から外す）
- `emsdk/` — Emscripten SDK
- `*.code-workspace` — VSCode ワークスペース設定

## トラブルシュート

### `scons not found`

`.venv` を activate していないか、`.venv/bin/scons` を直接呼んでください。

### `import SCons` エラー

`uv pip install scons==4.7.0` が `.venv` に対して実行されているか `.venv/bin/python -m pip list` で確認。

### Python バージョンが違う

```bash
.venv/bin/python --version    # 3.12.13 になっているか確認
```

異なる場合は `.venv` を削除して `uv venv .venv --python 3.12.13` をやり直してください。

### CI との挙動差

CI は `python-version: 3.x` を `actions/setup-python@v5` で resolve するため、将来 3.13/3.14 になる可能性があります。ローカルが 3.12.13 で動けば SCons 4.7.0 の公式サポート範囲内のため、ローカルでの再現は基本的に信頼できます。

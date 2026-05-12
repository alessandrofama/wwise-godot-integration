# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

GDExtension-based integration of Audiokinetic's Wwise audio middleware with Godot Engine 4.3+. The C++ library wraps the Wwise SDK and exposes it as Godot nodes, resources, and singletons via GDScript.

## Initial Setup

`godot-cpp` is the only submodule. Fetch it before building:

```bash
git submodule update --init --recursive
```

**Web (Emscripten) ビルドの準備:**  
`emsdk/` はリポジトリルートに配置済み（`.gitignore` 除外、git 管理外）。
Web ビルド前に以下で環境変数を有効化する:

```bash
source emsdk/emsdk_env.sh   # emcc 4.0.23 が PATH に追加される
```

emsdk が未インストールの場合:

```bash
git clone https://github.com/emscripten-core/emsdk.git emsdk
cd emsdk
./emsdk install 4.0.23
./emsdk activate 4.0.23
```

## Build Commands

All build commands run from `addons/Wwise/native/`. The Wwise SDK path must be provided.

```bash
# macOS editor build
scons platform=macos target=editor wwise_sdk=/path/to/WwiseSDK

# macOS runtime builds
scons platform=macos target=template_debug wwise_sdk=/path/to/WwiseSDK wwise_config=debug
scons platform=macos target=template_release wwise_sdk=/path/to/WwiseSDK wwise_config=release
```

**Key SCons options:**
- `platform`: `windows` | `macos` | `linux` | `ios` | `android`
- `target`: `editor` | `template_debug` | `template_release`
- `wwise_config`: `debug` | `profile` | `release`
- `wwise_sdk`: path to Wwise SDK root directory
- `plugins`: comma-separated list (`reflect`, `motion`, `convolution`, `soundseed_grain`, `soundseed_air`, `impacter`, `mastering_suite`)
- `dev_build=yes` / `asserts=yes`: enable debug features

**Android** (from `addons/Wwise/native/android/`):
```bash
./gradlew build
```

## Architecture

### Unity Build Pattern

All C++ source files are `#include`d into a single translation unit via `src/wwise_gdextension_main.cpp`. This file is the sole compilation unit — **do not add `.cpp` files to SCons sources directly**; instead include them in `wwise_gdextension_main.cpp`.

### Class Registration

`wwise_gdextension_main.cpp` also contains `register_wwise_types()` / `unregister_wwise_types()`. Every new Godot-exposed class must be registered there with `ClassDB::register_class<T>()` at the appropriate initialization level (`SCENE` or `EDITOR`).

### Module Layout

```
src/
  core/           — Wwise singleton (Wwise), AkUtils, I/O hook, settings, platform info
  core/types/     — Godot Resource subclasses for Wwise objects (WwiseEvent, WwiseBank, etc.)
  scene/          — Godot Node subclasses (AkEvent2D/3D, AkListener, AkBank, AkGeometry, …)
  editor/         — Editor-only: Wwise Browser, inspector properties, project database
  editor/plugins/ — EditorPlugin subclasses registered via EditorPlugins::add_by_type<>()
  editor/properties/ — Custom inspector property editors for each Wwise type
  platform/       — Platform-specific code (Android JNI)
```

Editor code is gated behind `#if defined(TOOLS_ENABLED)` / `#ifdef TOOLS_ENABLED`.  
WAAPI (authoring API) is gated behind `#if defined(AK_WIN) || defined(AK_MAC_OS_X)`.

### Wwise Type Hierarchy

`WwiseBaseType` (Resource) is the base for all Wwise asset types. Each type (Event, Bank, RTPC, State, Switch, Trigger, AuxBus, AcousticTexture) stores `name`, `id` (short ID), and `guid`. They are saved as `.tres` files under the path configured in `AkEditorSettings`.

### Singletons Registered with the Engine

| C++ class | GDScript name | Init level |
|---|---|---|
| `Wwise` | `Wwise` | SCENE |
| `AkUtils` | `AkUtils` | SCENE |
| `WwiseSettings` | — (internal) | SCENE |
| `Waapi` | `Waapi` | EDITOR (Win/Mac only) |
| `WwiseProjectDatabase` | `WwiseProjectDatabase` | EDITOR |

## C++ Conventions

- **Indentation**: tabs
- **Headers**: `#pragma once`
- **Namespaces**: `using namespace godot;` at file scope
- **Class macros**: `GDCLASS(ClassName, ParentClass)` inside every exposed class
- **Method binding**: `static void _bind_methods()` in `protected:`
- **Singletons**: `static ClassName* singleton = nullptr;` + `ERR_FAIL_COND` guards in constructor/destructor
- **Naming**: PascalCase classes, snake_case methods/members, `UPPER_SNAKE_CASE` enum constants
- **Ak prefix**: Godot scene nodes (e.g., `AkEvent3D`, `AkListener2D`); **Wwise prefix**: Wwise SDK type wrappers (e.g., `WwiseEvent`, `WwiseBank`)
- **Error handling**: `ERR_FAIL_COND(...)` / `ERR_FAIL_COND_V(...)` for precondition checks
- No auto-formatter is configured; maintain existing style manually

## Adding a New Exposed Class

1. Create `src/<module>/my_class.h` and `src/<module>/my_class.cpp`
2. Include the `.cpp` in `src/wwise_gdextension_main.cpp`
3. Register in `register_wwise_types()` with `ClassDB::register_class<MyClass>()`
4. If editor-only, wrap in `TOOLS_ENABLED` guards
5. Add XML documentation to `doc_classes/MyClass.xml` (triggers doc rebuild in SCons)

## Testing

Tests use GdUnit4. Open `tests/GodotProject/` in the Godot Editor and run `tests/GodotProject/test/test_wwise.gd`. There is no CLI test runner.

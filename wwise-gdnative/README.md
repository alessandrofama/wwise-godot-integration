# Wwise GDNative

## Build instructions

Building the Wwise GDNative library requires that the Godot bindings have been previously built at `godot_cpp/bin` e.g. `libgodot-cpp.windows.debug.64.lib`

### Windows 64-bit (Scons)

You might need to execute `vcvars64.bat` before building. For VS2017, it's located at `C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build`

Debug (includes Wwise profiler connection)

```
scons target=debug platform=windows wwise_sdk="%WWISESDK%"
```

Release

```
scons target=release platform=windows wwise_sdk="%WWISESDK%"
```

### macOS 64-bit (Scons)

TODO

### Android (ndk-build)

Available target configurations:
* debug_android_armeabi-v7a
* debug_android_x86
* debug_android_arm64-v8a
* debug_android_x86_64
* release_android_armeabi-v7a
* release_android_x86
* release_android_arm64-v8a
* release_android_x86_64

Example command (replace PM5_CONFIG with your desired configuration):

```
%ANDROID_NDK_ROOT%/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk PM5_CONFIG=debug_android_arm64-v8a NDK_LIBS_OUT=./gdnative-demo/wwise/bin/android/debug -j4
```

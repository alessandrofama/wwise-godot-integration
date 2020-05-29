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
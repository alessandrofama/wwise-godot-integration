# Waapi GDNative

## Build instructions

Building the Waapi GDNative library requires that the Godot bindings have been previously built at `godot_cpp/bin` e.g. `libgodot-cpp.windows.debug.64.lib`
The Waapi wrapper is only available for Windows and macOS 64-bit.

### Windows 64-bit (Scons)

You might need to execute `vcvars64.bat` before building. For VS2017, it's located at `C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build`
The build requires that the %WWISESDK% symbol is defined in the system (it's defined by default after installing Wwise on Windows). Otherwise, you'd need to specify the path to your Wwise SDK installation.

Debug

```
scons target=debug platform=windows wwise_sdk="%WWISESDK%"
```

Release

```
scons target=release platform=windows wwise_sdk="%WWISESDK%"
```

### macOS 64-bit (Scons)

The main difference with Windows is that the build requires the Wwise SDK path passed in the build call (see examples below).

Debug

```
scons target=debug platform=osx wwise_sdk=/Applications/Audiokinetic/Wwise\ 2019.2.1.7250/SDK
```

Release

```
scons target=release platform=osx wwise_sdk=/Applications/Audiokinetic/Wwise\ 2019.2.1.7250/SDK
```

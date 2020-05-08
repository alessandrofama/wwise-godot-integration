# Wwise GDNative

## Build instructions (Scons)

### Windows 64-bit

You might need to execute `vcvars64.bat` before building. For VS2017, it's located at `C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build`

Debug (includes Wwise profiler connection)

```
scons target=debug platform=windows target_name=wwise-gdnative-debug wwise_sdk="%WWISESDK%"
```

Release

```
scons target=release platform=windows target_name=wwise-gdnative-release wwise_sdk="%WWISESDK%"
```

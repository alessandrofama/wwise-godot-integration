# Wwise GDNative

## Build instructions (Scons)

### Windows 64-bit

Debug (includes Wwise profiler connection)

```
scons target=debug platform=windows target_name=wwise-gdnative-debug wwise_sdk="%WWISESDK%"
```

Release

```
scons target=release platform=windows target_name=wwise-gdnative-release wwise_sdk="%WWISESDK%"
```
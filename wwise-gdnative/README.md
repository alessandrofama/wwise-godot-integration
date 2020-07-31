# Wwise GDNative

## Build instructions

Building the Wwise GDNative library requires that the Godot bindings have been previously built at `godot_cpp/bin` e.g. `libgodot-cpp.windows.debug.64.lib`

### Windows 64-bit (Scons)

You might need to execute `vcvars64.bat` before building. For VS2017, it's located at `C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build`
The build requires that the %WWISESDK% symbol is defined in the system (it's defined by default after installing Wwise on Windows). Otherwise, you'd need to specify the path to your Wwise SDK installation.

Debug (includes Wwise profiler connection)

```
scons target=debug platform=windows wwise_sdk="%WWISESDK%"
```

Release

```
scons target=release platform=windows wwise_sdk="%WWISESDK%"
```

### macOS 64-bit (Scons)

The main difference with Windows is that the build requires the Wwise SDK path passed in the build call (see examples below).

Debug (includes Wwise profiler connection)

```
scons target=debug platform=osx wwise_sdk=/Applications/Audiokinetic/Wwise\ 2019.2.1.7250/SDK
```

Release

```
scons target=release platform=osx wwise_sdk=/Applications/Audiokinetic/Wwise\ 2019.2.1.7250/SDK
```


### Linux 64-bit (Scons)

Building the Wwise GDNative library works exactly like Windows and macOS. You probably have to copy the Wwise SDK and the generated soundbanks from a Windows or macOS machine to your Linux machine, since the SDK and the authoring application can only be installed on these first two operating systems. 

Debug (includes Wwise profiler connection)

```
scons target=debug platform=linux wwise_sdk='/home/alessandro/Documents/wwisesdk'
```

Release

```
scons target=release platform=linux wwise_sdk='/home/alessandro/Documents/wwisesdk'
```

### iOS ARM 64-bit (Scons)

Other architectures like armv7 are also supported, though arm64 examples are mentioned below. Please check the Sconstruct for details.

Debug (includes Wwise profiler connection)

```
scons target=debug platform=ios ios_arch=arm64 wwise_sdk=/Applications/Audiokinetic/Wwise\ 2019.2.1.7250/SDK
```

Release

```
scons target=release platform=ios ios_arch=arm64 wwise_sdk=/Applications/Audiokinetic/Wwise\ 2019.2.1.7250/SDK
```

### Android (ndk-build)

To build the Android GDNative library we are using ndk-build. 

The available debug target configurations are the following (includes Wwise profiler connection):
* debug_android_armeabi-v7a
* debug_android_x86
* debug_android_arm64-v8a
* debug_android_x86_64

The release target configurates are the following: 
* release_android_armeabi-v7a
* release_android_x86
* release_android_arm64-v8a
* release_android_x86_64

Example command (replace PM5_CONFIG with your desired configuration):

```
%ANDROID_NDK_ROOT%/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk PM5_CONFIG=debug_android_arm64-v8a NDK_LIBS_OUT=./gdnative-demo/wwise/bin/android/debug -j4
```

Having whitespaces in your NDK, Wwise SDK or output libs paths will likely result in errors while running ndk-build.

### Plugins (Scons)

The integration supports all factory plugins and the following premium plugins:

| Plugin | Compilation Parameter | Supported Platforms |
|-|-|-|
| Reflect | reflect | All
| Motion | motion | Windows
| Convolution | convolution | All
| SoundSeed Air + SoundSeed Impact | soundseed_air_impact | All
| SoundSeed Grain | soundseed_grain | All

Add the plugins separated by comma as a compilation parameter to Scons (example):

```
scons target=release platform=windows wwise_sdk="%WWISESDK% plugins=convolution,reflect,motion
```

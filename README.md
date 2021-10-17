# Wwise Godot Integration
![Windows Builds](https://github.com/alessandrofama/wwise-godot-integration/workflows/Windows%20Builds/badge.svg)
![macOS Builds](https://github.com/alessandrofama/wwise-godot-integration/workflows/macOS%20Builds/badge.svg)
![Linux Builds](https://github.com/alessandrofama/wwise-godot-integration/workflows/Linux%20Builds/badge.svg)
![Android Builds](https://github.com/alessandrofama/wwise-godot-integration/workflows/Android%20Builds/badge.svg)
![iOS Builds](https://github.com/alessandrofama/wwise-godot-integration/workflows/iOS%20Builds/badge.svg)
![Integration App Windows](https://github.com/alessandrofama/wwise-godot-integration/workflows/Integration%20App%20Windows/badge.svg)
![Integration App macOS](https://github.com/alessandrofama/wwise-godot-integration/workflows/Integration%20App%20macOS/badge.svg)

[![Wwise logo](/wwise-logo.png)](https://www.audiokinetic.com/products/wwise/)
[![Godot Engine logo](/godot-logo.png)](https://godotengine.org)

Welcome! This repository provides an integration of [Audiokinetic's Wwise audio middleware](https://www.audiokinetic.com/products/wwise/) into the [Godot game engine](https://godotengine.org). At the moment, Wwise version 2021.1.4.7707 is supported. Older versions can be downloaded in the releases section of the repository.


## Features

[<img src="https://i.imgur.com/PrMMagV.png" width="50%">](https://www.youtube.com/watch?v=rO42WLEnC18) 

* Wwise wrapper libraries that can be used in games and apps for Windows, macOS, Linux, iOS and Android. These are provided as [GDNative modules](https://docs.godotengine.org/en/stable/tutorials/plugins/gdnative/index.html).
* Integration app to ease the process of integrating Wwise into Godot projects.
* Multi-platform build with Scons for the Wwise and Wwise authoring API (Waapi) wrappers. Visual Studio 2017 and 2019 solutions are provided to build on Windows.
* Wwise and Waapi wrappers unit tests with [Gut](https://github.com/bitwes/Gut).
* Wwise and Waapi testbed projects.
* Wwise profiler connection enabled in debug builds of the GDNative wrapper.
* Stream manager uses the default blocking I/O implementation and it can be extended to use custom I/O devices.
* Custom Godot nodes: AkEvent, AkEvent2D, AkBank, AkListener, AkListener2D, AkState, AkSwitch, AkEnvironment, AkGeometry, AkRoom, AkPortal, AkEarlyReflections.
* Custom Godot Visual Script nodes: AkLoadBank, AkUnloadBank, AkPostEvent, AkPostEvent2D, AkSet3DPosition, AkSet2DPosition, AkSetState, AkSetSwitch, AkGetRTPCValue, AkSetRTPCValue.
* Custom Godot signals that can be mapped to event callbacks: end of event, audio marker, music sync beat etc.
* Wwise IDs converter from C++ to GDScript code in the Godot editor as addon.
* Waapi picker that allows querying and searching the Wwise project and generating soundbanks from Godot's editor.
* Wwise settings configurable per-platform in the Godot editor.
* Spatial audio API support.
* Wwise plug-ins support.

## Getting started

If you are a game developer that would like to integrate Wwise into your Godot project, go to the [releases](https://github.com/alessandrofama/wwise-godot-integration/releases) page to download the integration app and files and read the [getting started](https://github.com/alessandrofama/wwise-godot-integration/wiki/Getting-Started) guide. Don't forget to also check out the [tutorials](https://github.com/alessandrofama/wwise-godot-integration/wiki/Tutorials) on the wiki.

If you would like to modify the code of the GDNative wrappers or build them yourself, go to the [Wwise GDNative](https://github.com/alessandrofama/wwise-godot-integration/tree/main/wwise-gdnative) or [Waapi GDNative](https://github.com/alessandrofama/wwise-godot-integration/tree/main/waapiclient-gdnative) pages for build instructions.

## Documentation

The docs for the GDNative interfaces, custom nodes and addons can be accessed through the [wiki](https://github.com/alessandrofama/wwise-godot-integration/wiki).

## Known issues

The known issues are being logged into the [issues page](https://github.com/alessandrofama/wwise-godot-integration/issues) of this repo. Please check this page first, then log a new issue if you haven't found it listed.

## Contributing

Any contribution is welcome. Please send pull-requests with fixes or additions!

## License

Wwise is an audio middleware that is free for non-commercial projects. A limited commercial license (Wwise Project may contain a maximum of 500 sound files/media assets) is also available for free. Otherwise, you will need to pay for a license. Please visit https://www.audiokinetic.com/pricing/ for details.

The integration files, code and scripts in this repository are released under the [MIT license](https://github.com/alessandrofama/wwise-godot-integration/blob/master/LICENSE).

# Wwise Godot Integration

[![Wwise logo](/wwise-logo.png)](https://www.audiokinetic.com/products/wwise/)
[![Godot Engine logo](/godot-logo.png)](https://godotengine.org)

Welcome! This repository provides an integration of [Audiokinetic's Wwise audio middleware](https://www.audiokinetic.com/products/wwise/) into the [Godot game engine](https://godotengine.org). At the moment, Wwise version 2019.2.1.7250 is supported.

## Features

* Wwise wrapper libraries that can be used in games and apps for Windows, macOS, iOS and Android.
* Multi-platform build with Scons for the Wwise and Wwise authoring API (Waapi) wrappers.
* Wwise and Waapi wrappers unit tests with [Gut](https://github.com/bitwes/Gut).
* Wwise and Waapi testbed projects.
* Wwise profiler connection enabled in debug buids of the GDNative wrapper.
* Blocking I/O implementation that can be extended to use custom I/O devices.
* Custom Godot nodes: AkEvent, AkBank, AkListener, AkState, AkSwitch, AkEnvironment, AkGeometry, AkRoom, AkPortal, AkEarlyReflections.
* Custom Godot Visual Script nodes: AkLoadBank, AkUnloadBank, AkPostEvent, AkSet3DPosition, AkSetState, AkSetSwitch, AkGetRTPCValue, AkSetRTPCValue.
* Custom Godot signals that can be mapped to event callbacks: end of event, audio marker, music sync beat etc.
* Wwise IDs converter from C++ to GDScript code in the Godot editor.
* Waapi picker that allows querying and searching the Wwise project and generating soundbanks from Godot's editor.
* Wwise settings configurable per-platform in the Godot editor.
* Spatial audio API support.
* Wwise plug-ins support.
* Integration app to ease the process of integrating Wwise into Godot projects.

## Getting started

If you are a game developer that would like to integrate Wwise into your Godot project, go to the [releases](https://github.com/alessandrofama/wwise-godot-integration/releases) page to download the integration app and files.

If you would like to modify the code of the GDNative wrappers or build them yourself, go to the [Wwise GDNative](https://github.com/alessandrofama/wwise-godot-integration/tree/master/wwise-gdnative) or [Waapi GDNative](https://github.com/alessandrofama/wwise-godot-integration/tree/master/waapi-gdnative) pages for build instructions.

## Documentation

TODO: list documentation pages. Wrapper docs in the wiki. Official Wwise docs.

## Known issues

TODO: list of known issues or things that aren't working as expected

## License

Wwise is a commercial audio middleware so you must get a license in order to use it in your projects. Please visit https://www.audiokinetic.com/pricing/ for details.

The integration files, code and scripts are released under MIT license. This means you can use the integration in your projects (commercial or not), for free.

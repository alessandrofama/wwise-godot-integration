# Wwise Godot Integration
[![Build Status](https://travis-ci.com/alessandrofama/wwise-godot-integration.svg?token=BfcnymVgT84X7d4hjzaJ&branch=master)](https://travis-ci.com/alessandrofama/wwise-godot-integration)

[![Wwise logo](/wwise-logo.png)](https://www.audiokinetic.com/products/wwise/)
[![Godot Engine logo](/godot-logo.png)](https://godotengine.org)

Welcome! This repository provides an integration of [Audiokinetic's Wwise audio middleware](https://www.audiokinetic.com/products/wwise/) into the [Godot game engine](https://godotengine.org). At the moment, Wwise version 2019.2.1.7250 is supported.

## Features

* Wwise wrapper libraries that can be used in games and apps for Windows, macOS, Linux, iOS and Android. These are provided as [GDNative modules](https://docs.godotengine.org/en/stable/tutorials/plugins/gdnative/index.html).
* Integration app to ease the process of integrating Wwise into Godot projects.
* Multi-platform build with Scons for the Wwise and Wwise authoring API (Waapi) wrappers. Visual Studio 2017 and 2019 solutions are provided to build on Windows.
* Wwise and Waapi wrappers unit tests with [Gut](https://github.com/bitwes/Gut).
* Wwise and Waapi testbed projects.
* Wwise profiler connection enabled in debug builds of the GDNative wrapper.
* Stream manager uses the default blocking I/O implementation and it can be extended to use custom I/O devices.
* Custom Godot nodes: AkEvent, AkBank, AkListener, AkState, AkSwitch, AkEnvironment, AkGeometry, AkRoom, AkPortal, AkEarlyReflections.
* Custom Godot Visual Script nodes: AkLoadBank, AkUnloadBank, AkPostEvent, AkSet3DPosition, AkSetState, AkSetSwitch, AkGetRTPCValue, AkSetRTPCValue.
* Custom Godot signals that can be mapped to event callbacks: end of event, audio marker, music sync beat etc.
* Wwise IDs converter from C++ to GDScript code in the Godot editor as addon.
* Waapi picker that allows querying and searching the Wwise project and generating soundbanks from Godot's editor.
* Wwise settings configurable per-platform in the Godot editor.
* Spatial audio API support.
* Wwise plug-ins support.

## Getting started

If you are a game developer that would like to integrate Wwise into your Godot project, go to the [releases](https://github.com/alessandrofama/wwise-godot-integration/releases) page to download the integration app and files. Don't forget to check out the [tutorials](https://github.com/alessandrofama/wwise-godot-integration/wiki/Tutorials) on the wiki.

If you would like to modify the code of the GDNative wrappers or build them yourself, go to the [Wwise GDNative](https://github.com/alessandrofama/wwise-godot-integration/tree/master/wwise-gdnative) or [Waapi GDNative](https://github.com/alessandrofama/wwise-godot-integration/tree/master/waapi-gdnative) pages for build instructions.

## Documentation

The docs for the GDNative interfaces, custom nodes and addons can be accessed through the [wiki](https://github.com/alessandrofama/wwise-godot-integration/wiki).

## Known issues

The known issues are being logged into the [issues page](https://github.com/alessandrofama/wwise-godot-integration/issues) of this repo. Please check this page first, then log a new issue if you haven't found it listed.

## Contributing

Any contribution is welcome. Please send pull-requests with fixes or additions!

## License

Wwise is an audio middleware that is free for non-commercial projects. Otherwise, you will need to pay for a license. Please visit https://www.audiokinetic.com/pricing/ for details.

The integration files, code and scripts in this repository are released under the [MIT license](https://github.com/alessandrofama/wwise-godot-integration/blob/master/LICENSE).

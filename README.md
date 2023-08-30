# Wwise Godot Integration
[![Build](https://github.com/alessandrofama/wwise-godot-integration/workflows/Build/badge.svg)](https://github.com/alessandrofama/wwise-godot-integration/actions/workflows/build_all.yml) 
[![Discord](https://img.shields.io/discord/1128222869898416182?label=Discord&logo=discord&logoColor=ffffff&labelColor=5865F2&color=5865F2)](https://discord.gg/6tHrP3gaYf) 

Welcome! This repository provides an integration of [Audiokinetic's Wwise audio middleware](https://www.audiokinetic.com/products/wwise/) into [Godot Engine](https://godotengine.org) 4.1.1. At the moment, Wwise version 2023.1 Beta is supported. Older versions can be downloaded in the releases section of the repository.

## Features

* [GDExtension](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/what_is_gdextension.html)-based Wwise wrapper library for Windows, macOS, Linux, and iOS (with future Android compatibility).
* Multi-platform build with SCons for Wwise and Wwise Authoring API wrappers. Visual Studio 2022 solution provided for Windows development.
* Wwise profiler connection available in GDExtension debug builds.
* Stream manager uses the default blocking I/O implementation and it can be extended to use custom I/O devices.
* Custom Godot nodes: AkEvent3D, AkEvent2D, AkBank, AkListener3D, AkListener2D, AkState, AkSwitch, AkEnvironment, AkGeometry, AkRoom, AkPortal, AkEarlyReflections.
* Event callbacks exposed as Signals in AkEvent nodes.
* WAAPI picker for querying Wwise projects, generating soundbanks from Godot's editor.
* C++ to GDScript Wwise IDs converter tool embedded in WAAPI Picker.
* Configurable per-platform Wwise settings in Godot editor.
* Support for spatial audio API.
* Compatibility with Wwise plug-ins.


## Getting started

If you are a game developer that would like to integrate Wwise into your Godot project, go to the [releases](https://github.com/alessandrofama/wwise-godot-integration/releases) page to download the addon and read the [getting started](https://github.com/alessandrofama/wwise-godot-integration/wiki/Getting-Started-(4.0)) guide.

If you would like to modify the code of the GDExtensions or build them yourself, go to the [Building tutorial](https://github.com/alessandrofama/wwise-godot-integration/wiki/Building-the-GDExtension-Library) for build instructions.

## Documentation

The docs for the GDExtensions interfaces, custom nodes and addons can be accessed through the [wiki](https://github.com/alessandrofama/wwise-godot-integration/wiki).

## Known issues

The known issues are being logged into the [issues page](https://github.com/alessandrofama/wwise-godot-integration/issues) of this repo. Please check this page first, then log a new issue if you haven't found it listed.

## Contributing

Any contribution is welcome. Please send pull-requests with fixes or additions!

## License

Audiokinetic offers a free Wwise license for indie developers. For detailed licensing information, visit https://www.audiokinetic.com/en/pricing/. 

The licensing details for integration files, code, and scripts within this repository can be found [here](https://github.com/alessandrofama/wwise-godot-integration/blob/master/LICENSE).

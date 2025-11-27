# Wwise Godot Integration
[![Build](https://github.com/alessandrofama/wwise-godot-integration/workflows/Build/badge.svg)](https://github.com/alessandrofama/wwise-godot-integration/actions/workflows/build_all.yml) 
[![Discord](https://img.shields.io/discord/1128222869898416182?label=Discord&logo=discord&logoColor=ffffff&labelColor=5865F2&color=5865F2)](https://discord.gg/6tHrP3gaYf) 

Welcome! This repository provides an integration of [Audiokinetic's Wwise audio middleware](https://www.audiokinetic.com/products/wwise/) with the [Godot Engine](https://godotengine.org).

<img src="https://github.com/user-attachments/assets/e0a3dd1c-36de-4a9a-ad14-c6fe18303f90" width="720">

## Features

* [GDExtension](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/what_is_gdextension.html)-based Wwise wrapper library for Windows, macOS, Linux, Android and iOS.
* Multi-platform builds with SCons (Windows, macOS, Linux, iOS) and Gradle/CMake (Android). A Visual Studio 2022 solution is provided for Windows development.
* Wwise profiler connection available in GDExtension debug and profile builds.
* Stream manager uses the default blocking I/O implementation and it can be extended to use custom I/O devices.
* Custom Godot Nodes: AkEvent3D, AkEvent2D, AkBank, AkListener3D, AkListener2D, AkState, AkSwitch, AkEnvironment, AkGeometry, AkRoom, AkPortal, AkEarlyReflections.
* Wwise Types: WwiseAcousticTexture, WwiseAuxBus, WwiseBank, WwiseEvent, WwiseRTPC, WwiseState, WwiseSwitch, WwiseTrigger.
* Support for Auto-Defined SoundBanks.
* Event Callbacks exposed as Signals in AkEvent Nodes.
* Wwise Browser: Query the integrated Wwise project and generate SoundBanks directly in the editor.
* A Wwise IDs generator tool is embedded in the Wwise Browser.
* Per-platform Wwise configurations in the Godot Project Settings.
* In-engine documentation: quick access to reference materials and code snippets in the Godot Editor
* Plugin Support: Plugin detection and export across all platforms, including custom plugin support.

## Getting Started

If you're a game developer who wants to integrate Wwise into your Godot project, go to the [Releases](https://github.com/alessandrofama/wwise-godot-integration/releases) page to download the integration and check out the [Getting Started](https://github.com/alessandrofama/wwise-godot-integration/wiki/Getting-Started-(24.1)) guide.

If you would like to modify the GDExtension code or build it yourself, refer to the [Building the GDExtension Library tutorial](https://github.com/alessandrofama/wwise-godot-integration/wiki/Building-the-GDExtension-Library-(24.1)) for detailed build instructions.

## Documentation

Documentation for the GDExtension interfaces is available on the [wiki](https://github.com/alessandrofama/wwise-godot-integration/wiki).

## Known issues

The known issues are being logged into the [issues page](https://github.com/alessandrofama/wwise-godot-integration/issues) of this repo. Please check this page first, then log a new issue if you haven't found it listed.

## Contributing

Any contribution is welcome. Please send pull-requests with fixes or additions!

## License

Audiokinetic offers a free Wwise license for indie developers. For detailed licensing information, visit https://www.audiokinetic.com/en/pricing/. 

The licensing details for integration files, code, and scripts within this repository can be found [here](https://github.com/alessandrofama/wwise-godot-integration/blob/master/LICENSE).

# Wwise Godot Integration

[![Wwise logo](/wwise-logo.png)](https://www.audiokinetic.com/products/wwise/)
[![Godot Engine logo](/godot-logo.png)](https://godotengine.org)

## Overview

Welcome! This repository provides an integration for Audiokinetic's Wwise audio middleware with the Godot game engine. At the moment, Wwise version 2019.2.1.7250 is supported.

## Features

* Wwise wrapper libraries that can be used in games and apps for Windows, macOS, iOS and Android.
* Multi-platform build with Scons for the Wwise and Waapi wrappers.
* Wwise and Waapi wrappers unit tests using [Gut](https://github.com/bitwes/Gut).
* Wwise and Waapi Godot test projects.
* Wwise profiler connection enabled in debug buids of the GDNative wrapper (Windows and macOS).
* Blocking I/O implementation for loading banks (the Wwise GDNative wrapper can be extended to use custom I/O).
* Custom Godot nodes: AkEvent, AkBank, AkListener, AkState, AkSwitch, AkEnvironment, AkGeometry, AkRoom, AkPortal, AkEarlyReflections.
* Custom Godot Visual Script nodes: AkLoadBank, AkUnloadBank, AkPostEvent, AkSet3DPosition, AkSetState, AkSetSwitch, AkGetRTPCValue, AkSetRTPCValue.
* Custom Godot signals that can be mapped to event callbacks: end of event, audio marker, music sync beat etc.
* Wwise IDs converter from C++ header to GDScript.
* Waapi picker in the Godot editor that allows querying and searching the Wwise project and generating soundbanks from the Wwise authoring app.
* Wwise settings configurable per-platform in the Godot editor.
* Spatial audio API support e.g. can send geometry from Godot to Wwise.
* Integration app to ease the process of integrating Wwise in Godot projects.

## Getting started

If you are a game developer that wants to integrate Wwise into your Godot project, go to the [releases](https://github.com/alessandrofama/wwise-godot-integration/releases) page to download the integration app and files.

If you would like to modify the code of the GDNative wrappers or build them yourself, go to the [Wwise GDNative](https://github.com/alessandrofama/wwise-godot-integration/tree/master/wwise-gdnative) or [Waapi GDNative](https://github.com/alessandrofama/wwise-godot-integration/tree/master/waapi-gdnative) pages for build instructions.

## Documentation

TODO: list documentation pages. Wrapper docs

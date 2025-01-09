#pragma once

#include "core/wwise_platform_info.h"
#include "core/wwise_settings.h"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/string.hpp>
#include <unordered_map>
#include <unordered_set>

using namespace godot;

namespace AkEditorSettings
{
String get_wwise_resource_relative_path() { return "res://Wwise/resources"; }

String get_wwise_project_data_file_name() { return "WwiseProjectData.tres"; }

String get_wwise_project_info_file_name() { return "ProjectInfo.json"; }

String get_wwise_ids_file_name() { return "wwise_ids.gd"; }

String get_project_data_path()
{
	return get_wwise_resource_relative_path().path_join(get_wwise_project_data_file_name());
}

String get_wwise_ids_path() { return get_wwise_resource_relative_path().path_join(get_wwise_ids_file_name()); }

String get_gdextension_config_path() { return "res://addons/Wwise/native/wwise.gdextension"; }

String get_platform_name(String custom_platform_name = String())
{
	WwiseSettings* settings = WwiseSettings::get_singleton();
	String platform_info_res_path;
	Ref<WwisePlatformInfo> platform_info;
	String platform_name;

	auto load_platform_name = [&](const String& setting) -> String
	{
		platform_info_res_path = settings->get_setting(setting);
		if (!platform_info_res_path.is_empty() && FileAccess::file_exists(platform_info_res_path))
		{
			platform_info = ResourceLoader::get_singleton()->load(platform_info_res_path);
		}
		if (platform_info.is_valid())
		{
			return platform_info->get_platform_name();
		}
		return String();
	};

	if (!custom_platform_name.is_empty())
	{
		return custom_platform_name;
	}

#if defined(AK_WIN)
	platform_name = load_platform_name(settings->project_settings.windows_platform_info);
	return platform_name.is_empty() ? "Windows" : platform_name;
#elif defined(AK_MAC_OS_X)
	platform_name = load_platform_name(settings->project_settings.mac_platform_info);
	return platform_name.is_empty() ? "Mac" : platform_name;
#elif defined(AK_LINUX)
	platform_name = load_platform_name(settings->project_settings.linux_platform_info);
	return platform_name.is_empty() ? "Linux" : platform_name;
#else
	UtilityFunctions::push_warning("WwiseGodot: Trying to get platform name for unsupported platform in the Editor.");
	return "Windows";
#endif
}

const String get_library_base_path() { return "res://addons/Wwise/native/lib/"; }

const String get_editor_platform_path()
{
#if defined(AK_WIN)
	return "win64/";
#elif defined(AK_MAC_OS_X)
	return "macos/";
#elif defined(AK_LINUX)
	return "linux/";
#else
	static_assert(false, "WwiseGodot: Unsupported Editor platform!");
#endif
}

const String get_editor_godot_config_path()
{
#if defined(TOOLS_ENABLED)
	return "editor/";
#else
	static_assert(false, "WwiseGodot: Unsupported Editor Godot configuration!");
#endif
}

const String get_editor_wwise_config_path()
{
#if defined(AK_DEBUG)
	return "debug/";
#elif defined(AK_PROFILE)
	return "profile/";
#else
	static_assert(false, "WwiseGodot: Unsupported Editor Wwise configuration!");
#endif
}

const String get_editor_library_path()
{
	return get_library_base_path() + get_editor_platform_path() + get_editor_godot_config_path() +
			get_editor_wwise_config_path();
}

const String get_editor_plugins_path() { return get_editor_library_path().path_join("DSP"); }

enum class PluginID : uint32_t
{
	// Built-in plugins
	Ak3DAudioBedMixer = 0x00BE0003, // Wwise 3D Audio Bed Mixer
	AkCompressor = 0x006C0003, // Wwise Compressor
	AkRouterMixer = 0x00AC0006, // Wwise RouterMixer
	AkChannelRouter = 0x00BF0003, // Wwise Channel Router
	AkDelay = 0x006A0003, // Delay
	AkExpander = 0x006D0003, // Wwise Expander
	AkGain = 0x008B0003, // Gain
	AkMatrixReverb = 0x00730003, // Matrix Reverb
	AkMeter = 0x00810003, // Wwise Meter
	AkParametricEQ = 0x00690003, // Wwise Parametric EQ
	AkPeakLimiter = 0x006E0003, // Wwise Peak Limiter
	AkRoomVerb = 0x00760003, // Wwise RoomVerb
	SineGenerator = 0x00640002, // Sine
	SinkAuxiliary = 0xB40007,
	SinkCommunication = 0xB00007,
	SinkControllerHeadphones = 0xB10007,
	SinkControllerSpeaker = 0xB30007,
	SinkDVRByPass = 0xAF0007,
	SinkNoOutput = 0xB50007,
	SinkSystem = 0xAE0007,
	ToneGenerator = 0x00660002, // Tone Generator
	WwiseSilence = 0x00650002, // Wwise Silence

	// Static or DLL plugins
	AkAudioInput = 0xC80002,
	AkConvolutionReverb = 0x7F0003,
	AkFlanger = 0x7D0003,
	AkGuitarDistortion = 0x7E0003,
	AkHarmonizer = 0x8A0003,
	AkMotionSink = 0x1FB0007,
	AkMotionSource = 0x1990002,
	AkPitchShifter = 0x880003,
	AkRecorder = 0x840003,
	AkReflect = 0xAB0003,
	AkSoundSeedGrain = 0xB70002,
	AkSoundSeedWind = 0x770002,
	AkSoundSeedWoosh = 0x780002,
	AkStereoDelay = 0x870003,
	AkSynthOne = 0x940002,
	AkTimeStretch = 0x820003,
	AkTremolo = 0x830003,
	AuroHeadphone = 0x44C1073,
	CrankcaseAudioREVModelPlayer = 0x1A01052,
	iZHybridReverb = 0x21033,
	iZTrashBoxModeler = 0x71033,
	iZTrashDelay = 0x41033,
	iZTrashDistortion = 0x31033,
	iZTrashDynamics = 0x51033,
	iZTrashFilters = 0x61033,
	iZTrashMultibandDistortion = 0x91033,
	MasteringSuite = 0xBA0003,
	AkImpacterSource = 0xB80002,
	McDSPFutzBox = 0x6E1003,
	McDSPLimiter = 0x671003,
	ResonanceAudioRoomEffect = 0xC81106,
	IgniterLive = 0x5110D2,
	IgniterLiveSynth = 0x5210D2
};

static const std::unordered_set<PluginID> always_skip_plugin_ids = { PluginID::SineGenerator, PluginID::SinkAuxiliary,
	PluginID::SinkCommunication, PluginID::SinkControllerHeadphones, PluginID::SinkControllerSpeaker,
	PluginID::SinkDVRByPass, PluginID::SinkNoOutput, PluginID::SinkSystem, PluginID::ToneGenerator,
	PluginID::WwiseSilence, PluginID::AkAudioInput };

static const std::unordered_set<PluginID> built_in_plugin_ids = { PluginID::Ak3DAudioBedMixer, PluginID::AkCompressor,
	PluginID::AkDelay, PluginID::AkExpander, PluginID::AkFlanger, PluginID::AkGain, PluginID::AkGuitarDistortion,
	PluginID::AkHarmonizer, PluginID::AkMatrixReverb, PluginID::AkMeter, PluginID::AkParametricEQ,
	PluginID::AkPeakLimiter, PluginID::AkPitchShifter, PluginID::AkRecorder, PluginID::AkRoomVerb,
	PluginID::AkStereoDelay, PluginID::AkSynthOne, PluginID::AkTimeStretch, PluginID::AkTremolo };

static const std::unordered_map<PluginID, const char*> ios_header_mapping = { { PluginID::AkChannelRouter,
																					  "AkChannelRouterFXFactory" },
	{ PluginID::AkConvolutionReverb, "AkConvolutionReverbFXFactory" },
	{ PluginID::AkMotionSink, "AkMotionSinkFactory" }, { PluginID::AkMotionSource, "AkMotionSourceSourceFactory" },
	{ PluginID::AkReflect, "AkReflectFXFactory" }, { PluginID::AkImpacterSource, "AkImpacterSourceFactory" },
	{ PluginID::MasteringSuite, "MasteringSuiteFXFactory" },
	{ PluginID::AkSoundSeedGrain, "AkSoundSeedGrainSourceFactory" },
	{ PluginID::AkSoundSeedWind, "AkSoundSeedWindSourceFactory" },
	{ PluginID::AkSoundSeedWoosh, "AkSoundSeedWooshSourceFactory" },
	{ PluginID::AuroHeadphone, "AuroHeadphoneFXFactory" },
	{ PluginID::CrankcaseAudioREVModelPlayer, "CrankcaseAudioREVModelPlayerSourceFactory" },
	{ PluginID::iZHybridReverb, "iZHybridReverbFXFactory" },
	{ PluginID::iZTrashBoxModeler, "iZTrashBoxModelerFXFactory" }, { PluginID::iZTrashDelay, "iZTrashDelayFXFactory" },
	{ PluginID::iZTrashDistortion, "iZTrashDistortionFXFactory" },
	{ PluginID::iZTrashDynamics, "iZTrashDynamicsFXFactory" }, { PluginID::iZTrashFilters, "iZTrashFiltersFXFactory" },
	{ PluginID::iZTrashMultibandDistortion, "iZTrashMultibandDistortionFXFactory" },
	{ PluginID::McDSPFutzBox, "McDSPFutzBoxFXFactory" }, { PluginID::McDSPLimiter, "McDSPLimiterFXFactory" },
	{ PluginID::ResonanceAudioRoomEffect, "ResonanceAudioFXFactory" } };

} //namespace AkEditorSettings
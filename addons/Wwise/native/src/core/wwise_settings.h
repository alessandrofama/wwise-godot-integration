#pragma once

#include <AK/SpatialAudio/Common/AkSpatialAudio.h>
#include <AK/SpatialAudio/Common/AkSpatialAudioTypes.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class WwiseSettings : public RefCounted
{
	GDCLASS(WwiseSettings, RefCounted);

protected:
	static void _bind_methods();

private:
	String WWISE_COMMON_USER_SETTINGS_PATH = "wwise/common_user_settings/";
	String WWISE_COMMON_USER_SETTINGS_MAIN_OUTPUT_PATH = "wwise/common_user_settings/main_output/";
	String WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH = "wwise/common_user_settings/spatial_audio/";
	String WWISE_COMMON_ADVANCED_SETTINGS_PATH = "wwise/common_advanced_settings/";
	String WWISE_COMMUNICATION_SETTINGS_PATH = "wwise/communication_settings/";
	String WWISE_WINDOWS_ADVANCED_SETTINGS_PATH = "wwise/windows_advanced_settings/";
	String WWISE_MACOS_ADVANCED_SETTINGS_PATH = "wwise/macos_advanced_settings/";
	String WWISE_IOS_ADVANCED_SETTINGS_PATH = "wwise/ios_advanced_settings/";
	String WWISE_ANDROID_ADVANCED_SETTINGS_PATH = "wwise/android_advanced_settings/";
	String WWISE_LINUX_ADVANCED_SETTINGS_PATH = "wwise/linux_advanced_settings/";

	void add_wwise_settings();
	void add_setting(const String& name, const Variant& default_value, Variant::Type type,
			PropertyHint hint = PROPERTY_HINT_NONE, const String& hint_string = "", int usage = PROPERTY_USAGE_DEFAULT);

public:
	WwiseSettings();
};
#pragma once

#include "core/utils.h"
#include <AK/Acoustics/Common/AkAcoustics.h>
#include <AK/Acoustics/Common/AkAcousticsTypes.h>
#include <AK/SoundEngine/Common/AkOption.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/templates/hash_set.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class WwiseSettings : public Object
{
	GDCLASS(WwiseSettings, Object);

protected:
	static void _bind_methods();

private:
	enum class Visibility
	{
		DEFAULT,
		ADVANCED,
		HIDDEN
	};

	static WwiseSettings* singleton;
	HashSet<StringName> defined_settings;

	HashMap<StringName, uint32_t> setting_key_map;
	HashMap<StringName, String> setting_type_map;
	HashMap<uint32_t, StringName> wwise_key_to_godot_path;

	void add_setting(const StringName& name, const Variant& default_value, Variant::Type type,
			PropertyHint hint = PROPERTY_HINT_NONE, const StringName& hint_string = "",
			Visibility visibility = Visibility::DEFAULT);
	void remove_undefined_settings();
	void load_options_from_json(const String& p_json_path, const String& p_platform_tag);

public:
	struct IntegrationSettings
	{
		StringName root_output_path = "wwise/integration/root_output_path";
		StringName startup_language = "wwise/integration/startup_language";
		StringName suspend_audio_during_focus_loss = "wwise/integration/suspend_audio_during_focus_loss";
		StringName engine_logging = "wwise/integration/engine_logging";
		StringName log_level = "wwise/integration/log_level";
		StringName waapi_ip = "wwise/integration/waapi_ip";
		StringName waapi_port = "wwise/integration/waapi_port";
	};

	struct ProjectSettings
	{
		StringName windows_platform_info = "wwise/project/windows_platform_info";
		StringName mac_platform_info = "wwise/project/mac_platform_info";
		StringName linux_platform_info = "wwise/project/linux_platform_info";
		StringName ios_platform_info = "wwise/project/ios_platform_info";
		StringName android_platform_info = "wwise/project/android_platform_info";
		StringName web_platform_info = "wwise/project/web_platform_info";
		StringName custom_platform_name = "wwise/project/custom_platform_name";
	};

	IntegrationSettings integration_settings;
	ProjectSettings project_settings;

	String GODOT_WINDOWS_SETTING_POSTFIX = ".windows";
	String GODOT_MAC_OSX_SETTING_POSTFIX = ".macos";
	String GODOT_LINUX_SETTING_POSTFIX = ".linux";
	String GODOT_IOS_SETTING_POSTFIX = ".ios";
	String GODOT_ANDROID_SETTING_POSTFIX = ".android";
	String GODOT_WEB_SETTING_POSTFIX = ".web";

	static WwiseSettings* get_singleton();
	WwiseSettings();
	~WwiseSettings();
	void set_setting(const String& p_name, const Variant& p_value);
	Variant get_setting(const StringName& p_setting, const Variant& p_default = Variant()) const;
	void set_setting_by_wwise_key(uint32_t p_key, const Variant& p_value);
	Variant get_setting_by_wwise_key(uint32_t p_key, const Variant& p_default_value) const;

	void apply_options_to_wwise();
};
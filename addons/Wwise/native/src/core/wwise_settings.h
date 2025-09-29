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
	static WwiseSettings* singleton;

	void add_wwise_settings();
	void add_setting(const StringName& name, const Variant& default_value, Variant::Type type,
			PropertyHint hint = PROPERTY_HINT_NONE, const StringName& hint_string = "");

public:
	struct MainOutputSettings
	{
		StringName audio_device_shareset;
		StringName device_id;
		StringName panning_rule;
		StringName channel_config_type;
		StringName channel_mask;
		StringName number_of_channels;
	};

	struct CommonUserSettings
	{
		StringName root_output_path;
		StringName startup_language;
		StringName load_init_bank_at_startup;
		StringName suspend_audio_during_focus_loss;
		StringName engine_logging;
		StringName maximum_number_of_positioning_paths;
		StringName command_queue_size;
		StringName samples_per_frame;
		StringName game_units_to_meters;
		StringName number_of_refills_in_voice;
		StringName sample_rate;
		StringName streaming_look_ahead_ratio;
		MainOutputSettings main_output;
	};

	struct SpatialAudioSettings
	{
		StringName max_sound_propagation_depth;
		StringName movement_threshold;
		StringName number_of_primary_rays;
		StringName max_reflection_order;
		StringName max_diffraction_order;
		StringName max_diffraction_paths;
		StringName max_global_reflection_paths;
		StringName max_emitter_room_aux_sends;
		StringName diffraction_on_reflections_order;
		StringName max_diffraction_angle_degrees;
		StringName max_path_length;
		StringName cpu_limit_percentage;
		StringName smoothing_constant_ms;
		StringName load_balancing_spread;
		StringName enable_geometric_diffraction_and_transmission;
		StringName calc_emitter_virtual_position;
		StringName transmission_operation;
	};

	struct AdvancedSettings
	{
		StringName io_memory_size;
		StringName target_auto_stream_buffer_length_ms;
		StringName use_stream_cache;
		StringName maximum_pinned_bytes_in_cache;
		StringName enable_game_sync_preparation;
		StringName continuous_playback_look_ahead;
		StringName monitor_queue_pool_size;
		StringName maximum_hardware_timeout_ms;
		StringName debug_out_of_range_check_enabled;
		StringName debug_out_of_range_limit;
	};

	struct CommunicationSettings
	{
		StringName discovery_broadcast_port;
		StringName command_port;
		StringName initialize_system_comms;
		StringName network_name;
		StringName waapi_port;
	};

	struct PlatformSettings
	{
		StringName windows_max_system_audio_objects;
		StringName macos_audio_api;
		StringName ios_audio_api;
		StringName ios_audio_session_category;
		StringName ios_audio_session_category_options;
		StringName ios_audio_session_mode;
		StringName android_audio_api;
		StringName linux_audio_api;
	};

	struct ProjectSettings
	{
		StringName use_soundbank_names;
		StringName create_subfolders_for_generated_files;
		StringName windows_platform_info;
		StringName mac_platform_info;
		StringName linux_platform_info;
		StringName ios_platform_info;
		StringName android_platform_info;
		StringName custom_platform_name;
	};

	CommonUserSettings common_user_settings;
	SpatialAudioSettings spatial_audio_settings;
	AdvancedSettings advanced_settings;
	CommunicationSettings communication_settings;
	PlatformSettings platform_settings;
	ProjectSettings project_settings;

	String GODOT_WINDOWS_SETTING_POSTFIX = ".windows";
	String GODOT_MAC_OSX_SETTING_POSTFIX = ".macos";
	String GODOT_LINUX_SETTING_POSTFIX = ".linux";
	String GODOT_IOS_SETTING_POSTFIX = ".ios";
	String GODOT_ANDROID_SETTING_POSTFIX = ".android";

	static WwiseSettings* get_singleton();
	WwiseSettings();
	~WwiseSettings();
	void set_setting(const String& p_name, const Variant& p_value);
	Variant get_setting(const StringName& p_setting, const Variant& p_default = Variant()) const;
};
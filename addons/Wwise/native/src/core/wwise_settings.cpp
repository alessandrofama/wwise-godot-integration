#include "wwise_settings.h"

void WwiseSettings::_bind_methods() {}

WwiseSettings::WwiseSettings()
{
	add_wwise_settings();

	if (Engine::get_singleton()->is_editor_hint())
	{
		Error error = ProjectSettings::get_singleton()->save();
		if (error)
		{
			UtilityFunctions::push_error(String("WwiseGodot: Encountered error {0} while saving Wwise Project Settings.")
												 .format(Array::make(error)));
		}
		else
		{
			UtilityFunctions::print("WwiseGodot: Saved Project Settings.");
		}
	}
}

void WwiseSettings::add_wwise_settings()
{
	// Common User Settings
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "base_path", String("res://GeneratedSoundBanks"),
			Variant::Type::STRING, PROPERTY_HINT_DIR, "res://GeneratedSoundBanks");
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "startup_language", String("English(US)"), Variant::Type::STRING);
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "load_init_bank_at_startup", true, Variant::Type::BOOL);
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "suspend_on_focus_loss", false, Variant::Type::BOOL);
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "use_soundbank_names", true, Variant::Type::BOOL);
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "engine_logging", false, Variant::Type::BOOL);
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "maximum_number_of_positioning_paths", 255, Variant::Type::INT);
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "command_queue_size", 262144, Variant::Type::INT);
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "samples_per_frame", 2, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"256, 512, 1024, 2048");
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "game_units_to_meters", 1.0f, Variant::Type::FLOAT);
	add_setting(WWISE_COMMON_USER_SETTINGS_MAIN_OUTPUT_PATH + "audio_device_shareset", "System", Variant::Type::STRING,
			PROPERTY_HINT_NONE, "");
	add_setting(WWISE_COMMON_USER_SETTINGS_MAIN_OUTPUT_PATH + "device_id", 0, Variant::Type::INT);
	add_setting(WWISE_COMMON_USER_SETTINGS_MAIN_OUTPUT_PATH + "panning_rule", 0, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"Speakers, Headphones");
	add_setting(WWISE_COMMON_USER_SETTINGS_MAIN_OUTPUT_PATH + "channel_config/channel_config_type", 0,
			Variant::Type::INT, PROPERTY_HINT_ENUM, "Annonymous, Standard, Ambisonics");
	// note(alex): this is not implemented:
	add_setting(WWISE_COMMON_USER_SETTINGS_MAIN_OUTPUT_PATH + "channel_config/channel_mask", 0, Variant::Type::INT,
			PROPERTY_HINT_ENUM, "NONE, Everything, SETUP_ALL_SPEAKERS");
	add_setting(
			WWISE_COMMON_USER_SETTINGS_MAIN_OUTPUT_PATH + "channel_config/number_of_channels", 0, Variant::Type::INT);
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "streaming_look_ahead_ratio", 1.0f, Variant::Type::FLOAT,
			PROPERTY_HINT_RANGE, "0.0, 1.0");
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "sample_rate", 4, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"16000, 24000, 32000, 44100, 48000");
	add_setting(WWISE_COMMON_USER_SETTINGS_PATH + "number_of_refills_in_voice", 1, Variant::Type::INT,
			PROPERTY_HINT_ENUM, "2, 4");

	// Spatial Audio Settings
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "max_sound_propagation_depth",
			AK_MAX_SOUND_PROPAGATION_DEPTH, Variant::Type::FLOAT, PROPERTY_HINT_RANGE, "0.0,8.0");
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "movement_threshold", 0.25f, Variant::Type::FLOAT,
			PROPERTY_HINT_NONE, "");
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "number_of_primary_rays", 35, Variant::Type::INT,
			PROPERTY_HINT_NONE, "");
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "max_reflection_order", 2, Variant::Type::INT,
			PROPERTY_HINT_RANGE, "1,4");
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "max_diffraction_order", 4, Variant::Type::INT,
			PROPERTY_HINT_RANGE, "1,8");
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "max_diffraction_paths", 8, Variant::Type::INT,
			PROPERTY_HINT_NONE, "");
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "max_global_reflection_paths", 0, Variant::Type::INT,
			PROPERTY_HINT_NONE, "");
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "max_emitter_room_aux_sends", 3, Variant::Type::INT);
	add_setting(
			WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "diffraction_on_reflections_order", 2, Variant::Type::INT);
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "max_diffraction_angle_degrees", 180.f,
			Variant::Type::FLOAT, PROPERTY_HINT_NONE, "");
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "max_path_length", 10000.0f, Variant::Type::FLOAT,
			PROPERTY_HINT_NONE, "");
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "cpu_limit_percentage", 0.0f, Variant::Type::FLOAT,
			PROPERTY_HINT_RANGE, "0.0,100.0");
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "smoothing_constant_ms", 0.0f, Variant::Type::FLOAT,
			PROPERTY_HINT_NONE, "");
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "load_balancing_spread", 1, Variant::Type::INT);
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "enable_geometric_diffraction_and_transmission", true,
			Variant::Type::BOOL);
	add_setting(
			WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "calc_emitter_virtual_position", true, Variant::Type::BOOL);
	add_setting(WWISE_COMMON_USER_SETTINGS_SPATIAL_AUDIO_PATH + "transmission_operation",
			AkTransmissionOperation_Default, Variant::Type::INT, PROPERTY_HINT_ENUM, "Add, Multiply, Max");

	// Common Advanced Settings
	add_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "IO_memory_size", 2097152, Variant::Type::INT, PROPERTY_HINT_NONE,
			"");
	add_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "target_auto_stream_buffer_length_ms", 380, Variant::Type::INT);
	add_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "use_stream_cache", false, Variant::Type::BOOL);
	add_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "maximum_pinned_bytes_in_cache",
			static_cast<uint64_t>(4294967295), Variant::Type::INT);
	add_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "enable_game_sync_preparation", false, Variant::Type::BOOL);
	add_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "continuous_playback_look_ahead", 1, Variant::Type::INT);
	add_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "monitor_queue_pool_size", 65536, Variant::Type::INT);
	add_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "maximum_hardware_timeout_ms", 1000, Variant::Type::INT);
	add_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "debug_out_of_range_check_enabled", false, Variant::Type::BOOL);
	add_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "debug_out_of_range_limit", 16.0f, Variant::Type::FLOAT);

	// Communication Settings
	add_setting(WWISE_COMMUNICATION_SETTINGS_PATH + "discovery_broadcast_port", 24024, Variant::Type::INT);
	add_setting(WWISE_COMMUNICATION_SETTINGS_PATH + "command_port", 0, Variant::Type::INT);
	add_setting(WWISE_COMMUNICATION_SETTINGS_PATH + "initialize_system_comms", true, Variant::Type::BOOL);
	add_setting(WWISE_COMMUNICATION_SETTINGS_PATH + "network_name", "", Variant::Type::STRING);
	add_setting(WWISE_COMMUNICATION_SETTINGS_PATH + "waapi_port", 8080, Variant::Type::INT);

	// Windows Advanced Settings
	add_setting(WWISE_WINDOWS_ADVANCED_SETTINGS_PATH + "max_system_audio_objects", 128, Variant::Type::INT);

	// macOS advanced settings
	add_setting(WWISE_MACOS_ADVANCED_SETTINGS_PATH + "audio_API", 3, Variant::Type::INT, PROPERTY_HINT_FLAGS,
			"AVAudioEngine,AudioUnit");

	// iOS advanced settings
	add_setting(WWISE_IOS_ADVANCED_SETTINGS_PATH + "audio_API", 3, Variant::Type::INT, PROPERTY_HINT_FLAGS,
			"AVAudioEngine,AudioUnit");
	add_setting(WWISE_IOS_ADVANCED_SETTINGS_PATH + "audio_session_category", 0, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"Ambient, Solo Ambient, Play And Record, Playback");
	add_setting(WWISE_IOS_ADVANCED_SETTINGS_PATH + "audio_session_category_options", 13, Variant::Type::INT,
			PROPERTY_HINT_FLAGS, "Mix with others, Duck others, Allow bluetooth, Default to speaker");
	add_setting(WWISE_IOS_ADVANCED_SETTINGS_PATH + "audio_session_mode", 0, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"Default, Voice Chat, Game Chat, Video Recording, Measurement, Movie Playback, Video Chat");

	// android advanced settings
	add_setting(WWISE_ANDROID_ADVANCED_SETTINGS_PATH + "audio_API", 3, Variant::Type::INT, PROPERTY_HINT_FLAGS,
			"AAudio, OPENSL_ES");
	add_setting(WWISE_ANDROID_ADVANCED_SETTINGS_PATH + "round_frame_size_to_hw_size", true, Variant::Type::BOOL);

	// linux advanced settings
	add_setting(WWISE_LINUX_ADVANCED_SETTINGS_PATH + "audio_API", 3, Variant::Type::INT, PROPERTY_HINT_FLAGS,
			"PulseAudio, ALSA");
}

void WwiseSettings::add_setting(const String& name, const Variant& default_value, Variant::Type type, PropertyHint hint,
		const String& hint_string, int usage)
{
	Dictionary setting;
	setting["name"] = name;
	setting["type"] = type;
	setting["hint"] = hint;
	setting["hint_string"] = hint_string;
	setting["usage"] = usage;

	if (ProjectSettings::get_singleton()->has_setting(name))
	{
		ProjectSettings::get_singleton()->add_property_info(setting);
		return;
	}

	ProjectSettings::get_singleton()->set_setting(name, default_value);
	ProjectSettings::get_singleton()->add_property_info(setting);
}

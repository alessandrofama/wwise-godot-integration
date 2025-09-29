#include "wwise_settings.h"

WwiseSettings* WwiseSettings::singleton = nullptr;

void WwiseSettings::_bind_methods() {}

WwiseSettings* WwiseSettings::get_singleton() { return singleton; }

WwiseSettings::WwiseSettings()
{
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;

	// Common User Settings
	common_user_settings.root_output_path = "wwise/common_user_settings/root_output_path";
	common_user_settings.startup_language = "wwise/common_user_settings/startup_language";
	common_user_settings.suspend_audio_during_focus_loss = "wwise/common_user_settings/suspend_audio_during_focus_loss";
	common_user_settings.engine_logging = "wwise/common_user_settings/engine_logging";
	common_user_settings.maximum_number_of_positioning_paths =
			"wwise/common_user_settings/maximum_number_of_positioning_paths";
	common_user_settings.command_queue_size = "wwise/common_user_settings/command_queue_size";
	common_user_settings.samples_per_frame = "wwise/common_user_settings/samples_per_frame";
	common_user_settings.game_units_to_meters = "wwise/common_user_settings/game_units_to_meters";
	common_user_settings.number_of_refills_in_voice = "wwise/common_user_settings/number_of_refills_in_voice";
	common_user_settings.sample_rate = "wwise/common_user_settings/sample_rate";
	common_user_settings.streaming_look_ahead_ratio = "wwise/common_user_settings/streaming_look_ahead_ratio";

	// Main Output Settings
	common_user_settings.main_output.audio_device_shareset =
			"wwise/common_user_settings/main_output/audio_device_shareset";
	common_user_settings.main_output.device_id = "wwise/common_user_settings/main_output/device_id";
	common_user_settings.main_output.panning_rule = "wwise/common_user_settings/main_output/panning_rule";
	common_user_settings.main_output.channel_config_type = "wwise/common_user_settings/main_output/channel_config_type";
	common_user_settings.main_output.number_of_channels = "wwise/common_user_settings/main_output/number_of_channels";

	// Spatial Audio Settings
	spatial_audio_settings.max_sound_propagation_depth =
			"wwise/common_user_settings/spatial_audio/max_sound_propagation_depth";
	spatial_audio_settings.movement_threshold = "wwise/common_user_settings/spatial_audio/movement_threshold";
	spatial_audio_settings.number_of_primary_rays = "wwise/common_user_settings/spatial_audio/number_of_primary_rays";
	spatial_audio_settings.max_reflection_order = "wwise/common_user_settings/spatial_audio/max_reflection_order";
	spatial_audio_settings.max_diffraction_order = "wwise/common_user_settings/spatial_audio/max_diffraction_order";
	spatial_audio_settings.max_diffraction_paths = "wwise/common_user_settings/spatial_audio/max_diffraction_paths";
	spatial_audio_settings.max_global_reflection_paths =
			"wwise/common_user_settings/spatial_audio/max_global_reflection_paths";
	spatial_audio_settings.max_emitter_room_aux_sends =
			"wwise/common_user_settings/spatial_audio/max_emitter_room_aux_sends";
	spatial_audio_settings.diffraction_on_reflections_order =
			"wwise/common_user_settings/spatial_audio/diffraction_on_reflections_order";
	spatial_audio_settings.max_diffraction_angle_degrees =
			"wwise/common_user_settings/spatial_audio/max_diffraction_angle_degrees";
	spatial_audio_settings.max_path_length = "wwise/common_user_settings/spatial_audio/max_path_length";
	spatial_audio_settings.cpu_limit_percentage = "wwise/common_user_settings/spatial_audio/cpu_limit_percentage";
	spatial_audio_settings.smoothing_constant_ms = "wwise/common_user_settings/spatial_audio/smoothing_constant_ms";
	spatial_audio_settings.load_balancing_spread = "wwise/common_user_settings/spatial_audio/load_balancing_spread";
	spatial_audio_settings.enable_geometric_diffraction_and_transmission =
			"wwise/common_user_settings/spatial_audio/enable_geometric_diffraction_and_transmission";
	spatial_audio_settings.calc_emitter_virtual_position =
			"wwise/common_user_settings/spatial_audio/calc_emitter_virtual_position";
	spatial_audio_settings.transmission_operation = "wwise/common_user_settings/spatial_audio/transmission_operation";

	// Advanced Settings
	advanced_settings.io_memory_size = "wwise/common_advanced_settings/IO_memory_size";
	advanced_settings.target_auto_stream_buffer_length_ms =
			"wwise/common_advanced_settings/target_auto_stream_buffer_length_ms";
	advanced_settings.use_stream_cache = "wwise/common_advanced_settings/use_stream_cache";
	advanced_settings.maximum_pinned_bytes_in_cache = "wwise/common_advanced_settings/maximum_pinned_bytes_in_cache";
	advanced_settings.enable_game_sync_preparation = "wwise/common_advanced_settings/enable_game_sync_preparation";
	advanced_settings.continuous_playback_look_ahead = "wwise/common_advanced_settings/continuous_playback_look_ahead";
	advanced_settings.monitor_queue_pool_size = "wwise/common_advanced_settings/monitor_queue_pool_size";
	advanced_settings.maximum_hardware_timeout_ms = "wwise/common_advanced_settings/maximum_hardware_timeout_ms";
	advanced_settings.debug_out_of_range_check_enabled =
			"wwise/common_advanced_settings/debug_out_of_range_check_enabled";
	advanced_settings.debug_out_of_range_limit = "wwise/common_advanced_settings/debug_out_of_range_limit";

	// Communication Settings
	communication_settings.discovery_broadcast_port = "wwise/communication_settings/discovery_broadcast_port";
	communication_settings.command_port = "wwise/communication_settings/command_port";
	communication_settings.initialize_system_comms = "wwise/communication_settings/initialize_system_comms";
	communication_settings.network_name = "wwise/communication_settings/network_name";
	communication_settings.waapi_port = "wwise/communication_settings/waapi_port";

	// Platform Settings
	platform_settings.windows_max_system_audio_objects = "wwise/windows_advanced_settings/max_system_audio_objects";
	platform_settings.macos_audio_api = "wwise/macos_advanced_settings/audio_API";
	platform_settings.ios_audio_api = "wwise/ios_advanced_settings/audio_API";
	platform_settings.ios_audio_session_category = "wwise/ios_advanced_settings/audio_session_category";
	platform_settings.ios_audio_session_category_options = "wwise/ios_advanced_settings/audio_session_category_options";
	platform_settings.ios_audio_session_mode = "wwise/ios_advanced_settings/audio_session_mode";
	platform_settings.android_audio_api = "wwise/android_advanced_settings/audio_API";
	platform_settings.android_round_frame_size_to_hw_size =
			"wwise/android_advanced_settings/round_frame_size_to_hw_size";
	platform_settings.linux_audio_api = "wwise/linux_advanced_settings/audio_API";

	// Project Settings
	project_settings.use_soundbank_names = "wwise/project_settings/use_soundbank_names";
	project_settings.create_subfolders_for_generated_files =
			"wwise/project_settings/create_subfolders_for_generated_files";
	project_settings.windows_platform_info = "wwise/project_settings/windows_platform_info";
	project_settings.mac_platform_info = "wwise/project_settings/mac_platform_info";
	project_settings.linux_platform_info = "wwise/project_settings/linux_platform_info";
	project_settings.ios_platform_info = "wwise/project_settings/ios_platform_info";
	project_settings.android_platform_info = "wwise/project_settings/android_platform_info";
	project_settings.custom_platform_name = "wwise/project_settings/custom_platform_name";

	add_wwise_settings();

	if (Engine::get_singleton()->is_editor_hint())
	{
		godot::Error error = godot::ProjectSettings::get_singleton()->save();
		if (error)
		{
			UtilityFunctions::push_error(
					StringName("WwiseGodot: Encountered error {0} while saving Wwise Project Settings.")
							.format(Array::make(error)));
		}
		else
		{
			UtilityFunctions::print("WwiseGodot: Saved Project Settings.");
		}
	}
}

WwiseSettings::~WwiseSettings()
{
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}

void WwiseSettings::set_setting(const String& p_name, const Variant& p_value)
{
	godot::ProjectSettings* settings = godot::ProjectSettings::get_singleton();

	if (settings && settings->has_setting(p_name))
	{
		settings->set_setting(p_name, p_value);
	}
	else
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: Trying to set non-existing setting: %s.", p_name));
	}
}

Variant WwiseSettings::get_setting(const StringName& p_setting, const Variant& p_default) const
{
	String platform_setting = p_setting;

#ifdef AK_WIN
	platform_setting += GODOT_WINDOWS_SETTING_POSTFIX;
#elif defined(AK_MAC_OS_X)
	platform_setting += GODOT_MAC_OSX_SETTING_POSTFIX;
#elif defined(AK_IOS)
	platform_setting += GODOT_IOS_SETTING_POSTFIX;
#elif defined(AK_ANDROID)
	platform_setting += GODOT_ANDROID_SETTING_POSTFIX;
#elif defined(AK_LINUX)
	platform_setting += GODOT_LINUX_SETTING_POSTFIX;
#else
#error "Platform not supported"
#endif

	godot::ProjectSettings* settings = godot::ProjectSettings::get_singleton();

	// Try to get the platform-specific setting, if it exists.
	if (settings && settings->has_setting(platform_setting))
	{
		return settings->get(platform_setting);
	}

	// Otherwise, try to get the default platform-agnostic setting.
	if (settings && settings->has_setting(p_setting))
	{
		return settings->get(p_setting);
	}
	else
	{
		UtilityFunctions::push_warning(
				vformat("WwiseGodot: Setting %s not found. Returning default value.", p_setting));
		return p_default;
	}
}

void WwiseSettings::add_wwise_settings()
{
	// Common User Settings
	add_setting(common_user_settings.root_output_path, "", Variant::Type::STRING, PROPERTY_HINT_DIR, "");
	add_setting(common_user_settings.startup_language, "English(US)", Variant::Type::STRING);
	add_setting(common_user_settings.suspend_audio_during_focus_loss, false, Variant::Type::BOOL);
	add_setting(common_user_settings.engine_logging, true, Variant::Type::BOOL);
	add_setting(common_user_settings.maximum_number_of_positioning_paths, 255, Variant::Type::INT);
	add_setting(common_user_settings.command_queue_size, 262144, Variant::Type::INT);
	add_setting(
			common_user_settings.samples_per_frame, 2, Variant::Type::INT, PROPERTY_HINT_ENUM, "256, 512, 1024, 2048");
	add_setting(common_user_settings.game_units_to_meters, 1.0f, Variant::Type::FLOAT);
	add_setting(common_user_settings.number_of_refills_in_voice, 1, Variant::Type::INT, PROPERTY_HINT_ENUM, "2, 4");
	add_setting(common_user_settings.sample_rate, 4, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"16000, 24000, 32000, 44100, 48000");
	add_setting(common_user_settings.streaming_look_ahead_ratio, 1.0f, Variant::Type::FLOAT, PROPERTY_HINT_RANGE,
			"0.0, 1.0");

	// Spatial Audio Settings
	add_setting(spatial_audio_settings.max_sound_propagation_depth, AK_MAX_SOUND_PROPAGATION_DEPTH,
			Variant::Type::FLOAT, PROPERTY_HINT_RANGE, "0.0,8.0");
	add_setting(spatial_audio_settings.movement_threshold, 0.25f, Variant::Type::FLOAT);
	add_setting(spatial_audio_settings.number_of_primary_rays, 35, Variant::Type::INT);
	add_setting(spatial_audio_settings.max_reflection_order, 2, Variant::Type::INT, PROPERTY_HINT_RANGE, "1,4");
	add_setting(spatial_audio_settings.max_diffraction_order, 4, Variant::Type::INT, PROPERTY_HINT_RANGE, "1,8");
	add_setting(spatial_audio_settings.max_diffraction_paths, 8, Variant::Type::INT);
	add_setting(spatial_audio_settings.max_global_reflection_paths, 0, Variant::Type::INT);
	add_setting(spatial_audio_settings.max_emitter_room_aux_sends, 3, Variant::Type::INT);
	add_setting(spatial_audio_settings.diffraction_on_reflections_order, 2, Variant::Type::INT);
	add_setting(spatial_audio_settings.max_diffraction_angle_degrees, 180.f, Variant::Type::FLOAT);
	add_setting(spatial_audio_settings.max_path_length, 10000.0f, Variant::Type::FLOAT);
	add_setting(
			spatial_audio_settings.cpu_limit_percentage, 0.0f, Variant::Type::FLOAT, PROPERTY_HINT_RANGE, "0.0,100.0");
	add_setting(spatial_audio_settings.smoothing_constant_ms, 0.0f, Variant::Type::FLOAT);
	add_setting(spatial_audio_settings.load_balancing_spread, 1, Variant::Type::INT);
	add_setting(spatial_audio_settings.enable_geometric_diffraction_and_transmission, true, Variant::Type::BOOL);
	add_setting(spatial_audio_settings.calc_emitter_virtual_position, true, Variant::Type::BOOL);
	add_setting(spatial_audio_settings.transmission_operation, AkTransmissionOperation::AkTransmissionOperation_Default,
			Variant::Type::INT, PROPERTY_HINT_ENUM, "Add, Multiply, Max");

	// Main Output Settings
	add_setting(common_user_settings.main_output.audio_device_shareset, "System", Variant::Type::STRING,
			PROPERTY_HINT_NONE, "");
	add_setting(common_user_settings.main_output.device_id, 0, Variant::Type::INT);
	add_setting(common_user_settings.main_output.panning_rule, 0, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"Speakers, Headphones");
	add_setting(common_user_settings.main_output.channel_config_type, 0, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"Anonymous, Standard, Ambisonics");
	add_setting(common_user_settings.main_output.number_of_channels, 0, Variant::Type::INT);

	// Advanced Settings
	add_setting(advanced_settings.io_memory_size, 2097152, Variant::Type::INT);
	add_setting(advanced_settings.target_auto_stream_buffer_length_ms, 380, Variant::Type::INT);
	add_setting(advanced_settings.use_stream_cache, false, Variant::Type::BOOL);
	add_setting(advanced_settings.maximum_pinned_bytes_in_cache, static_cast<uint64_t>(4294967295), Variant::Type::INT);
	add_setting(advanced_settings.enable_game_sync_preparation, false, Variant::Type::BOOL);
	add_setting(advanced_settings.continuous_playback_look_ahead, 1, Variant::Type::INT);
	add_setting(advanced_settings.monitor_queue_pool_size, 1048576, Variant::Type::INT);
	add_setting(advanced_settings.maximum_hardware_timeout_ms, 1000, Variant::Type::INT);
	add_setting(advanced_settings.debug_out_of_range_check_enabled, false, Variant::Type::BOOL);
	add_setting(advanced_settings.debug_out_of_range_limit, 16.0f, Variant::Type::FLOAT);

	// Communication Settings
	add_setting(communication_settings.discovery_broadcast_port, 24024, Variant::Type::INT);
	add_setting(communication_settings.command_port, 0, Variant::Type::INT);
	add_setting(communication_settings.initialize_system_comms, true, Variant::Type::BOOL);
	add_setting(communication_settings.network_name, "", Variant::Type::STRING);
	add_setting(communication_settings.waapi_port, 8080, Variant::Type::INT);

	// Platform Settings
	add_setting(platform_settings.windows_max_system_audio_objects, 128, Variant::Type::INT);
	add_setting(
			platform_settings.macos_audio_api, 3, Variant::Type::INT, PROPERTY_HINT_FLAGS, "AVAudioEngine,AudioUnit");
	add_setting(platform_settings.ios_audio_api, 3, Variant::Type::INT, PROPERTY_HINT_FLAGS, "AVAudioEngine,AudioUnit");
	add_setting(platform_settings.ios_audio_session_category, 0, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"Ambient, Solo Ambient, Play And Record, Playback");
	add_setting(platform_settings.ios_audio_session_category_options, 13, Variant::Type::INT, PROPERTY_HINT_FLAGS,
			"Mix with others, Duck others, Allow bluetooth, Default to speaker");
	add_setting(platform_settings.ios_audio_session_mode, 0, Variant::Type::INT, PROPERTY_HINT_ENUM,
			"Default, Voice Chat, Game Chat, Video Recording, Measurement, Movie Playback, Video Chat");
	add_setting(platform_settings.android_audio_api, 3, Variant::Type::INT, PROPERTY_HINT_FLAGS, "AAudio, OPENSL_ES");
	add_setting(platform_settings.android_round_frame_size_to_hw_size, true, Variant::Type::BOOL);
	add_setting(platform_settings.linux_audio_api, 3, Variant::Type::INT, PROPERTY_HINT_FLAGS, "PulseAudio, ALSA");

	// Project Setting
	add_setting(project_settings.use_soundbank_names, true, Variant::Type::BOOL, PROPERTY_HINT_NONE, "");
	add_setting(
			project_settings.create_subfolders_for_generated_files, false, Variant::Type::BOOL, PROPERTY_HINT_NONE, "");
	add_setting(project_settings.windows_platform_info, "", Variant::Type::STRING, PROPERTY_HINT_NONE, "");
	add_setting(project_settings.mac_platform_info, "", Variant::Type::STRING, PROPERTY_HINT_NONE, "");
	add_setting(project_settings.linux_platform_info, "", Variant::Type::STRING, PROPERTY_HINT_NONE, "");
	add_setting(project_settings.ios_platform_info, "", Variant::Type::STRING, PROPERTY_HINT_NONE, "");
	add_setting(project_settings.android_platform_info, "", Variant::Type::STRING, PROPERTY_HINT_NONE, "");
	add_setting(project_settings.custom_platform_name, "", Variant::Type::STRING, PROPERTY_HINT_NONE, "");
}

void WwiseSettings::add_setting(const StringName& name, const Variant& default_value, Variant::Type type,
		PropertyHint hint, const StringName& hint_string)
{
	Dictionary setting;
	setting["name"] = name;
	setting["type"] = type;
	setting["hint"] = hint;
	setting["hint_string"] = hint_string;

	godot::ProjectSettings* project_settings = godot::ProjectSettings::get_singleton();

	if (project_settings->has_setting(name))
	{
		project_settings->add_property_info(setting);
		project_settings->set_as_basic(name, true);
		project_settings->set_initial_value(name, default_value);
		return;
	}

	project_settings->set_setting(name, default_value);
	project_settings->add_property_info(setting);
	project_settings->set_as_basic(name, true);
	project_settings->set_initial_value(name, default_value);
}

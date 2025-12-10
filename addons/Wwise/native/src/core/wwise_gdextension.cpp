#include "wwise_gdextension.h"

#if defined(AK_ANDROID)
#include "platform/android/jni_support.cpp"
#endif

#include <AK/Plugin/AllPluginsFactories.h>

#include "scene/ak_game_obj.h"
#include "scene/ak_game_obj_2d.h"
#include "scene/ak_game_obj_3d.h"

Wwise* Wwise::singleton = nullptr;
CAkLock Wwise::callback_data_lock;
CAkLock g_localOutputLock;
HashSet<AkGameObjectID> Wwise::registered_game_objects;

#if defined(AK_ENABLE_ASSERTS)
void WwiseAssertHook(const char* in_pszExpression, const char* in_psz_filename, int in_lineNumber)
{
	String msg = vformat("WwiseGodot: AKASSERT: %s, %s:%d", in_pszExpression, in_psz_filename, in_lineNumber);
	UtilityFunctions::push_warning(msg);
	AKPLATFORM::OutputDebugMsg(msg.utf8().get_data());
}
#endif

void LocalOutput(AK::Monitor::ErrorCode in_eErrorCode, const AkOSChar* in_pszError,
		AK::Monitor::ErrorLevel in_eErrorLevel, AkPlayingID in_playingID, AkGameObjectID in_gameObjID)
{
	AkAutoLock<CAkLock> autoLock(g_localOutputLock);

	if (in_eErrorCode != AK::Monitor::ErrorCode::ErrorCode_NoError)
	{
		UtilityFunctions::push_error(vformat("WwiseGodot: %s", in_pszError));
	}
}

Wwise::Wwise()
{
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

Wwise::~Wwise()
{
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}

Wwise* Wwise::get_singleton() { return singleton; }

void Wwise::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("init"), &Wwise::init);
	ClassDB::bind_method(D_METHOD("render_audio"), &Wwise::render_audio);
	ClassDB::bind_method(D_METHOD("shutdown"), &Wwise::shutdown);
	ClassDB::bind_method(D_METHOD("set_banks_path", "root_output_path"), &Wwise::set_banks_path);
	ClassDB::bind_method(D_METHOD("set_current_language", "language"), &Wwise::set_current_language);
	ClassDB::bind_method(D_METHOD("load_bank", "bank_name"), &Wwise::load_bank);
	ClassDB::bind_method(D_METHOD("load_bank_id", "bank_id"), &Wwise::load_bank_id);
	ClassDB::bind_method(D_METHOD("load_bank_async", "bank_name", "cookie"), &Wwise::load_bank_async);
	ClassDB::bind_method(D_METHOD("load_bank_async_id", "bank_id", "cookie"), &Wwise::load_bank_async_id);
	ClassDB::bind_method(D_METHOD("unload_bank", "bank_name"), &Wwise::unload_bank);
	ClassDB::bind_method(D_METHOD("unload_bank_id", "bank_id"), &Wwise::unload_bank_id);
	ClassDB::bind_method(D_METHOD("unload_bank_async", "bank_name", "cookie"), &Wwise::unload_bank_async);
	ClassDB::bind_method(D_METHOD("unload_bank_async_id", "bank_id", "cookie"), &Wwise::unload_bank_async_id);
	ClassDB::bind_method(D_METHOD("register_listener", "game_object"), &Wwise::register_listener);
	ClassDB::bind_method(D_METHOD("register_game_obj", "game_object", "name"), &Wwise::register_game_obj);
	ClassDB::bind_method(D_METHOD("unregister_game_obj", "game_object"), &Wwise::unregister_game_obj);
	ClassDB::bind_method(D_METHOD("add_listener", "emitter", "listener"), &Wwise::add_listener);
	ClassDB::bind_method(D_METHOD("remove_listener", "emitter", "listener"), &Wwise::remove_listener);
	ClassDB::bind_method(D_METHOD("add_default_listener", "game_object"), &Wwise::add_default_listener);
	ClassDB::bind_method(D_METHOD("remove_default_listener", "game_object"), &Wwise::remove_default_listener);
	ClassDB::bind_method(D_METHOD("set_listeners", "emitter", "listeners"), &Wwise::set_listeners);
	ClassDB::bind_method(
			D_METHOD("set_distance_probe", "listener_game_object", "probe_game_object"), &Wwise::set_distance_probe);
	ClassDB::bind_method(D_METHOD("reset_distance_probe", "listener_game_object"), &Wwise::reset_distance_probe);
	ClassDB::bind_method(D_METHOD("set_random_seed", "seed"), &Wwise::set_random_seed);
	ClassDB::bind_method(D_METHOD("set_3d_position", "game_object", "transform_3d"), &Wwise::set_3d_position);
	ClassDB::bind_method(
			D_METHOD("set_2d_position", "game_object", "transform_2d", "z_depth"), &Wwise::set_2d_position);
	ClassDB::bind_method(
			D_METHOD("set_multiple_positions_3d", "game_object", "positions", "num_positions", "multi_position_type"),
			&Wwise::set_multiple_positions_3d);
	ClassDB::bind_method(D_METHOD("set_multiple_positions_2d", "game_object", "positions", "z_depths", "num_positions",
								 "multi_position_type"),
			&Wwise::set_multiple_positions_2d);
	ClassDB::bind_method(D_METHOD("set_game_object_radius", "game_object", "outer_radius", "inner_radius"),
			&Wwise::set_game_object_radius);
	ClassDB::bind_method(D_METHOD("post_event", "event_name", "game_object"), &Wwise::post_event);
	ClassDB::bind_method(D_METHOD("post_event_callback", "event_name", "flags", "game_object", "cookie"),
			&Wwise::post_event_callback);
	ClassDB::bind_method(D_METHOD("post_event_id", "event_id", "game_object"), &Wwise::post_event_id);
	ClassDB::bind_method(D_METHOD("post_event_id_callback", "event_id", "flags", "game_object", "cookie"),
			&Wwise::post_event_id_callback);
	ClassDB::bind_method(D_METHOD("post_midi_on_event_id", "event_id", "game_object", "midi_posts", "absolute_offsets"),
			&Wwise::post_midi_on_event_id);
	ClassDB::bind_method(D_METHOD("stop_event", "playing_id", "fade_time", "interpolation"), &Wwise::stop_event);
	ClassDB::bind_method(D_METHOD("stop_midi_on_event_id", "event_id", "game_object"), &Wwise::stop_midi_on_event_id);
	ClassDB::bind_method(D_METHOD("execute_action_on_event_id", "event_id", "action_type", "game_object",
								 "transition_duration", "fade_curve", "playing_id"),
			&Wwise::execute_action_on_event_id, DEFVAL(0), DEFVAL(AkUtils::AkCurveInterpolation::AK_CURVE_LINEAR),
			DEFVAL(AK_INVALID_PLAYING_ID));
	ClassDB::bind_method(D_METHOD("set_switch", "switch_group", "switch_value", "game_object"), &Wwise::set_switch);
	ClassDB::bind_method(
			D_METHOD("set_switch_id", "switch_group_id", "switch_value_id", "game_object"), &Wwise::set_switch_id);
	ClassDB::bind_method(D_METHOD("set_state", "state_group", "state_value"), &Wwise::set_state);
	ClassDB::bind_method(D_METHOD("set_state_id", "state_group_id", "state_value_id"), &Wwise::set_state_id);
	ClassDB::bind_method(D_METHOD("get_rtpc_value", "name", "game_object"), &Wwise::get_rtpc_value);
	ClassDB::bind_method(D_METHOD("get_rtpc_value_id", "id", "game_object"), &Wwise::get_rtpc_value_id);
	ClassDB::bind_method(D_METHOD("set_rtpc_value", "name", "value", "game_object"), &Wwise::set_rtpc_value);
	ClassDB::bind_method(D_METHOD("set_rtpc_value_id", "id", "value", "game_object"), &Wwise::set_rtpc_value_id);
	ClassDB::bind_method(D_METHOD("post_trigger", "name", "game_object"), &Wwise::post_trigger);
	ClassDB::bind_method(D_METHOD("post_trigger_id", "id", "game_object"), &Wwise::post_trigger_id);
	ClassDB::bind_method(
			D_METHOD("post_external_source", "event_name", "game_object", "source_object_name", "filename", "id_codec"),
			&Wwise::post_external_source);
	ClassDB::bind_method(
			D_METHOD("post_external_source_id", "event_id", "game_object", "source_object_id", "filename", "id_codec"),
			&Wwise::post_external_source_id);
	ClassDB::bind_method(D_METHOD("post_external_sources", "event_id", "game_object", "external_source_info"),
			&Wwise::post_external_sources);
	ClassDB::bind_method(
			D_METHOD("get_source_play_position", "playing_id", "extrapolate"), &Wwise::get_source_play_position);
	ClassDB::bind_method(
			D_METHOD("get_playing_segment_info", "playing_id", "extrapolate"), &Wwise::get_playing_segment_info);
	ClassDB::bind_method(D_METHOD("set_game_object_output_bus_volume", "game_object", "listener", "f_control_value"),
			&Wwise::set_game_object_output_bus_volume);
	ClassDB::bind_method(
			D_METHOD("set_game_object_aux_send_values", "game_object", "ak_aux_send_values", "num_send_values"),
			&Wwise::set_game_object_aux_send_values);
	ClassDB::bind_method(D_METHOD("set_object_obstruction_and_occlusion", "game_object", "listener", "calculated_obs",
								 "calculated_occ"),
			&Wwise::set_object_obstruction_and_occlusion);
	ClassDB::bind_method(D_METHOD("set_geometry", "vertices", "triangles", "acoustic_texture", "transission_loss_value",
								 "game_object", "enable_diffraction", "enable_diffraction_on_boundary_edges"),
			&Wwise::set_geometry);
	ClassDB::bind_method(D_METHOD("remove_geometry", "game_object"), &Wwise::remove_geometry);
	ClassDB::bind_method(
			D_METHOD("set_geometry_instance", "associated_geometry", "transform", "use_for_reflection_and_diffraction",
					"is_solid", "bypass_portal_subtraction", "geometry_instance"),
			&Wwise::set_geometry_instance);
	ClassDB::bind_method(D_METHOD("remove_geometry_instance", "geometry_instance"), &Wwise::remove_geometry_instance);
	ClassDB::bind_method(D_METHOD("register_spatial_listener", "game_object"), &Wwise::register_spatial_listener);
	ClassDB::bind_method(D_METHOD("set_room", "game_object", "aux_bus_id", "reverb_level", "transmission_loss",
								 "front_vector", "up_vector", "keep_registered", "associated_geometry"),
			&Wwise::set_room);
	ClassDB::bind_method(D_METHOD("remove_room", "game_object"), &Wwise::remove_room);
	ClassDB::bind_method(
			D_METHOD("set_portal", "game_object", "transform", "extent", "front_room", "back_room", "enabled"),
			&Wwise::set_portal);
	ClassDB::bind_method(D_METHOD("remove_portal", "game_object"), &Wwise::remove_portal);
	ClassDB::bind_method(
			D_METHOD("set_portal_obstruction_and_occlusion", "portal", "obstruction_value", "occlusion_value"),
			&Wwise::set_portal_obstruction_and_occlusion);
	ClassDB::bind_method(
			D_METHOD("set_game_object_to_portal_obstruction", "game_object", "portal", "obstruction_value"),
			&Wwise::set_game_object_to_portal_obstruction);
	ClassDB::bind_method(D_METHOD("set_portal_to_portal_obstruction", "portal0", "portal1", "obstruction_value"),
			&Wwise::set_portal_to_portal_obstruction);
	ClassDB::bind_method(D_METHOD("set_game_object_in_room", "game_object", "room"), &Wwise::set_game_object_in_room);
	ClassDB::bind_method(D_METHOD("remove_game_object_from_room", "game_object"), &Wwise::remove_game_object_from_room);
	ClassDB::bind_method(D_METHOD("set_early_reflections_aux_send", "game_object", "aux_id"),
			&Wwise::set_early_reflections_aux_send);
	ClassDB::bind_method(
			D_METHOD("set_early_reflections_volume", "game_object", "volume"), &Wwise::set_early_reflections_volume);
	ClassDB::bind_method(D_METHOD("add_output", "share_set", "output_id"), &Wwise::add_output);
	ClassDB::bind_method(D_METHOD("remove_output", "output_id"), &Wwise::remove_output);
	ClassDB::bind_method(D_METHOD("suspend", "render_anyway"), &Wwise::suspend);
	ClassDB::bind_method(D_METHOD("wakeup_from_suspend"), &Wwise::wakeup_from_suspend);
	ClassDB::bind_method(D_METHOD("stop_all", "game_object"), &Wwise::stop_all, DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("get_sample_tick"), &Wwise::get_sample_tick);
	ClassDB::bind_method(D_METHOD("get_id_from_string", "string"), &Wwise::get_id_from_string);
	ClassDB::bind_method(D_METHOD("is_initialized"), &Wwise::is_initialized);
}

void Wwise::init()
{
	bool result = initialize_wwise_systems();

	if (!result)
	{
		ERROR_CHECK_MSG(AK_Fail, "WwiseGodot: Sound engine initialization failed!");
		return;
	}
	else
	{
		UtilityFunctions::print("WwiseGodot: Sound engine initialized successfully.");
		UtilityFunctions::print(vformat(
				"WwiseGodot: Wwise(R) SDK %s Build %d.", AK_WWISESDK_VERSIONNAME_SHORT, AK_WWISESDK_VERSION_BUILD));
	}

	WwiseSettings* project_settings = WwiseSettings::get_singleton();

	String root_output_path = project_settings->get_setting(project_settings->common_user_settings.root_output_path);

	Ref<WwisePlatformInfo> platform_info;
	String platform_info_res_path;
	String banks_platform_suffix;

	auto load_platform_suffix = [&](const String& setting) -> String
	{
		platform_info_res_path = project_settings->get_setting(setting);
		platform_info = ResourceLoader::get_singleton()->load(platform_info_res_path);
		if (platform_info.is_valid())
		{
			return platform_info->get_platform_path();
		}
		return String();
	};

#ifdef AK_WIN
	banks_platform_suffix = load_platform_suffix(project_settings->project_settings.windows_platform_info);
#elif defined(AK_MAC_OS_X)
	banks_platform_suffix = load_platform_suffix(project_settings->project_settings.mac_platform_info);
#elif defined(AK_LINUX)
	banks_platform_suffix = load_platform_suffix(project_settings->project_settings.linux_platform_info);
#elif defined(AK_IOS)
	banks_platform_suffix = load_platform_suffix(project_settings->project_settings.ios_platform_info);
#elif defined(AK_ANDROID)
	banks_platform_suffix = load_platform_suffix(project_settings->project_settings.android_platform_info);
#else
#error "Platform not supported"
#endif

	if (banks_platform_suffix.is_empty())
	{
		UtilityFunctions::push_error("WwiseGodot: Failed to get the SoundBank directory for the current platform.");
	}

	String banks_path = vformat("%s/%s/", root_output_path, banks_platform_suffix);
	set_banks_path(banks_path);

	String startup_language = project_settings->get_setting(project_settings->common_user_settings.startup_language);
	set_current_language(startup_language);

#if !defined(AK_OPTIMIZED)
	const bool engineLogging =
			static_cast<bool>(project_settings->get_setting(project_settings->common_user_settings.engine_logging));

	if (engineLogging)
	{
		ERROR_CHECK_MSG(AK::Monitor::SetLocalOutput(
								AK::Monitor::ErrorLevel_All, static_cast<AK::Monitor::LocalOutputFunc>(LocalOutput)),
				"WwiseGodot: Setting local output to ErrorLevel_All failed.");
	}
#endif

	bool use_subfolders =
			project_settings->get_setting(project_settings->project_settings.create_subfolders_for_generated_files);

	low_level_io.set_use_subfolders(use_subfolders);

	bool use_soundbank_names = project_settings->get_setting(project_settings->project_settings.use_soundbank_names);

	if (use_soundbank_names)
	{
		AkBankManager::load_init_bank();
	}
	else
	{
		AkBankManager::load_init_bank_id();
	}
}

void Wwise::render_audio()
{
	AkBankManager::do_unload_banks();
	ERROR_CHECK(AK::SoundEngine::RenderAudio());
}

void Wwise::shutdown()
{
	if (shutdown_wwise_system())
	{
		UtilityFunctions::print("WwiseGodot: Sound engine terminated successfully.");
	}
}

void Wwise::set_banks_path(const String& p_banks_path)
{
	AKASSERT(!p_banks_path.is_empty());

	low_level_io.set_banks_path(p_banks_path);
}

void Wwise::set_current_language(const String& language)
{
	AKASSERT(!language.is_empty());

	low_level_io.set_language_folder(language);
}

bool Wwise::load_bank(const String& bank_name)
{
	AKASSERT(!bank_name.is_empty());

	AkBankID bank_id{};
	return ERROR_CHECK(AK::SoundEngine::LoadBank(bank_name.utf8().get_data(), bank_id));
}

bool Wwise::load_bank_id(const unsigned int bank_id) { return ERROR_CHECK(AK::SoundEngine::LoadBank(bank_id)); }

bool Wwise::load_bank_async(const String& bank_name, const Callable& cookie)
{
	AKASSERT(!bank_name.is_empty());

	AkBankID bank_id{};
	Callable* callable = memnew(Callable(cookie));
	bool result = ERROR_CHECK(AK::SoundEngine::LoadBank(
			bank_name.utf8().get_data(), (AkBankCallbackFunc)bank_callback, (void*)callable, bank_id));

	if (!result)
	{
		memdelete(callable);
	}

	return result;
}

bool Wwise::load_bank_async_id(const unsigned int bank_id, const Callable& cookie)
{
	Callable* callable = memnew(Callable(cookie));
	bool result = ERROR_CHECK(AK::SoundEngine::LoadBank(bank_id, (AkBankCallbackFunc)bank_callback, (void*)callable));

	if (!result)
	{
		memdelete(callable);
	}

	return result;
}

bool Wwise::unload_bank(const String& bank_name)
{
	AKASSERT(!bank_name.is_empty());

	return ERROR_CHECK(AK::SoundEngine::UnloadBank(bank_name.utf8().get_data(), NULL));
}

bool Wwise::unload_bank_id(const unsigned int bank_id)
{
	return ERROR_CHECK(AK::SoundEngine::UnloadBank(bank_id, NULL));
}

bool Wwise::unload_bank_async(const String& bank_name, const Callable& cookie)
{
	AKASSERT(!bank_name.is_empty());
	Callable* callable = memnew(Callable(cookie));
	bool result = ERROR_CHECK(AK::SoundEngine::UnloadBank(
			bank_name.utf8().get_data(), NULL, (AkBankCallbackFunc)bank_callback, (void*)callable));

	if (!result)
	{
		memdelete(callable);
	}

	return result;
}

bool Wwise::unload_bank_async_id(const unsigned int bank_id, const Callable& cookie)
{
	Callable* callable = memnew(Callable(cookie));
	bool result =
			ERROR_CHECK(AK::SoundEngine::UnloadBank(bank_id, NULL, (AkBankCallbackFunc)bank_callback, (void*)callable));

	if (!result)
	{
		memdelete(callable);
	}

	return result;
}

bool Wwise::register_listener(const Node* game_object)
{
	AKASSERT(game_object);

	if (!register_game_obj(game_object, "Default_Listener"))
	{
		return ERROR_CHECK(AK_Fail);
	}

	AkGameObjectID id = get_ak_game_object_id(game_object);
	return ERROR_CHECK(AK::SoundEngine::SetDefaultListeners(&id, 1));
}

bool Wwise::register_game_obj(const Node* game_object, const String& game_object_name)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	AKRESULT result = AK::SoundEngine::RegisterGameObj(id, game_object_name.utf8().get_data());
	post_register_game_object(result, game_object, id);
	return ERROR_CHECK_MSG(
			result, vformat("WwiseGodot: Failed to register Game Object with name: %s.", game_object_name));
}

bool Wwise::unregister_game_obj(const Node* game_object)
{
	if (!game_object || !is_initialized())
	{
		return true;
	}

	AkGameObjectID id = get_ak_game_object_id(game_object);
	AKRESULT result = AK::SoundEngine::UnregisterGameObj(id);
	post_unregister_game_object(result, game_object, id);
	return ERROR_CHECK(result);
}

bool Wwise::add_listener(Node* emitter, Node* listener)
{
	AkGameObjectID emitter_id = get_ak_game_object_id(emitter);
	pre_game_object_api_call(emitter, emitter_id);

	AkGameObjectID listener_id = get_ak_game_object_id(listener);
	pre_game_object_api_call(listener, listener_id);

	return ERROR_CHECK(AK::SoundEngine::AddListener(emitter_id, listener_id));
}

bool Wwise::remove_listener(Node* emitter, Node* listener)
{
	AkGameObjectID emitter_id = get_ak_game_object_id(emitter);
	pre_game_object_api_call(emitter, emitter_id);

	AkGameObjectID listener_id = get_ak_game_object_id(listener);
	pre_game_object_api_call(listener, listener_id);

	return ERROR_CHECK(AK::SoundEngine::RemoveListener(emitter_id, listener_id));
}

bool Wwise::add_default_listener(Node* game_object)
{
	AkGameObjectID listener_id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, listener_id);

	return ERROR_CHECK(AK::SoundEngine::AddDefaultListener(listener_id));
}

bool Wwise::remove_default_listener(Node* game_object)
{
	AkGameObjectID listener_id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, listener_id);

	return ERROR_CHECK(AK::SoundEngine::RemoveDefaultListener(listener_id));
}

bool Wwise::set_listeners(Node* emitter, TypedArray<Node> listeners)
{
	AkGameObjectID emitter_id = get_ak_game_object_id(emitter);
	pre_game_object_api_call(emitter, emitter_id);

	if (listeners.is_empty())
	{
		return ERROR_CHECK(AK::SoundEngine::SetListeners(emitter_id, nullptr, 0));
	}

	uint32_t num_listeners = listeners.size();
	auto listener_array = std::make_unique<AkGameObjectID[]>(num_listeners);
	for (int i = 0; i < listeners.size(); ++i)
	{
		AkGameObjectID listener_id = get_ak_game_object_id(Object::cast_to<Node>(listeners[i]));
		listener_array[i] = listener_id;
	}

	return ERROR_CHECK(AK::SoundEngine::SetListeners(emitter_id, listener_array.get(), num_listeners));
}

bool Wwise::set_distance_probe(Node* listener_game_object, Node* probe_game_object)
{
	AkGameObjectID listener_id = get_ak_game_object_id(listener_game_object);
	pre_game_object_api_call(listener_game_object, listener_id);

	AkGameObjectID probe_id = get_ak_game_object_id(probe_game_object);
	pre_game_object_api_call(probe_game_object, probe_id);

	return ERROR_CHECK(AK::SoundEngine::SetDistanceProbe(listener_id, probe_id));
}

bool Wwise::reset_distance_probe(Node* listener_game_object)
{
	AkGameObjectID listener_id = get_ak_game_object_id(listener_game_object);
	pre_game_object_api_call(listener_game_object, listener_id);

	return ERROR_CHECK(AK::SoundEngine::SetDistanceProbe(listener_id, AK_INVALID_GAME_OBJECT));
}

void Wwise::set_random_seed(const unsigned int seed) { AK::SoundEngine::SetRandomSeed(seed); }

bool Wwise::set_3d_position(const Node* game_object, const Transform3D& transform_3d)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);

	AkSoundPosition sound_pos{};
	godot_transform3d_to_ak_sound_position(transform_3d, sound_pos);

	return ERROR_CHECK(AK::SoundEngine::SetPosition(id, sound_pos));
}

bool Wwise::set_2d_position(const Node* game_object, const Transform2D& transform_2d, const real_t z_depth)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);

	AkSoundPosition sound_pos{};
	godot_transform2d_to_ak_sound_position(transform_2d, z_depth, sound_pos);

	return ERROR_CHECK(AK::SoundEngine::SetPosition(id, sound_pos));
}

bool Wwise::set_multiple_positions_3d(const Node* game_object, const TypedArray<Transform3D>& positions,
		const int num_positions, const AkUtils::MultiPositionType multi_position_type)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);

	auto ak_positions = std::make_unique<AkSoundPosition[]>(num_positions);
	for (int i = 0; i < positions.size(); i++)
	{
		const Transform3D& transform = positions[i];
		godot_transform3d_to_ak_sound_position(transform, ak_positions[i]);
	}

	return ERROR_CHECK(AK::SoundEngine::SetMultiplePositions(
			id, ak_positions.get(), num_positions, static_cast<AkMultiPositionType>(multi_position_type)));
}

bool Wwise::set_multiple_positions_2d(const Node* game_object, const TypedArray<Transform2D>& positions,
		const TypedArray<real_t>& z_depths, const int num_positions,
		const AkUtils::MultiPositionType multi_position_type)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);

	auto ak_positions = std::make_unique<AkSoundPosition[]>(num_positions);
	for (int i = 0; i < positions.size(); i++)
	{
		const Transform2D& transform = positions[i];
		godot_transform2d_to_ak_sound_position(transform, z_depths[i], ak_positions[i]);
	}

	return ERROR_CHECK(AK::SoundEngine::SetMultiplePositions(
			id, ak_positions.get(), num_positions, static_cast<AkMultiPositionType>(multi_position_type)));
}

bool Wwise::set_game_object_radius(const Node* game_object, const float outer_radius, const float inner_radius)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	return ERROR_CHECK(AK::SpatialAudio::SetGameObjectRadius(id, outer_radius, inner_radius));
}

AkPlayingID Wwise::post_event(const String& event_name, Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);
	AkPlayingID playing_id = AK::SoundEngine::PostEvent(event_name.utf8().get_data(), id);

	if (playing_id == AK_INVALID_PLAYING_ID && game_object)
	{
		ERROR_CHECK_MSG(AK_InvalidID,
				vformat("Failed to post Event: %s on Game Object: %s.", event_name, game_object->get_name()));
	}

	return playing_id;
}

AkPlayingID Wwise::post_event_callback(
		const String& event_name, AkUtils::AkCallbackType flags, Node* game_object, const Callable& cookie)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	AkEventCallbackPackage* callback_package = new AkEventCallbackPackage;
	callback_package->object_id = cookie.get_object_id();
	callback_package->cookie = memnew(Callable(cookie));
	callback_package->notify_end_of_event = (flags & AkUtils::AkCallbackType::AK_END_OF_EVENT);
	flags = static_cast<AkUtils::AkCallbackType>(flags | AkUtils::AkCallbackType::AK_END_OF_EVENT);

	AkPlayingID playing_id = AK::SoundEngine::PostEvent(
			event_name.utf8().get_data(), id, flags, event_callback, (void*)callback_package);

	if (playing_id == AK_INVALID_PLAYING_ID)
	{
		if (callback_package)
		{
			memdelete(callback_package->cookie);
			delete callback_package;
		}

		if (game_object)
		{
			ERROR_CHECK_MSG(AK_InvalidID,
					vformat("Failed to post Event: %s on Game Object: %s.", event_name, game_object->get_name()));
		}
		else
		{
			ERROR_CHECK_MSG(AK_InvalidID,
					vformat("Failed to post Event: %s. An invalid Game Object was passed to this function.",
							event_name));
		}
	}

	return playing_id;
}

AkPlayingID Wwise::post_event_id(uint32_t event_id, Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);
	AkPlayingID playing_id = AK::SoundEngine::PostEvent(event_id, id);

	if (playing_id == AK_INVALID_PLAYING_ID && game_object)
	{
		ERROR_CHECK_MSG(AK_InvalidID,
				vformat("Failed to post Event: %d on Game Object: %s.", event_id, game_object->get_name()));
	}

	return playing_id;
}

AkPlayingID Wwise::post_event_id_callback(
		uint32_t event_id, AkUtils::AkCallbackType flags, Node* game_object, const Callable& cookie)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	AkEventCallbackPackage* callback_package = new AkEventCallbackPackage;
	callback_package->object_id = cookie.get_object_id();
	callback_package->cookie = memnew(Callable(cookie));
	callback_package->notify_end_of_event = (flags & AkUtils::AkCallbackType::AK_END_OF_EVENT);
	flags = static_cast<AkUtils::AkCallbackType>(flags | AkUtils::AkCallbackType::AK_END_OF_EVENT);

	AkPlayingID playing_id = AK::SoundEngine::PostEvent(event_id, id, flags, event_callback, (void*)callback_package);

	if (playing_id == AK_INVALID_PLAYING_ID)
	{
		if (callback_package)
		{
			memdelete(callback_package->cookie);
			delete callback_package;
		}

		if (game_object)
		{
			ERROR_CHECK_MSG(AK_InvalidID,
					vformat("Failed to post Event with ID: %d on Game Object: %s.", event_id, game_object->get_name()));
		}
		else
		{
			ERROR_CHECK_MSG(AK_InvalidID,
					vformat("Failed to post Event with ID: %d. An invalid Game Object was passed to this function.",
							event_id));
		}
	}

	return playing_id;
}

AkPlayingID Wwise::post_midi_on_event_id(
		const AkUniqueID p_event_id, Node* p_game_object, TypedArray<AkMidiPost> p_midi_posts, bool p_absolute_offsets)
{
	AkGameObjectID id = get_ak_game_object_id(p_game_object);
	pre_game_object_api_call(p_game_object, id);

	auto ak_midi_posts = std::make_unique<AkMIDIPost[]>(p_midi_posts.size());

	for (int i = 0; i < p_midi_posts.size(); i++)
	{
		Ref<AkMidiPost> post = p_midi_posts[i];
		if (!post.is_valid())
		{
			continue;
		}

		AkMIDIPost ak_post{};
		ak_post.midiEvent.byType = post->get_by_type();
		ak_post.midiEvent.byChan = post->get_by_chan();
		ak_post.uOffset = post->get_u_offset();

		switch (ak_post.midiEvent.byType)
		{
			case AkMidiPost::MidiEventType::MIDI_EVENT_TYPE_NOTE_OFF:
			case AkMidiPost::MidiEventType::MIDI_EVENT_TYPE_NOTE_ON:
			{
				ak_post.midiEvent.NoteOnOff.byVelocity = post->get_by_velocity();
				ak_post.midiEvent.NoteOnOff.byNote = post->get_by_note();
				break;
			}
			default:
				break;
		}
		ak_midi_posts[i] = ak_post;
	}

	AkPlayingID playing_id = AK::SoundEngine::PostMIDIOnEvent(
			p_event_id, id, ak_midi_posts.get(), p_midi_posts.size(), p_absolute_offsets);

	if (playing_id == AK_INVALID_PLAYING_ID && p_game_object)
	{
		ERROR_CHECK_MSG(AK_InvalidID,
				vformat("Failed to post Midi on Event with ID: %d on Game Object: %s.", p_event_id,
						p_game_object->get_name()));
	}

	return playing_id;
}

void Wwise::stop_event(
		const unsigned int playing_id, const unsigned int fade_time, const AkUtils::AkCurveInterpolation interpolation)
{
	AK::SoundEngine::ExecuteActionOnPlayingID(AkActionOnEventType::AkActionOnEventType_Stop,
			static_cast<AkPlayingID>(playing_id), static_cast<AkTimeMs>(fade_time),
			static_cast<AkCurveInterpolation>(interpolation));
}

bool Wwise::stop_midi_on_event_id(const AkUniqueID p_event_id, Node* p_game_object)
{
	AkGameObjectID id = get_ak_game_object_id(p_game_object);
	pre_game_object_api_call(p_game_object, id);

	return ERROR_CHECK(AK::SoundEngine::StopMIDIOnEvent(p_event_id, id));
}

bool Wwise::execute_action_on_event_id(AkUniqueID p_event_id, AkUtils::AkActionOnEventType p_action_type,
		Node* game_object, int p_transition_duration, AkUtils::AkCurveInterpolation p_fade_curve,
		AkPlayingID p_playing_id)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	return ERROR_CHECK(AK::SoundEngine::ExecuteActionOnEvent(p_event_id, (AkActionOnEventType)p_action_type, id,
			p_transition_duration, (AkCurveInterpolation)p_fade_curve, p_playing_id));
}

bool Wwise::set_switch(const String& switch_group, const String& switch_value, Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	return ERROR_CHECK(AK::SoundEngine::SetSwitch(switch_group.utf8().get_data(), switch_value.utf8().get_data(), id));
}

bool Wwise::set_switch_id(const unsigned int switch_group_id, const unsigned int switch_value_id, Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	return ERROR_CHECK(AK::SoundEngine::SetSwitch(switch_group_id, switch_value_id, id));
}

bool Wwise::set_state(const String& state_group, const String& state_value)
{
	return ERROR_CHECK(AK::SoundEngine::SetState(state_group.utf8().get_data(), state_value.utf8().get_data()));
}

bool Wwise::set_state_id(const unsigned int state_group_id, const unsigned int state_value_id)
{
	return ERROR_CHECK(AK::SoundEngine::SetState(state_group_id, state_value_id));
}

float Wwise::get_rtpc_value(const String& rtpc_name, Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	AK::SoundEngine::Query::RTPCValue_type type = AK::SoundEngine::Query::RTPCValue_Default;
	if (id == AK_INVALID_GAME_OBJECT)
	{
		type = AK::SoundEngine::Query::RTPCValue_Global;
	}
	else
	{
		type = AK::SoundEngine::Query::RTPCValue_GameObject;
	}

	AkRtpcValue value;
	if (!ERROR_CHECK(AK::SoundEngine::Query::GetRTPCValue(
				rtpc_name.utf8().get_data(), id, static_cast<AkPlayingID>(0), value, type)))
	{
		return INVALID_RTPC_VALUE;
	}

	return value;
}

float Wwise::get_rtpc_value_id(const unsigned int rtpc_id, Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	AK::SoundEngine::Query::RTPCValue_type type = AK::SoundEngine::Query::RTPCValue_Default;
	if (id == AK_INVALID_GAME_OBJECT)
	{
		type = AK::SoundEngine::Query::RTPCValue_Global;
	}
	else
	{
		type = AK::SoundEngine::Query::RTPCValue_GameObject;
	}

	AkRtpcValue value;
	if (!ERROR_CHECK(AK::SoundEngine::Query::GetRTPCValue(rtpc_id, id, static_cast<AkPlayingID>(0), value, type)))
	{
		return INVALID_RTPC_VALUE;
	}

	return value;
}

bool Wwise::set_rtpc_value(const String& rtpc_name, const float rtpc_value, Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	return ERROR_CHECK(AK::SoundEngine::SetRTPCValue(rtpc_name.utf8().get_data(), rtpc_value, id));
}

bool Wwise::set_rtpc_value_id(const unsigned int rtpc_id, const float rtpc_value, Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	return ERROR_CHECK(AK::SoundEngine::SetRTPCValue(rtpc_id, rtpc_value, id));
}

bool Wwise::post_trigger(const String& trigger_name, Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	return ERROR_CHECK(AK::SoundEngine::PostTrigger(trigger_name.utf8().get_data(), id));
}

bool Wwise::post_trigger_id(const unsigned int trigger_id, Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	return ERROR_CHECK(AK::SoundEngine::PostTrigger(trigger_id, id));
}

unsigned int Wwise::post_external_source(const String& event_name, Node* game_object, const String& source_object_name,
		const String& filename, const AkUtils::AkCodecID id_codec)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	AkExternalSourceInfo source{};
	source.iExternalSrcCookie = AK::SoundEngine::GetIDFromString(source_object_name.utf8().get_data());

	source.szFile = filename.utf8().get_data();
	source.idCodec = id_codec;

	AkPlayingID playing_id = AK::SoundEngine::PostEvent(event_name.utf8().get_data(), id, 0, nullptr, 0, 1, &source);

	if (playing_id == AK_INVALID_PLAYING_ID && game_object)
	{
		ERROR_CHECK_MSG(AK_InvalidID,
				vformat("Failed to post External Source with Event: %s on Game Object: %s", event_name,
						game_object->get_name()));
		return AK_INVALID_PLAYING_ID;
	}

	return playing_id;
}

unsigned int Wwise::post_external_source_id(const unsigned int event_id, Node* game_object,
		const unsigned int source_object_id, const String& filename, const AkUtils::AkCodecID id_codec)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	AkExternalSourceInfo source{};
	source.iExternalSrcCookie = source_object_id;

	source.szFile = filename.utf8().get_data();
	source.idCodec = id_codec;

	AkPlayingID playing_id = AK::SoundEngine::PostEvent(event_id, id, 0, NULL, 0, 1, &source);

	if (playing_id == AK_INVALID_PLAYING_ID && game_object)
	{
		ERROR_CHECK_MSG(AK_InvalidID,
				vformat("Failed to post External Source with Event ID: %d on Game Object: %s", event_id,
						game_object->get_name()));
		return AK_INVALID_PLAYING_ID;
	}

	return playing_id;
}

unsigned int Wwise::post_external_sources(
		const unsigned int event_id, Node* game_object, TypedArray<WwiseExternalSourceInfo> external_source_info)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	auto ak_external_sources = std::make_unique<AkExternalSourceInfo[]>(external_source_info.size());

	for (int i = 0; i < external_source_info.size(); i++)
	{
		Ref<WwiseExternalSourceInfo> info = external_source_info[i];
		if (!info.is_valid())
		{
			continue;
		}

		AkExternalSourceInfo ak_info{};
		ak_info.iExternalSrcCookie = info->get_external_src_cookie();
		ak_info.idCodec = info->get_id_codec();
		ak_info.szFile = info->get_sz_file().utf8().get_data();

		ak_external_sources[i] = ak_info;
	}

	AkPlayingID playing_id = AK::SoundEngine::PostEvent(
			event_id, id, 0, NULL, 0, external_source_info.size(), ak_external_sources.get());

	if (playing_id == AK_INVALID_PLAYING_ID && game_object)
	{
		ERROR_CHECK_MSG(AK_InvalidID,
				vformat("Failed to post External Source with Event ID: %d on Game Object: %s", event_id,
						game_object->get_name()));
		return AK_INVALID_PLAYING_ID;
	}

	return playing_id;
}

int Wwise::get_source_play_position(const unsigned int playing_id, const bool extrapolate)
{
	AkTimeMs position{};
	AKRESULT result = AK::SoundEngine::GetSourcePlayPosition(playing_id, &position, extrapolate);

	if (result == AK_Fail)
	{
		ERROR_CHECK(result);
		return AK_INVALID_PLAYING_ID;
	}

	return position;
}

Dictionary Wwise::get_playing_segment_info(const unsigned int playing_id, const bool extrapolate)
{
	AkSegmentInfo segment_info;
	AKRESULT result = AK::SoundEngine::GetPlayingSegmentInfo(playing_id, segment_info, extrapolate);

	if (result == AK_Fail)
	{
		ERROR_CHECK(result);
	}

	Dictionary segment;
	segment["fBarDuration"] = segment_info.fBarDuration;
	segment["fBeatDuration"] = segment_info.fBeatDuration;
	segment["fGridDuration"] = segment_info.fGridDuration;
	segment["fGridOffset"] = segment_info.fGridOffset;
	segment["iActiveDuration"] = segment_info.iActiveDuration;
	segment["iCurrentPosition"] = segment_info.iCurrentPosition;
	segment["iPostExitDuration"] = segment_info.iPostExitDuration;
	segment["iPreEntryDuration"] = segment_info.iPreEntryDuration;
	segment["iRemainingLookAheadTime"] = segment_info.iRemainingLookAheadTime;

	return segment;
}

bool Wwise::set_game_object_output_bus_volume(Node* game_object, Node* listener, float f_control_value)
{
	AkGameObjectID emitter_id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, emitter_id);

	AkGameObjectID listener_id = get_ak_game_object_id(listener);
	pre_game_object_api_call(listener, listener_id);

	return ERROR_CHECK(AK::SoundEngine::SetGameObjectOutputBusVolume(emitter_id, listener_id, f_control_value));
}

bool Wwise::set_game_object_aux_send_values(
		Node* game_object, const Array ak_aux_send_values, const unsigned int num_send_values)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	std::vector<AkAuxSendValue> environments;

	for (int i = 0; i < ak_aux_send_values.size(); i++)
	{
		Dictionary aux_bus_data = ak_aux_send_values[i];
		AkAuxSendValue environment{};
		environment.auxBusID = static_cast<unsigned int>(aux_bus_data["aux_bus_id"]);
		environment.fControlValue = static_cast<float>(aux_bus_data["control_value"]);
		environment.listenerID = AK_INVALID_GAME_OBJECT;
		if (aux_bus_data.has("listener_path"))
		{
			NodePath listener_node_path = static_cast<NodePath>(aux_bus_data["listener_path"]);
			Node* listener_game_object = game_object->get_tree()->get_root()->get_node_or_null(listener_node_path);
			if (listener_game_object != nullptr)
			{
				AkGameObjectID listener_id = get_ak_game_object_id(listener_game_object);
				pre_game_object_api_call(listener_game_object, listener_id);
				environment.listenerID = listener_id;
			}
		}
		environments.push_back(environment);
	}

	environments.resize(AK_MAX_ENVIRONMENTS);

	return ERROR_CHECK(AK::SoundEngine::SetGameObjectAuxSendValues(id, environments.data(), num_send_values));
}

bool Wwise::set_object_obstruction_and_occlusion(
		Node* game_object, Node* listener, float f_calculated_obs, float f_calculated_occ)
{
	AkGameObjectID emitter_id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, emitter_id);

	AkGameObjectID listener_id = get_ak_game_object_id(listener);
	pre_game_object_api_call(listener, listener_id);

	return ERROR_CHECK(AK::SoundEngine::SetObjectObstructionAndOcclusion(
			emitter_id, listener_id, f_calculated_obs, f_calculated_occ));
}

bool Wwise::set_geometry(const Array vertices, const Array triangles, const Ref<WwiseAcousticTexture>& acoustic_texture,
		float transmission_loss_value, const Node* game_object, bool enable_diffraction,
		bool enable_diffraction_on_boundary_edges)
{
	AKASSERT(!vertices.is_empty());
	AKASSERT(!triangles.is_empty());
	AkGameObjectID id = get_ak_game_object_id(game_object);

	if (id == AK_INVALID_GAME_OBJECT)
	{
		UtilityFunctions::printerr("WwiseGodot: Failed to Set Geometry, passed game_object is null.");
		return false;
	}

	AkGeometryParams geometry{};

	int vertex_count = vertices.size();
	auto verts_remap = std::make_unique<int[]>(vertex_count);
	PackedVector3Array unique_verts;
	Dictionary verts_dict;

	// removing duplicate verts
	for (int v = 0; v < vertex_count; ++v)
	{
		int verts_index = 0;

		if (!find_matching_vertex(vertices[v], verts_dict, verts_index))
		{
			verts_index = unique_verts.size();
			unique_verts.append(vertices[v]);
			verts_dict[vertices[v]] = verts_index;
		}
		verts_remap[v] = verts_index;
	}

	vertex_count = unique_verts.size();
	auto ak_vertices = std::make_unique<AkVertex[]>(vertex_count);

	for (int i = 0; i < vertex_count; i++)
	{
		Vector3 point = unique_verts[i];
		AkVertex v{};
		v.X = point.x;
		v.Y = point.y;
		v.Z = point.z;

		ak_vertices[i] = v;
	}

	if (triangles.size() % 3 != 0)
	{
		UtilityFunctions::print(
				vformat("WwiseGodot: Wrong number of triangles on GameObject: %s.", game_object->get_path()));
	}

	int num_triangles = triangles.size() / 3;
	auto ak_triangles = std::make_unique<AkTriangle[]>(num_triangles);

	AkAcousticSurface surface{};
	String texture_name = acoustic_texture.is_valid() ? acoustic_texture->get_name() : "";
	AkUInt32 texture_id = acoustic_texture.is_valid() ? acoustic_texture->get_id() : AK_INVALID_SURFACE;

	bool texture_valid = !texture_name.is_empty() && texture_id != AK_INVALID_SURFACE;
	if (texture_valid)
	{
		surface.strName = texture_name.utf8().get_data();
		surface.textureID = texture_id;
	}
	surface.transmissionLoss = transmission_loss_value;

	AkAcousticSurface ak_surfaces[1] = { surface };
	geometry.NumSurfaces = 1;
	geometry.Surfaces = ak_surfaces;

	int triangleIdx = 0;
	for (int i = 0; i < num_triangles; i++)
	{
		AkTriangle t{};
		t.point0 = verts_remap[static_cast<unsigned int>(triangles[3 * i + 0])];
		t.point1 = verts_remap[static_cast<unsigned int>(triangles[3 * i + 1])];
		t.point2 = verts_remap[static_cast<unsigned int>(triangles[3 * i + 2])];
		t.surface = 0;

		ak_triangles[triangleIdx] = t;

		if (t.point0 != t.point1 && t.point0 != t.point2 && t.point1 != t.point2)
		{
			++triangleIdx;
		}
		else
		{
			UtilityFunctions::print(
					vformat("WwiseGodot: Skipped degenerate triangles on GameObject: %s.", game_object->get_path()));
		}
	}

	geometry.Vertices = ak_vertices.get();
	geometry.NumVertices = vertex_count;
	geometry.Triangles = ak_triangles.get();
	geometry.NumTriangles = triangleIdx;
	geometry.EnableDiffraction = enable_diffraction;
	geometry.EnableDiffractionOnBoundaryEdges = enable_diffraction_on_boundary_edges;

	return ERROR_CHECK(AK::SpatialAudio::SetGeometry(id, geometry));
}

bool Wwise::remove_geometry(const Object* game_object)
{
	AKASSERT(game_object);

	return ERROR_CHECK(AK::SpatialAudio::RemoveGeometry(static_cast<AkGeometrySetID>(game_object->get_instance_id())));
}

bool Wwise::set_geometry_instance(const Object* associated_geometry, const Transform3D& transform,
		bool use_for_reflection_and_diffraction, bool is_solid, bool bypass_portal_subtraction,
		const Object* geometry_instance)
{
	AkGeometryInstanceParams params{};
	params.GeometrySetID = associated_geometry->get_instance_id();
	params.UseForReflectionAndDiffraction = use_for_reflection_and_diffraction;
	params.IsSolid = is_solid;
	params.BypassPortalSubtraction = bypass_portal_subtraction;
	godot_transform3d_to_ak_sound_position(transform, params.PositionAndOrientation);

	Vector3 scale = transform.get_basis().get_scale();
	params.Scale = { static_cast<AkReal32>(scale.x), static_cast<AkReal32>(scale.y), static_cast<AkReal32>(scale.z) };

	return ERROR_CHECK(AK::SpatialAudio::SetGeometryInstance(
			static_cast<AkGeometryInstanceID>(geometry_instance->get_instance_id()), params));
}

bool Wwise::remove_geometry_instance(const Object* geometry_instance)
{
	return ERROR_CHECK(AK::SpatialAudio::RemoveGeometryInstance(
			static_cast<AkGeometryInstanceID>(geometry_instance->get_instance_id())));
}

bool Wwise::register_spatial_listener(Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	return ERROR_CHECK(AK::SpatialAudio::RegisterListener(id));
}

bool Wwise::set_room(const Node* game_object, const unsigned int ak_aux_bus_id, const float reverb_level,
		const float transmission_loss, const Vector3& front_vector, const Vector3& up_vector, bool keep_registered,
		const Object* associated_geometry_instance)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);

	AkRoomParams room_params{};
	room_params.ReverbAuxBus = ak_aux_bus_id;
	room_params.ReverbLevel = reverb_level;
	room_params.TransmissionLoss = transmission_loss;
	room_params.Front = godot_to_ak_orientation(front_vector);
	room_params.Up = godot_to_ak_orientation(up_vector);
	room_params.RoomGameObj_KeepRegistered = keep_registered;
	room_params.RoomGameObj_AuxSendLevelToSelf = 0.0f;

	if (associated_geometry_instance)
	{
		room_params.GeometryInstanceID = static_cast<AkGeometrySetID>(associated_geometry_instance->get_instance_id());
	}

	return ERROR_CHECK(AK::SpatialAudio::SetRoom(AkRoomID::FromGameObjectID(id), room_params,
			game_object ? String(game_object->get_name()).utf8().get_data() : nullptr));
}

bool Wwise::remove_room(const Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	return ERROR_CHECK(AK::SpatialAudio::RemoveRoom(AkRoomID::FromGameObjectID(id)));
}

bool Wwise::set_portal(const Node* game_object, const Transform3D transform, const Vector3& extent,
		const Node* front_room, const Node* back_room, bool enabled)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	AkGameObjectID front_room_id = get_ak_game_object_id(front_room);
	AkGameObjectID back_room_id = get_ak_game_object_id(back_room);

	AkWorldTransform ak_transform{};
	godot_transform3d_to_ak_sound_position(transform, ak_transform);

	AkPortalParams portal_params;
	AkExtent portal_extent;

	portal_extent.halfWidth = extent.x * 0.5f;
	portal_extent.halfHeight = extent.y * 0.5f;
	portal_extent.halfDepth = extent.z * 0.5f;

	portal_params.Transform = ak_transform;
	portal_params.Extent = portal_extent;
	portal_params.FrontRoom = front_room ? AkRoomID::FromGameObjectID(front_room_id) : AK::SpatialAudio::kOutdoorRoomID;
	portal_params.BackRoom = back_room ? AkRoomID::FromGameObjectID(back_room_id) : AK::SpatialAudio::kOutdoorRoomID;
	portal_params.bEnabled = enabled;

	return ERROR_CHECK(AK::SpatialAudio::SetPortal(id, portal_params));
}

bool Wwise::remove_portal(const Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);

	return ERROR_CHECK(AK::SpatialAudio::RemovePortal(id));
}

bool Wwise::set_portal_obstruction_and_occlusion(
		const Node* portal, const float obstruction_value, const float occlusion_value)
{
	AkGameObjectID id = get_ak_game_object_id(portal);

	return ERROR_CHECK(AK::SpatialAudio::SetPortalObstructionAndOcclusion(id, obstruction_value, occlusion_value));
}

bool Wwise::set_game_object_to_portal_obstruction(
		const Node* game_object, const Node* portal, const float obstruction_value)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	AkGameObjectID portal_id = get_ak_game_object_id(portal);

	return ERROR_CHECK(AK::SpatialAudio::SetGameObjectToPortalObstruction(id, portal_id, obstruction_value));
}

bool Wwise::set_portal_to_portal_obstruction(const Node* portal0, const Node* portal1, const float obstruction_value)
{
	AkGameObjectID portal0_id = get_ak_game_object_id(portal0);
	AkGameObjectID portal1_id = get_ak_game_object_id(portal1);

	return ERROR_CHECK(AK::SpatialAudio::SetPortalToPortalObstruction(portal0_id, portal1_id, obstruction_value));
}

bool Wwise::set_game_object_in_room(Node* game_object, const Node* room)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	AkGameObjectID room_id = get_ak_game_object_id(room);

	return ERROR_CHECK(AK::SpatialAudio::SetGameObjectInRoom(id, AkRoomID::FromGameObjectID(room_id)));
}

bool Wwise::remove_game_object_from_room(Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	return ERROR_CHECK(AK::SpatialAudio::SetGameObjectInRoom(id, INVALID_ROOM_ID));
}

bool Wwise::set_early_reflections_aux_send(Node* game_object, const unsigned int aux_bus_id)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	return ERROR_CHECK(AK::SpatialAudio::SetEarlyReflectionsAuxSend(id, aux_bus_id));
}

bool Wwise::set_early_reflections_volume(Node* game_object, const float volume)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);
	pre_game_object_api_call(game_object, id);

	return ERROR_CHECK(AK::SpatialAudio::SetEarlyReflectionsVolume(id, volume));
}

bool Wwise::add_output(const String& share_set, const unsigned int output_id)
{
	AkOutputSettings output_settings(share_set.utf8().get_data(), output_id);

	return ERROR_CHECK(AK::SoundEngine::AddOutput(output_settings));
}

bool Wwise::remove_output(const unsigned int output_id)
{
	return ERROR_CHECK(AK::SoundEngine::RemoveOutput(output_id));
}

bool Wwise::suspend(bool render_anyway) { return ERROR_CHECK(AK::SoundEngine::Suspend(render_anyway)); }

bool Wwise::wakeup_from_suspend() { return ERROR_CHECK(AK::SoundEngine::WakeupFromSuspend()); }

void Wwise::stop_all(Node* game_object)
{
	AkGameObjectID id = get_ak_game_object_id(game_object);

	if (id == AK_INVALID_GAME_OBJECT)
	{
		AK::SoundEngine::StopAll();
	}
	else
	{
		AK::SoundEngine::StopAll(id);
	}
}

uint64_t Wwise::get_sample_tick() { return AK::SoundEngine::GetSampleTick(); }

uint32_t Wwise::get_id_from_string(const String& p_string)
{
	return AK::SoundEngine::GetIDFromString(p_string.utf8().get_data());
}

bool Wwise::is_initialized() { return AK::SoundEngine::IsInitialized(); }

void Wwise::event_callback(enum AkCallbackType in_eType, struct AkEventCallbackInfo* in_pEventInfo,
		void* in_pCallbackInfo, void* in_pCookie)
{
	Wwise* soundengine = get_singleton();

	AkAutoLock<CAkLock> scoped_lock(callback_data_lock);

	AkEventCallbackPackage* package = static_cast<AkEventCallbackPackage*>(in_pCookie);
	if (!package || !package->cookie)
	{
		ERROR_CHECK_MSG(AK_Fail, "Event Callback package is not valid.");
		return;
	}

	Array args = Array();
	Dictionary callback_data;
	callback_data["callback_type"] = static_cast<unsigned int>(in_eType);

	switch (in_eType)
	{
		case AK_EndOfEvent:
		case AK_Starvation:
		case AK_MusicPlayStarted:
		{
			pack_base_event_info(callback_data, in_pEventInfo);
			break;
		}
		case AK_EndOfDynamicSequenceItem:
		{
			const auto* info = static_cast<AkDynamicSequenceItemCallbackInfo*>(in_pCallbackInfo);
			pack_base_event_info(callback_data, in_pEventInfo);
			callback_data["audioNodeID"] = info->audioNodeID;
			break;
		}
		case AK_Marker:
		{
			const auto* info = static_cast<AkMarkerCallbackInfo*>(in_pCallbackInfo);
			pack_base_event_info(callback_data, in_pEventInfo);
			callback_data["uIdentifier"] = info->uIdentifier;
			callback_data["uPosition"] = info->uPosition;
			callback_data["strLabel"] = info->strLabel ? String(info->strLabel) : String();
			break;
		}
		case AK_Duration:
		{
			const auto* info = static_cast<AkDurationCallbackInfo*>(in_pCallbackInfo);
			pack_base_event_info(callback_data, in_pEventInfo);
			callback_data["fDuration"] = info->fDuration;
			callback_data["fEstimatedDuration"] = info->fEstimatedDuration;
			callback_data["audioNodeID"] = info->audioNodeID;
			callback_data["mediaID"] = info->mediaID;
			callback_data["bStreaming"] = info->bStreaming;
			break;
		}
		case AK_MusicSyncBeat:
		case AK_MusicSyncBar:
		case AK_MusicSyncEntry:
		case AK_MusicSyncExit:
		case AK_MusicSyncGrid:
		case AK_MusicSyncUserCue:
		case AK_MusicSyncPoint:
		case AK_MusicSyncAll:
		{
			const auto* info = static_cast<AkMusicSyncCallbackInfo*>(in_pCallbackInfo);
			pack_base_event_info(callback_data, in_pEventInfo);
			callback_data["musicSyncType"] = info->musicSyncType;
			callback_data["pszUserCueName"] = info->pszUserCueName ? String(info->pszUserCueName) : String();
			pack_segment_info(callback_data, info->segmentInfo);
			break;
		}
		case AK_MusicPlaylistSelect:
		{
			const auto* info = static_cast<AkMusicPlaylistCallbackInfo*>(in_pCallbackInfo);
			pack_base_event_info(callback_data, in_pEventInfo);
			callback_data["playlistID"] = info->playlistID;
			callback_data["uNumPlaylistItems"] = info->uNumPlaylistItems;
			callback_data["uPlaylistSelection"] = info->uPlaylistSelection;
			callback_data["uPlaylistItemDone"] = info->uPlaylistItemDone;
			break;
		}
		case AK_SpeakerVolumeMatrix:
		{
			const auto* info = static_cast<AkSpeakerVolumeMatrixCallbackInfo*>(in_pCallbackInfo);
			pack_base_event_info(callback_data, in_pEventInfo);

			Dictionary input_config;
			input_config["uNumChannels"] = info->inputConfig.uNumChannels;
			input_config["eConfigType"] = info->inputConfig.eConfigType;
			input_config["uChannelMask"] = info->inputConfig.uChannelMask;
			callback_data["inputConfig"] = input_config;

			Dictionary output_config;
			output_config["uNumChannels"] = info->outputConfig.uNumChannels;
			output_config["eConfigType"] = info->outputConfig.eConfigType;
			output_config["uChannelMask"] = info->outputConfig.uChannelMask;
			callback_data["outputConfig"] = output_config;
			break;
		}
		case AK_MIDIEvent:
		{
			const auto* info = static_cast<AkMIDIEventCallbackInfo*>(in_pCallbackInfo);
			pack_base_event_info(callback_data, in_pEventInfo);

			Dictionary midiEvent;
			midiEvent["byType"] = static_cast<unsigned char>(info->midiEvent.byType);
			midiEvent["byChan"] = static_cast<unsigned char>(info->midiEvent.byChan);

			Dictionary cc;
			cc["byCc"] = static_cast<unsigned char>(info->midiEvent.Cc.byCc);
			cc["byValue"] = static_cast<unsigned char>(info->midiEvent.Cc.byValue);

			Dictionary chanAftertouch;
			chanAftertouch["byValue"] = static_cast<unsigned char>(info->midiEvent.ChanAftertouch.byValue);

			Dictionary gen;
			gen["byParam1"] = static_cast<unsigned char>(info->midiEvent.Gen.byParam1);
			gen["byParam2"] = static_cast<unsigned char>(info->midiEvent.Gen.byParam2);

			Dictionary noteAftertouch;
			noteAftertouch["byNote"] = static_cast<unsigned char>(info->midiEvent.NoteAftertouch.byNote);
			noteAftertouch["byValue"] = static_cast<unsigned char>(info->midiEvent.NoteAftertouch.byValue);

			Dictionary noteOnOff;
			noteOnOff["byNote"] = static_cast<unsigned char>(info->midiEvent.NoteOnOff.byNote);
			noteOnOff["byVelocity"] = static_cast<unsigned char>(info->midiEvent.NoteOnOff.byVelocity);

			Dictionary pitchBend;
			pitchBend["byValueLsb"] = static_cast<unsigned char>(info->midiEvent.PitchBend.byValueLsb);
			pitchBend["byValueMsb"] = static_cast<unsigned char>(info->midiEvent.PitchBend.byValueMsb);

			Dictionary programChange;
			programChange["byProgramNum"] = static_cast<unsigned char>(info->midiEvent.ProgramChange.byProgramNum);

			midiEvent["cc"] = cc;
			midiEvent["chanAftertouch"] = chanAftertouch;
			midiEvent["gen"] = gen;
			midiEvent["noteAftertouch"] = noteAftertouch;
			midiEvent["noteOnOff"] = noteOnOff;
			midiEvent["pitchBend"] = pitchBend;
			midiEvent["programChange"] = programChange;
			callback_data["midiEvent"] = midiEvent;
			break;
		}
		default:
			AKASSERT(false);
			return;
	}

	args.push_back(callback_data);

	if (ObjectDB::get_instance(package->object_id) && package->cookie)
	{
		if (in_eType != AK_EndOfEvent || package->notify_end_of_event)
		{
			package->cookie->callv(args);
		}
	}

	if (in_eType == AK_EndOfEvent)
	{
		memdelete(package->cookie);
		delete package;
	}
}

void Wwise::bank_callback(AkUInt32 bank_id, const void* in_memory_bank_ptr, AKRESULT load_result, void* in_pCookie)
{
	AkAutoLock<CAkLock> scoped_lock(callback_data_lock);

	Callable* cookie = static_cast<Callable*>(in_pCookie);

	if (!cookie)
	{
		ERROR_CHECK_MSG(AK_Fail, "WwiseGodot: The Bank Callback cookie is not valid.");
		return;
	}

	Array args = Array();
	Dictionary callback_data;
	callback_data["bank_id"] = bank_id;
	callback_data["result"] = load_result;

	args.push_back(callback_data);
	cookie->callv(args);
	memdelete(cookie);
}

void Wwise::pre_game_object_api_call(Node* p_node, AkGameObjectID p_id)
{
	bool found = registered_game_objects.find(p_id) != registered_game_objects.end();
	if (!found && is_initialized())
	{
		if (!p_node)
		{
			return;
		}

		if (Object::cast_to<Node3D>(p_node))
		{
			if (!p_node->has_node("AkGameObj3D"))
			{
				AkGameObj3D* game_obj_3d = memnew(AkGameObj3D);
				game_obj_3d->set_name("AkGameObj3D");
				p_node->add_child(game_obj_3d);
			}
		}
		else if (Object::cast_to<Node2D>(p_node))
		{
			if (!p_node->has_node("AkGameObj2D"))
			{
				AkGameObj2D* game_obj_2d = memnew(AkGameObj2D);
				game_obj_2d->set_name("AkGameObj2D");
				p_node->add_child(game_obj_2d);
			}
		}
		else
		{
			if (!p_node->has_node("AkGameObj"))
			{
				AkGameObj* game_obj = memnew(AkGameObj);
				game_obj->set_name("AkGameObj");
				p_node->add_child(game_obj);
			}
		}
	}
}

void Wwise::post_register_game_object(AKRESULT p_result, const Node* p_node, AkGameObjectID p_id)
{
	if (p_result == AK_Success)
	{
		registered_game_objects.insert(p_id);
	}
}

void Wwise::post_unregister_game_object(AKRESULT p_result, const Node* p_node, AkGameObjectID p_id)
{
	if (p_result == AK_Success)
	{
		auto it = registered_game_objects.find(p_id);
		if (it != registered_game_objects.end())
		{
			registered_game_objects.remove(it);
		}
	}
}

bool Wwise::initialize_wwise_systems()
{
	WwiseSettings* project_settings = WwiseSettings::get_singleton();

	AkMemSettings mem_settings;
	AK::MemoryMgr::GetDefaultSettings(mem_settings);
	if (!ERROR_CHECK_MSG(AK::MemoryMgr::Init(&mem_settings), "Memory manager initialization failed."))
	{
		return false;
	}

	AkStreamMgrSettings stream_mgr_settings;
	AK::StreamMgr::GetDefaultSettings(stream_mgr_settings);
	if (!AK::StreamMgr::Create(stream_mgr_settings))
	{
		return false;
	}

	AkDeviceSettings device_settings;
	AK::StreamMgr::GetDefaultDeviceSettings(device_settings);

	device_settings.bUseStreamCache =
			static_cast<bool>(project_settings->get_setting(project_settings->advanced_settings.use_stream_cache));

	device_settings.fTargetAutoStmBufferLength = static_cast<float>(
			project_settings->get_setting(project_settings->advanced_settings.target_auto_stream_buffer_length_ms));

	device_settings.uIOMemorySize = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->advanced_settings.io_memory_size));

	device_settings.uMaxCachePinnedBytes = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->advanced_settings.maximum_pinned_bytes_in_cache));

	if (!ERROR_CHECK_MSG(low_level_io.init(device_settings), "Initializing Low level IO failed."))
	{
		return false;
	}

	AkInitSettings init_settings{};
	AK::SoundEngine::GetDefaultInitSettings(init_settings);

#if defined(AK_ENABLE_ASSERTS)
	init_settings.pfnAssertHook = WwiseAssertHook;
#endif

	init_settings.bDebugOutOfRangeCheckEnabled = static_cast<bool>(
			project_settings->get_setting(project_settings->advanced_settings.debug_out_of_range_check_enabled));

	init_settings.bEnableGameSyncPreparation = static_cast<bool>(
			project_settings->get_setting(project_settings->advanced_settings.enable_game_sync_preparation));

	init_settings.fStreamingLookAheadRatio = static_cast<float>(
			project_settings->get_setting(project_settings->common_user_settings.streaming_look_ahead_ratio));

	init_settings.fDebugOutOfRangeLimit = static_cast<float>(
			project_settings->get_setting(project_settings->advanced_settings.debug_out_of_range_limit));

	String audioDeviceShareSet =
			project_settings->get_setting(project_settings->common_user_settings.main_output.audio_device_shareset);
	init_settings.settingsMainOutput.audioDeviceShareset =
			AK::SoundEngine::GetIDFromString(audioDeviceShareSet.utf8().get_data());

	const unsigned int channelConfigType = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->common_user_settings.main_output.channel_config_type));

	const unsigned int numChannels = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->common_user_settings.main_output.number_of_channels));

	if (channelConfigType == AK_ChannelConfigType_Anonymous)
	{
		init_settings.settingsMainOutput.channelConfig.SetAnonymous(numChannels);
	}
	else if (channelConfigType == AK_ChannelConfigType_Standard)
	{
		init_settings.settingsMainOutput.channelConfig.SetStandard(numChannels);
	}
	else if (channelConfigType == AK_ChannelConfigType_Ambisonic)
	{
		init_settings.settingsMainOutput.channelConfig.SetAmbisonic(numChannels);
	}
	else
	{
		AKASSERT(false);
	}

	init_settings.settingsMainOutput.idDevice = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->common_user_settings.main_output.device_id));

	init_settings.settingsMainOutput.ePanningRule = static_cast<AkPanningRule>(static_cast<unsigned int>(
			project_settings->get_setting(project_settings->common_user_settings.main_output.panning_rule)));

	init_settings.uCommandQueueSize = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->common_user_settings.command_queue_size));

	init_settings.uContinuousPlaybackLookAhead = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->advanced_settings.continuous_playback_look_ahead));

	init_settings.uMaxHardwareTimeoutMs = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->advanced_settings.maximum_hardware_timeout_ms));

	init_settings.uMaxNumPaths = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->common_user_settings.maximum_number_of_positioning_paths));

	init_settings.uMonitorQueuePoolSize = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->advanced_settings.monitor_queue_pool_size));

	const unsigned int num_samples_per_frame_enum = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->common_user_settings.samples_per_frame));

	switch (num_samples_per_frame_enum)
	{
		case SamplesPerFrame::SAMPLES_256:
			init_settings.uNumSamplesPerFrame = 256;
			break;
		case SamplesPerFrame::SAMPLES_512:
			init_settings.uNumSamplesPerFrame = 512;
			break;
		case SamplesPerFrame::SAMPLES_1024:
			init_settings.uNumSamplesPerFrame = 1024;
			break;
		case SamplesPerFrame::SAMPLES_2048:
			init_settings.uNumSamplesPerFrame = 2048;
			break;
		default:
			AKASSERT(false);
			break;
	}

	init_settings.fGameUnitsToMeters = static_cast<float>(
			project_settings->get_setting(project_settings->common_user_settings.game_units_to_meters));

	init_settings.eFloorPlane = AkFloorPlane_Default;

	AkPlatformInitSettings platform_init_settings;
	AK::SoundEngine::GetDefaultPlatformInitSettings(platform_init_settings);

	// Common platform settings
	const unsigned int num_refills_in_voice_enum = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->common_user_settings.number_of_refills_in_voice));

	switch (num_refills_in_voice_enum)
	{
		case NumRefillsInVoice::REFILLS_2:
			platform_init_settings.uNumRefillsInVoice = 2;
			break;
		case NumRefillsInVoice::REFILLS_4:
			platform_init_settings.uNumRefillsInVoice = 4;
			break;
		default:
			AKASSERT(false);
			break;
	}

	const unsigned int samplerate_enum = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->common_user_settings.sample_rate));

	switch (samplerate_enum)
	{
		case SampleRate::RATE_16000:
			platform_init_settings.uSampleRate = 16000;
			break;
		case SampleRate::RATE_24000:
			platform_init_settings.uSampleRate = 24000;
			break;
		case SampleRate::RATE_32000:
			platform_init_settings.uSampleRate = 32000;
			break;
		case SampleRate::RATE_44100:
			platform_init_settings.uSampleRate = 44100;
			break;
		case SampleRate::RATE_48000:
			platform_init_settings.uSampleRate = 48000;
			break;
		default:
			AKASSERT(false);
			break;
	}

#if defined(TOOLS_ENABLED)
	String dsp_path = AkEditorSettings::get_editor_plugins_path();
	AkOSChar* dll_path;
	CONVERT_CHAR_TO_OSCHAR(ProjectSettings::get_singleton()->globalize_path(dsp_path).utf8().get_data(), dll_path);
	init_settings.szPluginDLLPath = dll_path;
#endif

	// Platform-specific settings
#ifdef AK_WIN
	platform_init_settings.uMaxSystemAudioObjects = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->platform_settings.windows_max_system_audio_objects));

#elif defined(AK_MAC_OS_X)
	platform_init_settings.eAudioAPI = static_cast<AkAudioAPIMac>(static_cast<unsigned int>(
			project_settings->get_setting(project_settings->platform_settings.macos_audio_api)));

#elif defined(AK_IOS)
	platform_init_settings.eAudioAPI = static_cast<AkAudioAPIiOS>(static_cast<unsigned int>(
			project_settings->get_setting(project_settings->platform_settings.ios_audio_api)));

	const unsigned int session_category_enum = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->platform_settings.ios_audio_session_category));

	platform_init_settings.audioSession.eCategory = static_cast<AkAudioSessionCategory>(session_category_enum);

	const unsigned int session_category_flags = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->platform_settings.ios_audio_session_category_options));

	platform_init_settings.audioSession.eCategoryOptions =
			static_cast<AkAudioSessionCategoryOptions>(session_category_flags);

	const unsigned int audio_session_mode_enum = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->platform_settings.ios_audio_session_mode));

	platform_init_settings.audioSession.eMode = static_cast<AkAudioSessionMode>(audio_session_mode_enum);

#elif defined(AK_ANDROID)
	AkAudioAPIAndroid android_api_flag = AkAudioAPI_Default;

	int selected_api_index =
			static_cast<int>(project_settings->get_setting(project_settings->platform_settings.android_audio_api));

	switch (selected_api_index)
	{
		case 0:
			android_api_flag = AkAudioAPI_AAudio;
			break;
		case 1:
			android_api_flag = AkAudioAPI_OpenSL_ES;
			break;
		case 2:
			android_api_flag = AkAudioAPI_DolbyAtmos;
			break;
		case 3:
			android_api_flag = AkAudioAPI_AndroidSpatializer;
			break;
		case 4:
			android_api_flag = AkAudioAPI_Default;
			break;
		default:
			android_api_flag = AkAudioAPI_Default;
			break;
	}

	platform_init_settings.eAudioAPI = android_api_flag;

	platform_init_settings.pJavaVM = JNISupport::getJavaVM();
	platform_init_settings.jActivity = JNISupport::getActivity();

	AkOSChar* dll_path;
	CONVERT_CHAR_TO_OSCHAR(JNISupport::getPluginsDir().c_str(), dll_path);
	init_settings.szPluginDLLPath = dll_path;

	// todo(afama): Expose setting
	platform_init_settings.eAudioPath = AkAudioPath_LowLatency;

#elif defined(AK_LINUX)

	platform_init_settings.eAudioAPI = static_cast<AkAudioAPILinux>(static_cast<unsigned int>(
			project_settings->get_setting(project_settings->platform_settings.linux_audio_api)));

#else
#error "Platform not supported"
#endif

	if (!ERROR_CHECK_MSG(AK::SoundEngine::Init(&init_settings, &platform_init_settings),
				"WwiseGodot: Sound engine initialization failed."))

	{
		return false;
	}

	AkSpatialAudioInitSettings spatialSettings;

	spatialSettings.uMaxSoundPropagationDepth = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->spatial_audio_settings.max_sound_propagation_depth));

	spatialSettings.fMovementThreshold = static_cast<AkReal32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.movement_threshold));

	spatialSettings.uNumberOfPrimaryRays = static_cast<AkUInt32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.number_of_primary_rays));

	spatialSettings.uMaxReflectionOrder = static_cast<AkUInt32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.max_reflection_order));

	spatialSettings.uMaxDiffractionOrder = static_cast<AkUInt32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.max_diffraction_order));

	spatialSettings.uMaxDiffractionPaths = static_cast<AkUInt32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.max_diffraction_paths));

	spatialSettings.uMaxGlobalReflectionPaths = static_cast<AkUInt32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.max_global_reflection_paths));

	spatialSettings.uMaxEmitterRoomAuxSends = static_cast<AkUInt32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.max_emitter_room_aux_sends));

	spatialSettings.uDiffractionOnReflectionsOrder = static_cast<AkUInt32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.diffraction_on_reflections_order));

	spatialSettings.fMaxDiffractionAngleDegrees = static_cast<AkReal32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.max_diffraction_angle_degrees));

	spatialSettings.fMaxPathLength = static_cast<AkReal32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.max_path_length));

	spatialSettings.fCPULimitPercentage = static_cast<AkReal32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.cpu_limit_percentage));

	spatialSettings.fSmoothingConstantMs = static_cast<AkReal32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.smoothing_constant_ms));

	spatialSettings.uLoadBalancingSpread = static_cast<AkUInt32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.load_balancing_spread));

	spatialSettings.bEnableGeometricDiffractionAndTransmission = static_cast<bool>(project_settings->get_setting(
			project_settings->spatial_audio_settings.enable_geometric_diffraction_and_transmission));

	spatialSettings.bCalcEmitterVirtualPosition = static_cast<bool>(
			project_settings->get_setting(project_settings->spatial_audio_settings.calc_emitter_virtual_position));

	spatialSettings.eTransmissionOperation = (AkTransmissionOperation) static_cast<AkUInt32>(
			project_settings->get_setting(project_settings->spatial_audio_settings.transmission_operation));

	if (!ERROR_CHECK_MSG(AK::SpatialAudio::Init(spatialSettings), "WwiseGodot: Spatial Audio initialization failed."))
	{
		return false;
	}

#ifndef AK_OPTIMIZED
	AkCommSettings comm_settings{};
	AK::Comm::GetDefaultInitSettings(comm_settings);

	comm_settings.bInitSystemLib = static_cast<bool>(
			project_settings->get_setting(project_settings->communication_settings.initialize_system_comms));

	comm_settings.ports.uCommand = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->communication_settings.command_port));

	comm_settings.ports.uDiscoveryBroadcast = static_cast<unsigned int>(
			project_settings->get_setting(project_settings->communication_settings.discovery_broadcast_port));

	String network_name = project_settings->get_setting(project_settings->communication_settings.network_name);
	AKPLATFORM::SafeStrCpy(
			comm_settings.szAppNetworkName, network_name.utf8().get_data(), AK_COMM_SETTINGS_MAX_STRING_SIZE);

	ERROR_CHECK_MSG(AK::Comm::Init(comm_settings), "WwiseGodot: Comm initialization failed.");
#endif

	return true;
}

bool Wwise::shutdown_wwise_system()
{
#ifndef AK_OPTIMIZED
	AK::Comm::Term();
#endif

	if (!ERROR_CHECK(AK::SoundEngine::UnregisterAllGameObj()))
	{
		return false;
	}

	if (!ERROR_CHECK(AK::SoundEngine::ClearBanks()))
	{
		return false;
	}

	AK::SoundEngine::Term();

	low_level_io.term();

	if (AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}

	AK::MemoryMgr::Term();

	AkBankManager::reset();

	return true;
}
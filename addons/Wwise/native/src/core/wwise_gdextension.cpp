#include "wwise_gdextension.h"

#if defined(AK_ANDROID)
#include "platform/android/jni_support.cpp"
#endif

#include <AK/Plugin/AllPluginsFactories.h>

#if defined(AK_REFLECT)
#include <AK/Plugin/AkReflectFXFactory.h>
#endif

#if defined(AK_MOTION)
#include <AK/Plugin/AkMotionSinkFactory.h>
#include <AK/Plugin/AkMotionSourceSourceFactory.h>
#endif

#if defined(AK_CONVOLUTION)
#include <AK/Plugin/AkConvolutionReverbFXFactory.h>
#endif

#if defined(AK_SOUNDSEED_GRAIN)
#include <AK/Plugin/AkSoundSeedGrainSourceFactory.h>
#endif

#if defined(AK_SOUNDSEED_AIR)
#include <AK/Plugin/AkSoundSeedWindSourceFactory.h>
#include <AK/Plugin/AkSoundSeedWooshSourceFactory.h>
#endif

#if defined(AK_IMPACTER)
#include <AK/Plugin/AkImpacterSourceFactory.h>
#endif

#if defined(AK_MASTERING_SUITE)
#include <AK/Plugin/MasteringSuiteFXFactory.h>
#endif

Wwise* Wwise::singleton = nullptr;
CAkLock Wwise::callback_data_lock;
CAkLock g_localOutputLock;

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
	ClassDB::bind_method(D_METHOD("set_base_path", "base_path"), &Wwise::set_base_path);
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
	ClassDB::bind_method(
			D_METHOD("set_distance_probe", "listener_game_object", "probe_game_object"), &Wwise::set_distance_probe);
	ClassDB::bind_method(D_METHOD("reset_distance_probe", "listener_game_object"), &Wwise::reset_distance_probe);
	ClassDB::bind_method(D_METHOD("set_listeners", "emtter", "listener"), &Wwise::set_listeners);
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
	ClassDB::bind_method(D_METHOD("stop_event", "playing_id", "fade_time", "interpolation"), &Wwise::stop_event);
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
	ClassDB::bind_method(D_METHOD("is_initialized"), &Wwise::is_initialized);
}

void Wwise::init()
{
	project_settings = ProjectSettings::get_singleton();
	AKASSERT(project_settings);

	bool result = initialize_wwise_systems();

	if (!result)
	{
		ERROR_CHECK_MSG(AK_Fail, "WwiseGodot: Sound engine initialization failed!");
		return;
	}
	else
	{
		UtilityFunctions::print("WwiseGodot: Sound engine initialized successfully.");
		UtilityFunctions::print(vformat("WwiseGodot: Wwise(R) SDK %s Build %d.", AK_WWISESDK_VERSIONNAME_SHORT, AK_WWISESDK_VERSION_BUILD));
	}

	String base_path = get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "base_path");

#ifdef AK_WIN
	String banks_platform_suffix = "/Windows";
#elif defined(AK_MAC_OS_X)
	String banks_platform_suffix = "/Mac";
#elif defined(AK_IOS)
	String banks_platform_suffix = "/iOS";
#elif defined(AK_ANDROID)
	String banks_platform_suffix = "/Android";
#elif defined(AK_LINUX)
	String banks_platform_suffix = "/Linux";
#else
#error "Platform not supported"
#endif

	base_path += banks_platform_suffix + "/";

	bool set_base_path_result = set_base_path(base_path);
	AKASSERT(set_base_path_result);

	String startup_language = get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "startup_language");

	set_current_language(startup_language);

#if !defined(AK_OPTIMIZED)
	const bool engineLogging =
			static_cast<bool>(get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "engine_logging"));

	if (engineLogging)
	{
		ERROR_CHECK_MSG(AK::Monitor::SetLocalOutput(
								AK::Monitor::ErrorLevel_All, static_cast<AK::Monitor::LocalOutputFunc>(LocalOutput)),
				"Setting local output to ErrorLevel_All failed.");
	}
#endif

	bool load_init_bank_at_startup =
			get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "load_init_bank_at_startup");

	bool use_soundbank_names = get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "use_soundbank_names");

	if (load_init_bank_at_startup)
	{
		if (use_soundbank_names)
		{
			String init_bank_name = "Init";
			load_bank(init_bank_name);
		}
		else
		{
			AkUInt32 initBankID = AK::SoundEngine::GetIDFromString("Init");
			load_bank_id(initBankID);
		}
	}
}

void Wwise::render_audio() { ERROR_CHECK(AK::SoundEngine::RenderAudio()); }

void Wwise::shutdown()
{
	if (shutdown_wwise_system())
	{
		UtilityFunctions::print("WwiseGodot: Sound engine terminated successfully.");
	}
}

bool Wwise::set_base_path(const String& base_path)
{
	AKASSERT(!base_path.is_empty());

	low_level_io.set_banks_path(base_path);

	return true;
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

bool Wwise::load_bank_async(const String& bank_name, const WwiseCookie* cookie)
{
	AKASSERT(!bank_name.is_empty());
	AKASSERT(cookie);

	AkBankID bank_id{};
	return ERROR_CHECK(AK::SoundEngine::LoadBank(
			bank_name.utf8().get_data(), (AkBankCallbackFunc)bank_callback, (void*)cookie, bank_id));
}

bool Wwise::load_bank_async_id(const unsigned int bank_id, const WwiseCookie* cookie)
{
	AKASSERT(cookie);

	return ERROR_CHECK(AK::SoundEngine::LoadBank(bank_id, (AkBankCallbackFunc)bank_callback, (void*)cookie));
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

bool Wwise::unload_bank_async(const String& bank_name, const WwiseCookie* cookie)
{
	AKASSERT(!bank_name.is_empty());
	AKASSERT(cookie);

	return ERROR_CHECK(AK::SoundEngine::UnloadBank(
			bank_name.utf8().get_data(), NULL, (AkBankCallbackFunc)bank_callback, (void*)cookie));
}

bool Wwise::unload_bank_async_id(const unsigned int bank_id, const WwiseCookie* cookie)
{
	AKASSERT(cookie);

	return ERROR_CHECK(AK::SoundEngine::UnloadBank(bank_id, NULL, (AkBankCallbackFunc)bank_callback, (void*)cookie));
}

bool Wwise::register_listener(const Object* game_object)
{
	AKASSERT(game_object);

	const AkGameObjectID listener = static_cast<AkGameObjectID>(game_object->get_instance_id());

	if (!ERROR_CHECK(AK::SoundEngine::RegisterGameObj(listener, "Default Listener")))
	{
		return false;
	}

	if (!ERROR_CHECK(AK::SoundEngine::SetDefaultListeners(&listener, 1)))
	{
		return false;
	}

	return true;
}

bool Wwise::register_game_obj(const Object* game_object, const String& game_object_name)
{
	AKASSERT(game_object);
	AKASSERT(!game_object_name.is_empty());

	return ERROR_CHECK_MSG(AK::SoundEngine::RegisterGameObj(static_cast<AkGameObjectID>(game_object->get_instance_id()),
								   game_object_name.utf8().get_data()),
			vformat("Failed to register Game Object with name: %s.", game_object_name));
}

bool Wwise::unregister_game_obj(const Object* game_object)
{
	AKASSERT(game_object);

	return ERROR_CHECK(AK::SoundEngine::UnregisterGameObj(static_cast<AkGameObjectID>(game_object->get_instance_id())));
}

bool Wwise::set_distance_probe(const Object* listener_game_object, const Object* probe_game_object)
{
	AKASSERT(listener_game_object);
	AKASSERT(probe_game_object);

	const AkGameObjectID listener = static_cast<AkGameObjectID>(listener_game_object->get_instance_id());
	const AkGameObjectID probe = static_cast<AkGameObjectID>(probe_game_object->get_instance_id());

	if (!ERROR_CHECK(AK::SoundEngine::SetDistanceProbe(listener, probe)))
	{
		return false;
	}

	return true;
}

bool Wwise::reset_distance_probe(const Object* listener_game_object)
{
	AKASSERT(listener_game_object);
	const AkGameObjectID listener = static_cast<AkGameObjectID>(listener_game_object->get_instance_id());
	if (!ERROR_CHECK(AK::SoundEngine::SetDistanceProbe(listener, AK_INVALID_GAME_OBJECT)))
	{
		return false;
	}

	return true;
}

bool Wwise::set_listeners(const Object* emitter, const Object* listener)
{
	AKASSERT(emitter);
	AKASSERT(listener);

	static const int num_listeners_for_emitter = 1;
	static const AkGameObjectID listeners_for_emitter[num_listeners_for_emitter] = { static_cast<AkGameObjectID>(
			listener->get_instance_id()) };
	return ERROR_CHECK(AK::SoundEngine::SetListeners(
			static_cast<AkGameObjectID>(emitter->get_instance_id()), listeners_for_emitter, 1));
}

void Wwise::set_random_seed(const unsigned int seed) { AK::SoundEngine::SetRandomSeed(seed); }

bool Wwise::set_3d_position(const Object* game_object, const Transform3D& transform_3d)
{
	AKASSERT(game_object);

	AkSoundPosition sound_pos{};

	AkVector position{};
	get_akvector(transform_3d, position, VectorType::POSITION);
	AkVector forward{};
	get_akvector(transform_3d, forward, VectorType::FORWARD);
	AkVector up{};
	get_akvector(transform_3d, up, VectorType::UP);

	sound_pos.Set(AK::ConvertAkVectorToAkVector64(position), forward, up);

	return ERROR_CHECK(
			AK::SoundEngine::SetPosition(static_cast<AkGameObjectID>(game_object->get_instance_id()), sound_pos));
}

bool Wwise::set_2d_position(const Object* game_object, const Transform2D& transform_2d, const float z_depth)
{
	AKASSERT(game_object);

	AkSoundPosition sound_pos{};

	Vector2 origin = transform_2d.get_origin();

	Vector3 position = Vector3(origin.x, origin.y, z_depth);
	Vector3 forward = Vector3(transform_2d.columns[1].x, 0.0f, transform_2d.columns[1].y).normalized();
	Vector3 up = Vector3(0, -1, 0);

	AkVector ak_position{};
	vector3_to_akvector(position, ak_position);
	AkVector ak_forward{};
	vector3_to_akvector(forward, ak_forward);
	AkVector ak_up{};
	vector3_to_akvector(up, ak_up);

	sound_pos.Set(AK::ConvertAkVectorToAkVector64(ak_position), ak_forward, ak_up);

	return ERROR_CHECK(
			AK::SoundEngine::SetPosition(static_cast<AkGameObjectID>(game_object->get_instance_id()), sound_pos));
}

bool Wwise::set_multiple_positions_3d(const Object* game_object, const TypedArray<Transform3D>& positions,
		const int num_positions, const AkUtils::MultiPositionType multi_position_type)
{
	AKASSERT(game_object);
	AKASSERT(positions.size() > 0);
	AKASSERT(positions.size() == num_positions);

	auto ak_positions = std::make_unique<AkSoundPosition[]>(num_positions);

	for (int i = 0; i < positions.size(); i++)
	{
		Transform3D transform = positions[i];

		AkVector position{};
		get_akvector(transform, position, VectorType::POSITION);
		AkVector forward{};
		get_akvector(transform, forward, VectorType::FORWARD);
		AkVector up{};
		get_akvector(transform, up, VectorType::UP);

		ak_positions[i].Set(AK::ConvertAkVectorToAkVector64(position), forward, up);
	}

	return ERROR_CHECK(AK::SoundEngine::SetMultiplePositions(game_object->get_instance_id(), ak_positions.get(),
			num_positions, static_cast<AK::SoundEngine::MultiPositionType>(multi_position_type)));
}

bool Wwise::set_multiple_positions_2d(const Object* game_object, const TypedArray<Transform2D>& positions,
		const TypedArray<float>& z_depths, const int num_positions,
		const AkUtils::MultiPositionType multi_position_type)
{
	AKASSERT(game_object);
	AKASSERT(positions.size() > 0);
	AKASSERT(positions.size() == num_positions);
	AKASSERT(z_depths.size() == num_positions);

	auto ak_positions = std::make_unique<AkSoundPosition[]>(num_positions);

	for (int i = 0; i < positions.size(); i++)
	{
		Transform2D transform_2d = positions[i];

		Vector2 origin = transform_2d.get_origin();

		Vector3 position = Vector3(origin.x, origin.y, z_depths[i]);
		Vector3 forward = Vector3(transform_2d.columns[1].x, 0, transform_2d.columns[1].y).normalized();
		Vector3 up = Vector3(0, 1, 0);

		AkVector ak_position{};
		vector3_to_akvector(position, ak_position);
		AkVector ak_forward{};
		vector3_to_akvector(forward, ak_forward);
		AkVector ak_up{};
		vector3_to_akvector(up, ak_up);

		ak_positions[i].Set(AK::ConvertAkVectorToAkVector64(ak_position), ak_forward, ak_up);
	}

	return ERROR_CHECK(AK::SoundEngine::SetMultiplePositions(game_object->get_instance_id(), ak_positions.get(),
			num_positions, static_cast<AK::SoundEngine::MultiPositionType>(multi_position_type)));
}

bool Wwise::set_game_object_radius(const Object* game_object, const float outer_radius, const float inner_radius)
{
	AKASSERT(game_object);

	return ERROR_CHECK(
			AK::SpatialAudio::SetGameObjectRadius(game_object->get_instance_id(), outer_radius, inner_radius));
}

unsigned int Wwise::post_event(const String& event_name, const Object* game_object)
{
	AKASSERT(!event_name.is_empty());
	AKASSERT(game_object);

	AkPlayingID playing_id = AK::SoundEngine::PostEvent(
			event_name.utf8().get_data(), static_cast<AkGameObjectID>(game_object->get_instance_id()));

	if (playing_id == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK_MSG(AK_InvalidID,
				vformat("Failed to post Event: %s on Game Object: %s.", event_name,
						itos(game_object->get_instance_id())));
	}

	return playing_id;
}

unsigned int Wwise::post_event_callback(const String& event_name, const AkUtils::AkCallbackType flags,
		const Object* game_object, const WwiseCookie* cookie)
{
	AKASSERT(!event_name.is_empty());
	AKASSERT(game_object);
	AKASSERT(cookie);

	AkPlayingID playing_id = AK::SoundEngine::PostEvent(event_name.utf8().get_data(),
			static_cast<AkGameObjectID>(game_object->get_instance_id()), flags, event_callback, (void*)cookie);

	if (playing_id == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK_MSG(AK_InvalidID,
				vformat("Failed to post Event: %s on Game Object: %s.", event_name,
						itos(game_object->get_instance_id())));
	}

	return playing_id;
}

unsigned int Wwise::post_event_id(const unsigned int event_id, const Object* game_object)
{
	AKASSERT(game_object);

	AkPlayingID playing_id =
			AK::SoundEngine::PostEvent(event_id, static_cast<AkGameObjectID>(game_object->get_instance_id()));

	if (playing_id == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK_MSG(AK_InvalidID,
				vformat("Failed to post Event with ID: %d on Game Object: %s.", event_id,
						itos(game_object->get_instance_id())));
	}

	return playing_id;
}

unsigned int Wwise::post_event_id_callback(const unsigned int event_id, const AkUtils::AkCallbackType flags,
		const Object* game_object, const WwiseCookie* cookie)
{
	AKASSERT(game_object);
	AKASSERT(cookie);

	AkPlayingID playing_id = AK::SoundEngine::PostEvent(event_id,
			static_cast<AkGameObjectID>(game_object->get_instance_id()), flags, event_callback, (void*)cookie);

	if (playing_id == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK_MSG(AK_InvalidID,
				vformat("Failed to post Event with ID: %d on Game Object: %s.", event_id,
						itos(game_object->get_instance_id())));
	}

	return playing_id;
}

void Wwise::stop_event(
		const unsigned int playing_id, const unsigned int fade_time, const AkUtils::AkCurveInterpolation interpolation)
{
	AK::SoundEngine::ExecuteActionOnPlayingID(AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Stop,
			static_cast<AkPlayingID>(playing_id), static_cast<AkTimeMs>(fade_time),
			static_cast<AkCurveInterpolation>(interpolation));
}

bool Wwise::set_switch(const String& switch_group, const String& switch_value, const Object* game_object)
{
	AKASSERT(!switch_group.is_empty());
	AKASSERT(!switch_value.is_empty());
	AKASSERT(game_object);

	return ERROR_CHECK(AK::SoundEngine::SetSwitch(switch_group.utf8().get_data(), switch_value.utf8().get_data(),
			static_cast<AkGameObjectID>(game_object->get_instance_id())));
}

bool Wwise::set_switch_id(
		const unsigned int switch_group_id, const unsigned int switch_value_id, const Object* game_object)
{
	AKASSERT(game_object);

	return ERROR_CHECK(AK::SoundEngine::SetSwitch(
			switch_group_id, switch_value_id, static_cast<AkGameObjectID>(game_object->get_instance_id())));
}

bool Wwise::set_state(const String& state_group, const String& state_value)
{
	AKASSERT(!state_group.is_empty());
	AKASSERT(!state_value.is_empty());

	return ERROR_CHECK(AK::SoundEngine::SetState(state_group.utf8().get_data(), state_value.utf8().get_data()));
}

bool Wwise::set_state_id(const unsigned int state_group_id, const unsigned int state_value_id)
{
	return ERROR_CHECK(AK::SoundEngine::SetState(state_group_id, state_value_id));
}

float Wwise::get_rtpc_value(const String& rtpc_name, const Object* game_object)
{
	AKASSERT(!rtpc_name.is_empty());
	AkRtpcValue value;
	AK::SoundEngine::Query::RTPCValue_type type = AK::SoundEngine::Query::RTPCValue_Default;
	AkGameObjectID game_object_id = AK_INVALID_GAME_OBJECT;

	if (game_object)
	{
		type = AK::SoundEngine::Query::RTPCValue_GameObject;
		game_object_id = static_cast<AkGameObjectID>(game_object->get_instance_id());
	}
	else
	{
		type = AK::SoundEngine::Query::RTPCValue_Global;
		game_object_id = AK_INVALID_GAME_OBJECT;
	}

	if (!ERROR_CHECK(AK::SoundEngine::Query::GetRTPCValue(
				rtpc_name.utf8().get_data(), game_object_id, static_cast<AkPlayingID>(0), value, type)))
	{
		return INVALID_RTPC_VALUE;
	}

	return static_cast<float>(value);
}

float Wwise::get_rtpc_value_id(const unsigned int rtpc_id, const Object* game_object)
{
	AkRtpcValue value;
	AK::SoundEngine::Query::RTPCValue_type type = AK::SoundEngine::Query::RTPCValue_Default;
	AkGameObjectID game_object_id = AK_INVALID_GAME_OBJECT;

	if (game_object)
	{
		type = AK::SoundEngine::Query::RTPCValue_GameObject;
		game_object_id = static_cast<AkGameObjectID>(game_object->get_instance_id());
	}
	else
	{
		type = AK::SoundEngine::Query::RTPCValue_Global;
		game_object_id = AK_INVALID_GAME_OBJECT;
	}

	if (!ERROR_CHECK(AK::SoundEngine::Query::GetRTPCValue(
				rtpc_id, game_object_id, static_cast<AkPlayingID>(0), value, type)))
	{
		return INVALID_RTPC_VALUE;
	}

	return static_cast<float>(value);
}

bool Wwise::set_rtpc_value(const String& rtpc_name, const float rtpc_value, const Object* game_object)
{
	AKASSERT(!rtpc_name.is_empty());

	AkGameObjectID game_object_id{ AK_INVALID_GAME_OBJECT };

	if (game_object)
	{
		game_object_id = static_cast<AkGameObjectID>(game_object->get_instance_id());
	}

	return ERROR_CHECK(AK::SoundEngine::SetRTPCValue(
			rtpc_name.utf8().get_data(), static_cast<AkRtpcValue>(rtpc_value), game_object_id));
}

bool Wwise::set_rtpc_value_id(const unsigned int rtpc_id, const float rtpc_value, const Object* game_object)
{
	AkGameObjectID game_object_id{ AK_INVALID_GAME_OBJECT };

	if (game_object)
	{
		game_object_id = static_cast<AkGameObjectID>(game_object->get_instance_id());
	}

	return ERROR_CHECK(AK::SoundEngine::SetRTPCValue(rtpc_id, static_cast<AkRtpcValue>(rtpc_value), game_object_id));
}

bool Wwise::post_trigger(const String& trigger_name, const Object* game_object)
{
	AKASSERT(!trigger_name.is_empty());
	AKASSERT(game_object);

	return ERROR_CHECK(AK::SoundEngine::PostTrigger(
			trigger_name.utf8().get_data(), static_cast<AkGameObjectID>(game_object->get_instance_id())));
}

bool Wwise::post_trigger_id(const unsigned int trigger_id, const Object* game_object)
{
	AKASSERT(game_object);

	return ERROR_CHECK(
			AK::SoundEngine::PostTrigger(trigger_id, static_cast<AkGameObjectID>(game_object->get_instance_id())));
}

unsigned int Wwise::post_external_source(const String& event_name, const Object* game_object,
		const String& source_object_name, const String& filename, const AkUtils::AkCodecID id_codec)
{
	AKASSERT(!event_name.is_empty());
	AKASSERT(game_object);
	AKASSERT(!source_object_name.is_empty());
	AKASSERT(!filename.is_empty());

	AkExternalSourceInfo source{};
	source.iExternalSrcCookie = AK::SoundEngine::GetIDFromString(source_object_name.utf8().get_data());

	AkOSChar* sz_file_os_string = nullptr;
	CONVERT_CHAR_TO_OSCHAR(filename.utf8().get_data(), sz_file_os_string);

	source.szFile = sz_file_os_string;
	source.idCodec = id_codec;

	AkPlayingID playing_id = AK::SoundEngine::PostEvent(event_name.utf8().get_data(),
			static_cast<AkGameObjectID>(game_object->get_instance_id()), 0, nullptr, 0, 1, &source);

	if (playing_id == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK_MSG(AK_InvalidID,
				vformat("Failed to post External Source with Event: %s on Game Object: %s", event_name,
						itos(game_object->get_instance_id())));
		return AK_INVALID_PLAYING_ID;
	}

	return playing_id;
}

unsigned int Wwise::post_external_source_id(const unsigned int event_id, const Object* game_object,
		const unsigned int source_object_id, const String& filename, const AkUtils::AkCodecID id_codec)
{
	AKASSERT(game_object);
	AKASSERT(!filename.is_empty());

	AkExternalSourceInfo source{};
	source.iExternalSrcCookie = source_object_id;

	AkOSChar* sz_file_os_string = nullptr;
	CONVERT_CHAR_TO_OSCHAR(filename.utf8().get_data(), sz_file_os_string);

	source.szFile = sz_file_os_string;
	source.idCodec = id_codec;

	AkPlayingID playing_id = AK::SoundEngine::PostEvent(
			event_id, static_cast<AkGameObjectID>(game_object->get_instance_id()), 0, NULL, 0, 1, &source);

	if (playing_id == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK_MSG(AK_InvalidID,
				vformat("Failed to post External Source with Event ID: %d on Game Object: %s", event_id,
						itos(game_object->get_instance_id())));
		return static_cast<unsigned int>(AK_INVALID_PLAYING_ID);
	}

	return playing_id;
}

int Wwise::get_source_play_position(const unsigned int playing_id, const bool extrapolate)
{
	AkTimeMs position{};
	AKRESULT result =
			AK::SoundEngine::GetSourcePlayPosition(static_cast<AkPlayingID>(playing_id), &position, extrapolate);

	if (result == AK_Fail)
	{
		ERROR_CHECK(result);
		return static_cast<int>(AK_INVALID_PLAYING_ID);
	}

	return position;
}

Dictionary Wwise::get_playing_segment_info(const unsigned int playing_id, const bool extrapolate)
{
	AkSegmentInfo segment_info;
	AKRESULT result =
			AK::MusicEngine::GetPlayingSegmentInfo(static_cast<AkPlayingID>(playing_id), segment_info, extrapolate);

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

bool Wwise::set_game_object_output_bus_volume(const Object* game_object, const Object* listener, float f_control_value)
{
	AkGameObjectID listener_obj_id{ AK_INVALID_GAME_OBJECT };

	if (listener)
	{
		listener_obj_id = static_cast<AkGameObjectID>(listener->get_instance_id());
	}

	return ERROR_CHECK(AK::SoundEngine::SetGameObjectOutputBusVolume(
			static_cast<AkGameObjectID>(game_object->get_instance_id()), listener_obj_id, f_control_value));
}

bool Wwise::set_game_object_aux_send_values(
		const Object* game_object, const Array ak_aux_send_values, const unsigned int num_send_values)
{
	std::vector<AkAuxSendValue> environments;

	for (int i = 0; i < ak_aux_send_values.size(); i++)
	{
		Dictionary aux_bus_data = ak_aux_send_values[i];
		AkAuxSendValue environment{};
		environment.auxBusID = static_cast<unsigned int>(aux_bus_data["aux_bus_id"]);
		environment.fControlValue = static_cast<float>(aux_bus_data["control_value"]);
		environment.listenerID = AK_INVALID_GAME_OBJECT;
		environments.push_back(environment);
	}

	environments.resize(AK_MAX_ENVIRONMENTS);

	return ERROR_CHECK(AK::SoundEngine::SetGameObjectAuxSendValues(
			static_cast<AkGameObjectID>(game_object->get_instance_id()), environments.data(), num_send_values));
}

bool Wwise::set_object_obstruction_and_occlusion(
		const Object* game_object, const Object* listener, float f_calculated_obs, float f_calculated_occ)
{
	return ERROR_CHECK(AK::SoundEngine::SetObjectObstructionAndOcclusion(
			static_cast<AkGameObjectID>(game_object->get_instance_id()),
			static_cast<AkGameObjectID>(listener->get_instance_id()), f_calculated_obs, f_calculated_occ));
}

bool Wwise::set_geometry(const Array vertices, const Array triangles, const Dictionary& acoustic_texture,
		float transmission_loss_value, const Object* game_object, bool enable_diffraction,
		bool enable_diffraction_on_boundary_edges)
{
	AKASSERT(!vertices.is_empty());
	AKASSERT(!triangles.is_empty());
	AKASSERT(game_object);

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

	int num_triangles = triangles.size() / 3;

	if ((num_triangles % 3) != 0)
	{
		UtilityFunctions::print(
				"Wrong number of triangles on mesh {0}", String::num_int64(game_object->get_instance_id()));
	}

	auto ak_triangles = std::make_unique<AkTriangle[]>(num_triangles);

	AkAcousticSurface surface{};
	String texture_name = acoustic_texture.get("name", "");
	AkUInt32 texture_id = acoustic_texture.get("id", AK_INVALID_SURFACE);

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
		t.surface = texture_valid ? 0 : AK_INVALID_SURFACE;

		ak_triangles[triangleIdx] = t;

		if (t.point0 != t.point1 && t.point0 != t.point2 && t.point1 != t.point2)
		{
			++triangleIdx;
		}
		else
		{
			UtilityFunctions::print(
					"Skipped degenerate triangles on mesh {0}", String::num_int64(game_object->get_instance_id()));
		}
	}

	geometry.Vertices = ak_vertices.get();
	geometry.NumVertices = vertex_count;
	geometry.Triangles = ak_triangles.get();
	geometry.NumTriangles = triangleIdx;
	geometry.EnableDiffraction = enable_diffraction;
	geometry.EnableDiffractionOnBoundaryEdges = enable_diffraction_on_boundary_edges;

	return ERROR_CHECK(
			AK::SpatialAudio::SetGeometry(static_cast<AkGeometrySetID>(game_object->get_instance_id()), geometry));
}

bool Wwise::remove_geometry(const Object* game_object)
{
	AKASSERT(game_object);

	return ERROR_CHECK(AK::SpatialAudio::RemoveGeometry(static_cast<AkGeometrySetID>(game_object->get_instance_id())));
}

bool Wwise::set_geometry_instance(
		const Object* associated_geometry, const Transform3D& transform, const Object* geometry_instance)
{
	AkGeometryInstanceParams params{};
	params.GeometrySetID = associated_geometry->get_instance_id();

	AkVector position{};
	AkVector orientation_front{};
	AkVector orientation_top{};
	get_akvector(transform, position, VectorType::POSITION);
	get_akvector(transform, orientation_front, VectorType::FORWARD);
	get_akvector(transform, orientation_top, VectorType::UP);

	AkWorldTransform position_and_orientation{};
	position_and_orientation.SetPosition(AK::ConvertAkVectorToAkVector64(position));
	position_and_orientation.SetOrientation(orientation_front, orientation_top);
	params.PositionAndOrientation = position_and_orientation;
	
	Vector3 scale = transform.get_basis().get_scale();
	params.Scale = { scale.x, scale.y, scale.z };

	return ERROR_CHECK(AK::SpatialAudio::SetGeometryInstance(
			static_cast<AkGeometryInstanceID>(geometry_instance->get_instance_id()), params));
}

bool Wwise::remove_geometry_instance(const Object* geometry_instance)
{
	return ERROR_CHECK(AK::SpatialAudio::RemoveGeometryInstance(
			static_cast<AkGeometryInstanceID>(geometry_instance->get_instance_id())));
}

bool Wwise::register_spatial_listener(const Object* game_object)
{
	AKASSERT(game_object);

	return ERROR_CHECK(AK::SpatialAudio::RegisterListener(static_cast<AkGameObjectID>(game_object->get_instance_id())));
}

bool Wwise::set_room(const Object* game_object, const unsigned int ak_aux_bus_id, const float reverb_level,
		const float transmission_loss, const Vector3& front_vector, const Vector3& up_vector, bool keep_registered,
		const Object* associated_geometry_instance)
{
	AKASSERT(game_object);

	AkRoomParams room_params{};
	room_params.ReverbAuxBus = ak_aux_bus_id;
	room_params.ReverbLevel = reverb_level;
	room_params.TransmissionLoss = transmission_loss;

	AkVector front{};
	vector3_to_akvector(front_vector, front);
	AkVector up{};
	vector3_to_akvector(up_vector, up);

	room_params.Front = front;
	room_params.Up = up;
	room_params.RoomGameObj_KeepRegistered = keep_registered;
	room_params.RoomGameObj_AuxSendLevelToSelf = 0.0f;

	if (associated_geometry_instance)
	{
		room_params.GeometryInstanceID = static_cast<AkGeometrySetID>(associated_geometry_instance->get_instance_id());
	}

	return ERROR_CHECK(AK::SpatialAudio::SetRoom(static_cast<AkRoomID>(game_object->get_instance_id()), room_params));
}

bool Wwise::remove_room(const Object* game_object)
{
	return ERROR_CHECK(AK::SpatialAudio::RemoveRoom(static_cast<AkRoomID>(game_object->get_instance_id())));
}

bool Wwise::set_portal(const Object* game_object, const Transform3D transform, const Vector3& extent,
		const Object* front_room, const Object* back_room, bool enabled)
{
	AKASSERT(game_object);

	AkVector position;
	get_akvector(transform, position, VectorType::POSITION);
	AkVector forward;
	get_akvector(transform, forward, VectorType::FORWARD);
	AkVector up;
	get_akvector(transform, up, VectorType::UP);

	AkWorldTransform ak_transform;
	ak_transform.Set(AK::ConvertAkVectorToAkVector64(position), forward, up);

	// todo(alex): not used?
	AkRoomID room_id;

	AkPortalParams portal_params;
	AkExtent portal_extent;

	portal_extent.halfWidth = extent.x * 0.5f;
	portal_extent.halfHeight = extent.y * 0.5f;
	portal_extent.halfDepth = extent.z * 0.5f;

	portal_params.Transform = ak_transform;
	portal_params.Extent = portal_extent;
	portal_params.FrontRoom =
			front_room ? static_cast<AkRoomID>(front_room->get_instance_id()) : AK::SpatialAudio::kOutdoorRoomID;
	portal_params.BackRoom =
			back_room ? static_cast<AkRoomID>(back_room->get_instance_id()) : AK::SpatialAudio::kOutdoorRoomID;
	portal_params.bEnabled = enabled;

	return ERROR_CHECK(
			AK::SpatialAudio::SetPortal(static_cast<AkPortalID>(game_object->get_instance_id()), portal_params));
}

bool Wwise::remove_portal(const Object* game_object)
{
	AKASSERT(game_object);

	return ERROR_CHECK(AK::SpatialAudio::RemovePortal(static_cast<AkPortalID>(game_object->get_instance_id())));
}

bool Wwise::set_portal_obstruction_and_occlusion(
		const Object* portal, const float obstruction_value, const float occlusion_value)
{
	AKASSERT(portal);

	return ERROR_CHECK(AK::SpatialAudio::SetPortalObstructionAndOcclusion(
			static_cast<AkPortalID>(portal->get_instance_id()), obstruction_value, occlusion_value));
}

bool Wwise::set_game_object_to_portal_obstruction(
		const Object* game_object, const Object* portal, const float obstruction_value)
{
	AKASSERT(game_object);
	AKASSERT(portal);

	return ERROR_CHECK(AK::SpatialAudio::SetGameObjectToPortalObstruction(
			game_object->get_instance_id(), portal->get_instance_id(), obstruction_value));
}

bool Wwise::set_portal_to_portal_obstruction(
		const Object* portal0, const Object* portal1, const float obstruction_value)
{
	AKASSERT(portal0);
	AKASSERT(portal1);

	return ERROR_CHECK(AK::SpatialAudio::SetPortalToPortalObstruction(
			portal0->get_instance_id(), portal1->get_instance_id(), obstruction_value));
}

bool Wwise::set_game_object_in_room(const Object* game_object, const Object* room)
{
	AKASSERT(game_object);
	AKASSERT(room);
	return ERROR_CHECK(
			AK::SpatialAudio::SetGameObjectInRoom(static_cast<AkGameObjectID>(game_object->get_instance_id()),
					static_cast<AkRoomID>(room->get_instance_id())));
}

bool Wwise::remove_game_object_from_room(const Object* game_object)
{
	AKASSERT(game_object);

	return ERROR_CHECK(AK::SpatialAudio::SetGameObjectInRoom(
			static_cast<AkGameObjectID>(game_object->get_instance_id()), INVALID_ROOM_ID));
}

bool Wwise::set_early_reflections_aux_send(const Object* game_object, const unsigned int aux_bus_id)
{
	AKASSERT(game_object);

	return ERROR_CHECK(AK::SpatialAudio::SetEarlyReflectionsAuxSend(
			static_cast<AkGameObjectID>(game_object->get_instance_id()), static_cast<AkAuxBusID>(aux_bus_id)));
}

bool Wwise::set_early_reflections_volume(const Object* game_object, const float volume)
{
	AKASSERT(game_object);

	return ERROR_CHECK(AK::SpatialAudio::SetEarlyReflectionsVolume(
			static_cast<AkGameObjectID>(game_object->get_instance_id()), volume));
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

bool Wwise::is_initialized() { return AK::SoundEngine::IsInitialized(); }

void Wwise::event_callback(AkCallbackType callback_type, AkCallbackInfo* callback_info)
{
	AkAutoLock<CAkLock> scoped_lock(callback_data_lock);

	const WwiseCookie* wrapper = static_cast<WwiseCookie*>(callback_info->pCookie);

	if (!wrapper)
	{
		ERROR_CHECK_MSG(AK_Fail, "The Event Callback cookie is not valid.");
		return;
	}
	else if (!wrapper->get_cookie().is_valid())
	{
		ERROR_CHECK_MSG(AK_Fail, "The Event Callback cookie is not valid.");
		return;
	}

	Array args = Array();
	Dictionary callback_data;
	callback_data["callback_type"] = static_cast<unsigned int>(callback_type);

	switch (callback_type)
	{
		case AK_EndOfEvent:
		{
			AkEventCallbackInfo* event_info = static_cast<AkEventCallbackInfo*>(callback_info);
			callback_data["eventID"] = event_info->eventID;
			callback_data["gameObjID"] = event_info->gameObjID;
			callback_data["playingID"] = event_info->playingID;
			break;
		}
		case AK_EndOfDynamicSequenceItem:
		{
			AkDynamicSequenceItemCallbackInfo* dynamic_sequence_item_info =
					static_cast<AkDynamicSequenceItemCallbackInfo*>(callback_info);
			callback_data["audioNodeID"] = dynamic_sequence_item_info->audioNodeID;
			callback_data["gameObjID"] = dynamic_sequence_item_info->gameObjID;
			callback_data["playingID"] = dynamic_sequence_item_info->playingID;
			break;
		}
		case AK_Marker:
		{
			AkMarkerCallbackInfo* marker_info = static_cast<AkMarkerCallbackInfo*>(callback_info);
			callback_data["uIdentifier"] = marker_info->uIdentifier;
			callback_data["uPosition"] = marker_info->uPosition;
			callback_data["strLabel"] = String(marker_info->strLabel);
			break;
		}
		case AK_Duration:
		{
			AkDurationCallbackInfo* duration_info = static_cast<AkDurationCallbackInfo*>(callback_info);

			callback_data["audioNodeID"] = duration_info->audioNodeID;
			callback_data["bStreaming"] = duration_info->bStreaming;
			callback_data["eventID"] = duration_info->eventID;
			callback_data["fDuration"] = duration_info->fDuration;
			callback_data["fEstimatedDuration"] = duration_info->fEstimatedDuration;
			callback_data["gameObjID"] = duration_info->gameObjID;
			callback_data["mediaID"] = duration_info->mediaID;
			callback_data["playingID"] = duration_info->playingID;
			break;
		}
		case AK_SpeakerVolumeMatrix:
		{
			AkSpeakerVolumeMatrixCallbackInfo* speaker_volume_matrix_info =
					static_cast<AkSpeakerVolumeMatrixCallbackInfo*>(callback_info);
			callback_data["eventID"] = speaker_volume_matrix_info->eventID;
			callback_data["gameObjID"] = speaker_volume_matrix_info->gameObjID;

			Dictionary input_config;
			input_config["uNumChannels"] = speaker_volume_matrix_info->inputConfig.uNumChannels;
			input_config["eConfigType"] = speaker_volume_matrix_info->inputConfig.eConfigType;
			input_config["uChannelMask"] = speaker_volume_matrix_info->inputConfig.uChannelMask;
			callback_data["inputConfig"] = input_config;

			Dictionary output_config;
			output_config["uNumChannels"] = speaker_volume_matrix_info->outputConfig.uNumChannels;
			output_config["eConfigType"] = speaker_volume_matrix_info->outputConfig.eConfigType;
			output_config["uChannelMask"] = speaker_volume_matrix_info->outputConfig.uChannelMask;
			callback_data["outputConfig"] = output_config;

			callback_data["playingID"] = speaker_volume_matrix_info->playingID;
			break;
		}
		case AK_Starvation:
		{
			AkEventCallbackInfo* event_info = static_cast<AkEventCallbackInfo*>(callback_info);
			callback_data["eventID"] = event_info->eventID;
			callback_data["gameObjID"] = event_info->gameObjID;
			callback_data["playingID"] = event_info->playingID;
			break;
		}
		case AK_MusicPlaylistSelect:
		{
			AkMusicPlaylistCallbackInfo* music_playlist_info = static_cast<AkMusicPlaylistCallbackInfo*>(callback_info);
			callback_data["eventID"] = music_playlist_info->eventID;
			callback_data["gameObjID"] = music_playlist_info->gameObjID;
			callback_data["playingID"] = music_playlist_info->playingID;
			callback_data["playlistID"] = music_playlist_info->playlistID;
			callback_data["uNumPlaylistItems"] = music_playlist_info->uNumPlaylistItems;
			callback_data["uPlaylistItemDone"] = music_playlist_info->uPlaylistItemDone;
			callback_data["uPlaylistSelection"] = music_playlist_info->uPlaylistSelection;
			break;
		}
		case AK_MusicPlayStarted:
		{
			AkEventCallbackInfo* event_info = static_cast<AkEventCallbackInfo*>(callback_info);
			callback_data["eventID"] = event_info->eventID;
			callback_data["gameObjID"] = event_info->gameObjID;
			callback_data["playingID"] = event_info->playingID;
			break;
		}
		case AK_MusicSyncBeat:
		{
			AkMusicSyncCallbackInfo* music_sync_info = static_cast<AkMusicSyncCallbackInfo*>(callback_info);
			callback_data["gameObjID"] = music_sync_info->gameObjID;
			callback_data["musicSyncType"] = music_sync_info->musicSyncType;
			callback_data["playingID"] = music_sync_info->playingID;
			callback_data["pszUserCueName"] = String(music_sync_info->pszUserCueName);

			Dictionary segment_info;
			segment_info["fBarDuration"] = music_sync_info->segmentInfo.fBarDuration;
			segment_info["fBeatDuration"] = music_sync_info->segmentInfo.fBeatDuration;
			segment_info["fGridDuration"] = music_sync_info->segmentInfo.fGridDuration;
			segment_info["fGridOffset"] = music_sync_info->segmentInfo.fGridOffset;
			segment_info["iActiveDuration"] = music_sync_info->segmentInfo.iActiveDuration;
			segment_info["iCurrentPosition"] = music_sync_info->segmentInfo.iCurrentPosition;
			segment_info["iPostExitDuration"] = music_sync_info->segmentInfo.iPostExitDuration;
			segment_info["iPreEntryDuration"] = music_sync_info->segmentInfo.iPreEntryDuration;
			segment_info["iRemainingLookAheadTime"] = music_sync_info->segmentInfo.iRemainingLookAheadTime;
			callback_data["segmentInfo"] = segment_info;
			break;
		}
		case AK_MusicSyncBar:
		{
			AkMusicSyncCallbackInfo* music_sync_info = static_cast<AkMusicSyncCallbackInfo*>(callback_info);
			callback_data["gameObjID"] = music_sync_info->gameObjID;
			callback_data["musicSyncType"] = music_sync_info->musicSyncType;
			callback_data["playingID"] = music_sync_info->playingID;
			callback_data["pszUserCueName"] = String(music_sync_info->pszUserCueName);

			Dictionary segment_info;
			segment_info["fBarDuration"] = music_sync_info->segmentInfo.fBarDuration;
			segment_info["fBeatDuration"] = music_sync_info->segmentInfo.fBeatDuration;
			segment_info["fGridDuration"] = music_sync_info->segmentInfo.fGridDuration;
			segment_info["fGridOffset"] = music_sync_info->segmentInfo.fGridOffset;
			segment_info["iActiveDuration"] = music_sync_info->segmentInfo.iActiveDuration;
			segment_info["iCurrentPosition"] = music_sync_info->segmentInfo.iCurrentPosition;
			segment_info["iPostExitDuration"] = music_sync_info->segmentInfo.iPostExitDuration;
			segment_info["iPreEntryDuration"] = music_sync_info->segmentInfo.iPreEntryDuration;
			segment_info["iRemainingLookAheadTime"] = music_sync_info->segmentInfo.iRemainingLookAheadTime;
			callback_data["segmentInfo"] = segment_info;
			break;
		}
		case AK_MusicSyncEntry:
		{
			AkMusicSyncCallbackInfo* music_sync_info = static_cast<AkMusicSyncCallbackInfo*>(callback_info);
			callback_data["gameObjID"] = music_sync_info->gameObjID;
			callback_data["musicSyncType"] = music_sync_info->musicSyncType;
			callback_data["playingID"] = music_sync_info->playingID;
			callback_data["pszUserCueName"] = String(music_sync_info->pszUserCueName);

			Dictionary segment_info;
			segment_info["fBarDuration"] = music_sync_info->segmentInfo.fBarDuration;
			segment_info["fBeatDuration"] = music_sync_info->segmentInfo.fBeatDuration;
			segment_info["fGridDuration"] = music_sync_info->segmentInfo.fGridDuration;
			segment_info["fGridOffset"] = music_sync_info->segmentInfo.fGridOffset;
			segment_info["iActiveDuration"] = music_sync_info->segmentInfo.iActiveDuration;
			segment_info["iCurrentPosition"] = music_sync_info->segmentInfo.iCurrentPosition;
			segment_info["iPostExitDuration"] = music_sync_info->segmentInfo.iPostExitDuration;
			segment_info["iPreEntryDuration"] = music_sync_info->segmentInfo.iPreEntryDuration;
			segment_info["iRemainingLookAheadTime"] = music_sync_info->segmentInfo.iRemainingLookAheadTime;
			callback_data["segmentInfo"] = segment_info;
			break;
		}
		case AK_MusicSyncExit:
		{
			AkMusicSyncCallbackInfo* music_sync_info = static_cast<AkMusicSyncCallbackInfo*>(callback_info);
			callback_data["gameObjID"] = music_sync_info->gameObjID;
			callback_data["musicSyncType"] = music_sync_info->musicSyncType;
			callback_data["playingID"] = music_sync_info->playingID;
			callback_data["pszUserCueName"] = String(music_sync_info->pszUserCueName);

			Dictionary segment_info;
			segment_info["fBarDuration"] = music_sync_info->segmentInfo.fBarDuration;
			segment_info["fBeatDuration"] = music_sync_info->segmentInfo.fBeatDuration;
			segment_info["fGridDuration"] = music_sync_info->segmentInfo.fGridDuration;
			segment_info["fGridOffset"] = music_sync_info->segmentInfo.fGridOffset;
			segment_info["iActiveDuration"] = music_sync_info->segmentInfo.iActiveDuration;
			segment_info["iCurrentPosition"] = music_sync_info->segmentInfo.iCurrentPosition;
			segment_info["iPostExitDuration"] = music_sync_info->segmentInfo.iPostExitDuration;
			segment_info["iPreEntryDuration"] = music_sync_info->segmentInfo.iPreEntryDuration;
			segment_info["iRemainingLookAheadTime"] = music_sync_info->segmentInfo.iRemainingLookAheadTime;
			callback_data["segmentInfo"] = segment_info;
			break;
		}
		case AK_MusicSyncGrid:
		{
			AkMusicSyncCallbackInfo* music_sync_info = static_cast<AkMusicSyncCallbackInfo*>(callback_info);
			callback_data["gameObjID"] = music_sync_info->gameObjID;
			callback_data["musicSyncType"] = music_sync_info->musicSyncType;
			callback_data["playingID"] = music_sync_info->playingID;
			callback_data["pszUserCueName"] = String(music_sync_info->pszUserCueName);

			Dictionary segment_info;
			segment_info["fBarDuration"] = music_sync_info->segmentInfo.fBarDuration;
			segment_info["fBeatDuration"] = music_sync_info->segmentInfo.fBeatDuration;
			segment_info["fGridDuration"] = music_sync_info->segmentInfo.fGridDuration;
			segment_info["fGridOffset"] = music_sync_info->segmentInfo.fGridOffset;
			segment_info["iActiveDuration"] = music_sync_info->segmentInfo.iActiveDuration;
			segment_info["iCurrentPosition"] = music_sync_info->segmentInfo.iCurrentPosition;
			segment_info["iPostExitDuration"] = music_sync_info->segmentInfo.iPostExitDuration;
			segment_info["iPreEntryDuration"] = music_sync_info->segmentInfo.iPreEntryDuration;
			segment_info["iRemainingLookAheadTime"] = music_sync_info->segmentInfo.iRemainingLookAheadTime;
			callback_data["segmentInfo"] = segment_info;
			break;
		}
		case AK_MusicSyncUserCue:
		{
			AkMusicSyncCallbackInfo* music_sync_info = static_cast<AkMusicSyncCallbackInfo*>(callback_info);
			callback_data["gameObjID"] = music_sync_info->gameObjID;
			callback_data["musicSyncType"] = music_sync_info->musicSyncType;
			callback_data["playingID"] = music_sync_info->playingID;
			callback_data["pszUserCueName"] = String(music_sync_info->pszUserCueName);

			Dictionary segment_info;
			segment_info["fBarDuration"] = music_sync_info->segmentInfo.fBarDuration;
			segment_info["fBeatDuration"] = music_sync_info->segmentInfo.fBeatDuration;
			segment_info["fGridDuration"] = music_sync_info->segmentInfo.fGridDuration;
			segment_info["fGridOffset"] = music_sync_info->segmentInfo.fGridOffset;
			segment_info["iActiveDuration"] = music_sync_info->segmentInfo.iActiveDuration;
			segment_info["iCurrentPosition"] = music_sync_info->segmentInfo.iCurrentPosition;
			segment_info["iPostExitDuration"] = music_sync_info->segmentInfo.iPostExitDuration;
			segment_info["iPreEntryDuration"] = music_sync_info->segmentInfo.iPreEntryDuration;
			segment_info["iRemainingLookAheadTime"] = music_sync_info->segmentInfo.iRemainingLookAheadTime;
			callback_data["segmentInfo"] = segment_info;
			break;
		}
		case AK_MusicSyncPoint:
		{
			AkMusicSyncCallbackInfo* music_sync_info = static_cast<AkMusicSyncCallbackInfo*>(callback_info);
			callback_data["gameObjID"] = music_sync_info->gameObjID;
			callback_data["musicSyncType"] = music_sync_info->musicSyncType;
			callback_data["playingID"] = music_sync_info->playingID;
			callback_data["pszUserCueName"] = String(music_sync_info->pszUserCueName);

			Dictionary segment_info;
			segment_info["fBarDuration"] = music_sync_info->segmentInfo.fBarDuration;
			segment_info["fBeatDuration"] = music_sync_info->segmentInfo.fBeatDuration;
			segment_info["fGridDuration"] = music_sync_info->segmentInfo.fGridDuration;
			segment_info["fGridOffset"] = music_sync_info->segmentInfo.fGridOffset;
			segment_info["iActiveDuration"] = music_sync_info->segmentInfo.iActiveDuration;
			segment_info["iCurrentPosition"] = music_sync_info->segmentInfo.iCurrentPosition;
			segment_info["iPostExitDuration"] = music_sync_info->segmentInfo.iPostExitDuration;
			segment_info["iPreEntryDuration"] = music_sync_info->segmentInfo.iPreEntryDuration;
			segment_info["iRemainingLookAheadTime"] = music_sync_info->segmentInfo.iRemainingLookAheadTime;
			callback_data["segmentInfo"] = segment_info;
			break;
		}
		case AK_MusicSyncAll:
		{
			AkMusicSyncCallbackInfo* music_sync_info = static_cast<AkMusicSyncCallbackInfo*>(callback_info);
			callback_data["gameObjID"] = music_sync_info->gameObjID;
			callback_data["musicSyncType"] = music_sync_info->musicSyncType;
			callback_data["playingID"] = music_sync_info->playingID;
			callback_data["pszUserCueName"] = String(music_sync_info->pszUserCueName);

			Dictionary segment_info;
			segment_info["fBarDuration"] = music_sync_info->segmentInfo.fBarDuration;
			segment_info["fBeatDuration"] = music_sync_info->segmentInfo.fBeatDuration;
			segment_info["fGridDuration"] = music_sync_info->segmentInfo.fGridDuration;
			segment_info["fGridOffset"] = music_sync_info->segmentInfo.fGridOffset;
			segment_info["iActiveDuration"] = music_sync_info->segmentInfo.iActiveDuration;
			segment_info["iCurrentPosition"] = music_sync_info->segmentInfo.iCurrentPosition;
			segment_info["iPostExitDuration"] = music_sync_info->segmentInfo.iPostExitDuration;
			segment_info["iPreEntryDuration"] = music_sync_info->segmentInfo.iPreEntryDuration;
			segment_info["iRemainingLookAheadTime"] = music_sync_info->segmentInfo.iRemainingLookAheadTime;
			callback_data["segmentInfo"] = segment_info;
			break;
		}
		case AK_MIDIEvent:
		{
			AkMIDIEventCallbackInfo* midiEventInfo = static_cast<AkMIDIEventCallbackInfo*>(callback_info);
			callback_data["eventID"] = midiEventInfo->eventID;
			callback_data["gameObjID"] = midiEventInfo->gameObjID;

			Dictionary midiEvent;
			midiEvent["byType"] = static_cast<unsigned char>(midiEventInfo->midiEvent.byType);
			midiEvent["byChan"] = static_cast<unsigned char>(midiEventInfo->midiEvent.byChan);

			Dictionary cc;
			cc["byCc"] = static_cast<unsigned char>(midiEventInfo->midiEvent.Cc.byCc);
			cc["byValue"] = static_cast<unsigned char>(midiEventInfo->midiEvent.Cc.byValue);

			Dictionary chanAftertouch;
			chanAftertouch["byValue"] = static_cast<unsigned char>(midiEventInfo->midiEvent.ChanAftertouch.byValue);

			Dictionary gen;
			gen["byParam1"] = static_cast<unsigned char>(midiEventInfo->midiEvent.Gen.byParam1);
			gen["byParam2"] = static_cast<unsigned char>(midiEventInfo->midiEvent.Gen.byParam2);

			Dictionary noteAftertouch;
			noteAftertouch["byNote"] = static_cast<unsigned char>(midiEventInfo->midiEvent.NoteAftertouch.byNote);
			noteAftertouch["byValue"] = static_cast<unsigned char>(midiEventInfo->midiEvent.NoteAftertouch.byValue);

			Dictionary noteOnOff;
			noteOnOff["byNote"] = static_cast<unsigned char>(midiEventInfo->midiEvent.NoteOnOff.byNote);
			noteOnOff["byVelocity"] = static_cast<unsigned char>(midiEventInfo->midiEvent.NoteOnOff.byVelocity);

			Dictionary pitchBend;
			pitchBend["byValueLsb"] = static_cast<unsigned char>(midiEventInfo->midiEvent.PitchBend.byValueLsb);
			pitchBend["byValueMsb"] = static_cast<unsigned char>(midiEventInfo->midiEvent.PitchBend.byValueMsb);

			Dictionary programChange;
			programChange["byProgramNum"] =
					static_cast<unsigned char>(midiEventInfo->midiEvent.ProgramChange.byProgramNum);

			midiEvent["cc"] = cc;
			midiEvent["chanAftertouch"] = chanAftertouch;
			midiEvent["gen"] = gen;
			midiEvent["noteAftertouch"] = noteAftertouch;
			midiEvent["noteOnOff"] = noteOnOff;
			midiEvent["pitchBend"] = pitchBend;
			midiEvent["programChange"] = programChange;

			callback_data["midiEvent"] = midiEvent;

			callback_data["playingID"] = midiEventInfo->playingID;
			break;
		}
		case AK_CallbackBits:
		case AK_EnableGetSourcePlayPosition:
		case AK_EnableGetMusicPlayPosition:
		case AK_EnableGetSourceStreamBuffering:
			break;
		default:
			AKASSERT(false);
			break;
	}

	args.push_back(callback_data);
	wrapper->get_cookie().callv(args);
}

void Wwise::bank_callback(AkUInt32 bank_id, const void* in_memory_bank_ptr, AKRESULT load_result, void* in_pCookie)
{
	AkAutoLock<CAkLock> scoped_lock(callback_data_lock);

	const WwiseCookie* wrapper = static_cast<WwiseCookie*>(in_pCookie);

	if (!wrapper)
	{
		ERROR_CHECK_MSG(AK_Fail, "The Bank Callback cookie is not valid.");
		return;
	}
	else if (!wrapper->get_cookie().is_valid())
	{
		ERROR_CHECK_MSG(AK_Fail, "The Bank Callback cookie is not valid.");
		return;
	}

	Array args = Array();
	Dictionary callback_data;
	callback_data["bank_id"] = bank_id;
	callback_data["result"] = load_result;

	args.push_back(callback_data);
	wrapper->get_cookie().callv(args);
}

Variant Wwise::get_platform_project_setting(const String& setting)
{
	AKASSERT(project_settings);
	AKASSERT(!setting.is_empty());

	String platform_setting = setting;

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

	// Try to get the platform-specific setting, if it exists
	if (project_settings && project_settings->has_setting(platform_setting))
	{
		return project_settings->get(platform_setting);
	}

	// Otherwise, try to get the default platform-agnostic setting
	if (project_settings && project_settings->has_setting(setting))
	{
		return project_settings->get(setting);
	}
	else
	{
		AKASSERT(false);
		UtilityFunctions::print(vformat("WwiseGodot: Failed to get setting: %s", platform_setting));
		return "";
	}
}

bool Wwise::initialize_wwise_systems()
{
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
			static_cast<bool>(get_platform_project_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "use_stream_cache"));

	device_settings.fTargetAutoStmBufferLength = static_cast<float>(
			get_platform_project_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "target_auto_stream_buffer_length_ms"));

	device_settings.uIOMemorySize = static_cast<unsigned int>(
			get_platform_project_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "IO_memory_size"));

	device_settings.uMaxCachePinnedBytes = static_cast<unsigned int>(
			get_platform_project_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "maximum_pinned_bytes_in_cache"));

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
			get_platform_project_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "debug_out_of_range_check_enabled"));

	init_settings.bEnableGameSyncPreparation = static_cast<bool>(
			get_platform_project_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "enable_game_sync_preparation"));

	init_settings.fDebugOutOfRangeLimit = static_cast<float>(
			get_platform_project_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "debug_out_of_range_limit"));

	String audioDeviceShareSet =
			get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "main_output/audio_device_shareset");
	init_settings.settingsMainOutput.audioDeviceShareset =
			AK::SoundEngine::GetIDFromString(audioDeviceShareSet.utf8().get_data());

	const unsigned int channelConfigType = static_cast<unsigned int>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + "main_output/channel_config/channel_config_type"));

	const unsigned int numChannels = static_cast<unsigned int>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + "main_output/channel_config/number_of_channels"));

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
			get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "main_output/device_id"));

	init_settings.settingsMainOutput.ePanningRule = static_cast<AkPanningRule>(static_cast<unsigned int>(
			get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "main_output/panning_rule")));

	init_settings.uCommandQueueSize = static_cast<unsigned int>(
			get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "command_queue_size"));

	init_settings.uContinuousPlaybackLookAhead = static_cast<unsigned int>(
			get_platform_project_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "continuous_playback_look_ahead"));

	init_settings.uMaxHardwareTimeoutMs = static_cast<unsigned int>(
			get_platform_project_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "maximum_hardware_timeout_ms"));

	init_settings.uMaxNumPaths = static_cast<unsigned int>(
			get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "maximum_number_of_positioning_paths"));

	init_settings.uMonitorQueuePoolSize = static_cast<unsigned int>(
			get_platform_project_setting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "monitor_queue_pool_size"));

	const unsigned int num_samples_per_frame_enum = static_cast<unsigned int>(
			get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "samples_per_frame"));

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

	init_settings.fGameUnitsToMeters =
			static_cast<float>(get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "game_units_to_meters"));

	init_settings.eFloorPlane = AkFloorPlane_Default;

	AkPlatformInitSettings platform_init_settings;
	AK::SoundEngine::GetDefaultPlatformInitSettings(platform_init_settings);

	// Common platform settings
	const unsigned int num_refills_in_voice_enum = static_cast<unsigned int>(
			get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "number_of_refills_in_voice"));

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

	const unsigned int samplerate_enum =
			static_cast<unsigned int>(get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "sample_rate"));

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

		// Platform-specific settings
#ifdef AK_WIN
	platform_init_settings.uMaxSystemAudioObjects = static_cast<unsigned int>(
			get_platform_project_setting("wwise/windows_advanced_settings/max_system_audio_objects"));

#elif defined(AK_MAC_OS_X)
	platform_init_settings.eAudioAPI = static_cast<AkAudioAPIMac>(
			static_cast<unsigned int>(get_platform_project_setting("wwise/macos_advanced_settings/audio_API")));

#elif defined(AK_IOS)
	platform_init_settings.eAudioAPI = static_cast<AkAudioAPIiOS>(
			static_cast<unsigned int>(get_platform_project_setting("wwise/ios_advanced_settings/audio_API")));

	const unsigned int session_category_enum = static_cast<unsigned int>(
			get_platform_project_setting("wwise/ios_advanced_settings/audio_session_category"));

	platform_init_settings.audioSession.eCategory = static_cast<AkAudioSessionCategory>(session_category_enum);

	const unsigned int session_category_flags = static_cast<unsigned int>(
			get_platform_project_setting("wwise/ios_advanced_settings/audio_session_category_options"));

	platform_init_settings.audioSession.eCategoryOptions =
			static_cast<AkAudioSessionCategoryOptions>(session_category_flags);

	const unsigned int audio_session_mode_enum =
			static_cast<unsigned int>(get_platform_project_setting("wwise/ios_advanced_settings/audio_session_mode"));

	platform_init_settings.audioSession.eMode = static_cast<AkAudioSessionMode>(audio_session_mode_enum);

#elif defined(AK_ANDROID)

	platform_init_settings.eAudioAPI = static_cast<AkAudioAPIAndroid>(
			static_cast<unsigned int>(get_platform_project_setting("wwise/android_advanced_settings/audio_API")));

	platform_init_settings.bRoundFrameSizeToHWSize = static_cast<bool>(
			get_platform_project_setting("wwise/android_advanced_settings/round_frame_size_to_hw_size"));

	platform_init_settings.pJavaVM = JNISupport::getJavaVM();
	platform_init_settings.jActivity = JNISupport::getActivity();

	platform_init_settings.bEnableLowLatency = true;

#elif defined(AK_LINUX)

	platform_init_settings.eAudioAPI = static_cast<AkAudioAPILinux>(
			static_cast<unsigned int>(get_platform_project_setting("wwise/linux_advanced_settings/audio_API")));

#else
#error "Platform not supported"
#endif

	if (!ERROR_CHECK_MSG(AK::SoundEngine::Init(&init_settings, &platform_init_settings),
				"WwiseGodot: Sound engine initialization failed."))

	{
		return false;
	}

	AkMusicSettings musicInitSettings;
	AK::MusicEngine::GetDefaultInitSettings(musicInitSettings);

	musicInitSettings.fStreamingLookAheadRatio = static_cast<float>(
			get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + "streaming_look_ahead_ratio"));

	if (!ERROR_CHECK_MSG(AK::MusicEngine::Init(&musicInitSettings), "Music Engine initialization failed."))
	{
		return false;
	}

	AkSpatialAudioInitSettings spatialSettings;

	spatialSettings.uMaxSoundPropagationDepth = static_cast<unsigned int>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "max_sound_propagation_depth"));

	spatialSettings.fMovementThreshold = static_cast<AkReal32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "movement_threshold"));

	spatialSettings.uNumberOfPrimaryRays = static_cast<AkUInt32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "number_of_primary_rays"));

	spatialSettings.uMaxReflectionOrder = static_cast<AkUInt32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "max_reflection_order"));

	spatialSettings.uMaxDiffractionOrder = static_cast<AkUInt32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "max_diffraction_order"));

	spatialSettings.uMaxDiffractionPaths = static_cast<AkUInt32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "max_diffraction_paths"));

	spatialSettings.uMaxGlobalReflectionPaths = static_cast<AkUInt32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "max_global_reflection_paths"));

	spatialSettings.uMaxEmitterRoomAuxSends = static_cast<AkUInt32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "max_emitter_room_aux_sends"));

	spatialSettings.uDiffractionOnReflectionsOrder = static_cast<AkUInt32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "diffraction_on_reflections_order"));

	spatialSettings.fMaxDiffractionAngleDegrees = static_cast<AkReal32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "max_diffraction_angle_degrees"));

	spatialSettings.fMaxPathLength = static_cast<AkReal32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "max_path_length"));

	spatialSettings.fCPULimitPercentage = static_cast<AkReal32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "cpu_limit_percentage"));

	spatialSettings.fSmoothingConstantMs = static_cast<AkReal32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "smoothing_constant_ms"));

	spatialSettings.uLoadBalancingSpread = static_cast<AkUInt32>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "load_balancing_spread"));

	spatialSettings.bEnableGeometricDiffractionAndTransmission =
			static_cast<bool>(get_platform_project_setting(WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH +
					"enable_geometric_diffraction_and_transmission"));

	spatialSettings.bCalcEmitterVirtualPosition = static_cast<bool>(get_platform_project_setting(
			WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "calc_emitter_virtual_position"));

	spatialSettings.eTransmissionOperation =
			(AkTransmissionOperation) static_cast<AkUInt32>(get_platform_project_setting(
					WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "transmission_operation"));

	if (!ERROR_CHECK_MSG(AK::SpatialAudio::Init(spatialSettings), "Spatial Audio initialization failed."))
	{
		return false;
	}

#ifndef AK_OPTIMIZED
	AkCommSettings comm_settings{};
	AK::Comm::GetDefaultInitSettings(comm_settings);

	comm_settings.bInitSystemLib = static_cast<bool>(
			get_platform_project_setting(WWISE_COMMUNICATION_SETTINGS_PATH + "initialize_system_comms"));

	// note(alex): Default value: 24024
	comm_settings.ports.uCommand =
			static_cast<unsigned int>(get_platform_project_setting(WWISE_COMMUNICATION_SETTINGS_PATH + "command_port"));

	comm_settings.ports.uDiscoveryBroadcast = static_cast<unsigned int>(
			get_platform_project_setting(WWISE_COMMUNICATION_SETTINGS_PATH + "discovery_broadcast_port"));

	String network_name = get_platform_project_setting(WWISE_COMMUNICATION_SETTINGS_PATH + "network_name");
	AKPLATFORM::SafeStrCpy(
			comm_settings.szAppNetworkName, network_name.utf8().get_data(), AK_COMM_SETTINGS_MAX_STRING_SIZE);

	ERROR_CHECK_MSG(AK::Comm::Init(comm_settings), "Comm initialization failed");
#endif

	return true;
}

bool Wwise::shutdown_wwise_system()
{
#ifndef AK_OPTIMIZED
	AK::Comm::Term();
#endif

	if (!ERROR_CHECK_MSG(AK::SoundEngine::UnregisterAllGameObj(), "Unregister All GameObj failed."))
	{
		return false;
	}

	if (!ERROR_CHECK_MSG(AK::SoundEngine::ClearBanks(), "Failed to clear all Banks at shutdown."))
	{
		return false;
	}

	AK::MusicEngine::Term();

	AK::SoundEngine::Term();

	low_level_io.term();

	if (AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}

	AK::MemoryMgr::Term();

	return true;
}
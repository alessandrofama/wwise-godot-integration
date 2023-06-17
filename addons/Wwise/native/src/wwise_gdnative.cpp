#include "wwise_gdnative.h"

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

#if defined(AK_SOUNDSEED_AIR_IMPACT)
#include <AK/Plugin/AkSoundSeedImpactFXFactory.h>
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
CAkLock Wwise::callbackDataLock;
CAkLock g_localOutputLock;

#if defined(AK_ENABLE_ASSERTS)
void WwiseAssertHook(const char* in_pszExpression, const char* in_pszFileName, int in_lineNumber)
{
	String msg = "AKASSERT: " + String(in_pszExpression);
	UtilityFunctions::push_warning(msg, "WwiseAssertHook", String(in_pszFileName), in_lineNumber);
	AKPLATFORM::OutputDebugMsg(msg.utf8().get_data());
}
#endif

void LocalOutput(AK::Monitor::ErrorCode in_eErrorCode, const AkOSChar* in_pszError,
				 AK::Monitor::ErrorLevel in_eErrorLevel, AkPlayingID in_playingID, AkGameObjectID in_gameObjID)
{
	AkAutoLock<CAkLock> autoLock(g_localOutputLock);

	if (in_eErrorCode != AK::Monitor::ErrorCode::ErrorCode_NoError)
	{
		UtilityFunctions::push_error(String(in_pszError), "Wwise monitor", "", 0);
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

Wwise* Wwise::get_singleton()
{
	return singleton;
}

void Wwise::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("init"), &Wwise::init);
	ClassDB::bind_method(D_METHOD("process"), &Wwise::process);
	ClassDB::bind_method(D_METHOD("shutdown"), &Wwise::shutdown);
	ClassDB::bind_method(D_METHOD("set_base_path"), &Wwise::setBasePath);
	ClassDB::bind_method(D_METHOD("set_current_language"), &Wwise::setCurrentLanguage);
	ClassDB::bind_method(D_METHOD("load_bank"), &Wwise::loadBank);
	ClassDB::bind_method(D_METHOD("load_bank_id"), &Wwise::loadBankID);
	ClassDB::bind_method(D_METHOD("load_bank_async"), &Wwise::loadBankAsync);
	ClassDB::bind_method(D_METHOD("load_bank_async_id"), &Wwise::loadBankAsyncID);
	ClassDB::bind_method(D_METHOD("unload_bank"), &Wwise::unloadBank);
	ClassDB::bind_method(D_METHOD("unload_bank_id"), &Wwise::unloadBankID);
	ClassDB::bind_method(D_METHOD("unload_bank_async"), &Wwise::unloadBankAsync);
	ClassDB::bind_method(D_METHOD("unload_bank_async_id"), &Wwise::unloadBankAsyncID);
	ClassDB::bind_method(D_METHOD("register_listener"), &Wwise::registerListener);
	ClassDB::bind_method(D_METHOD("set_random_seed"), &Wwise::setRandomSeed);
	ClassDB::bind_method(D_METHOD("register_game_obj"), &Wwise::registerGameObject);
	ClassDB::bind_method(D_METHOD("unregister_game_obj"), &Wwise::unregisterGameObject);
	ClassDB::bind_method(D_METHOD("set_listeners"), &Wwise::setListeners);
	ClassDB::bind_method(D_METHOD("set_3d_position"), &Wwise::set3DPosition);
	ClassDB::bind_method(D_METHOD("set_2d_position"), &Wwise::set2DPosition);
	ClassDB::bind_method(D_METHOD("set_multiple_positions_3d"), &Wwise::setMultiplePositions3D);
	ClassDB::bind_method(D_METHOD("set_multiple_positions_2d"), &Wwise::setMultiplePositions2D);
	ClassDB::bind_method(D_METHOD("set_game_obj_radius"), &Wwise::setGameObjectRadius);
	ClassDB::bind_method(D_METHOD("post_event"), &Wwise::postEvent);
	ClassDB::bind_method(D_METHOD("post_event_callback"), &Wwise::postEventCallback);
	ClassDB::bind_method(D_METHOD("post_event_id"), &Wwise::postEventID);
	ClassDB::bind_method(D_METHOD("post_event_id_callback"), &Wwise::postEventIDCallback);
	ClassDB::bind_method(D_METHOD("stop_event"), &Wwise::stopEvent);
	ClassDB::bind_method(D_METHOD("set_switch"), &Wwise::setSwitch);
	ClassDB::bind_method(D_METHOD("set_switch_id"), &Wwise::setSwitchID);
	ClassDB::bind_method(D_METHOD("set_state"), &Wwise::setState);
	ClassDB::bind_method(D_METHOD("set_state_id"), &Wwise::setStateID);
	ClassDB::bind_method(D_METHOD("get_rtpc"), &Wwise::getRTPCValue);
	ClassDB::bind_method(D_METHOD("get_rtpc_id"), &Wwise::getRTPCValueID);
	ClassDB::bind_method(D_METHOD("set_rtpc"), &Wwise::setRTPCValue);
	ClassDB::bind_method(D_METHOD("set_rtpc_id"), &Wwise::setRTPCValueID);
	ClassDB::bind_method(D_METHOD("post_trigger"), &Wwise::postTrigger);
	ClassDB::bind_method(D_METHOD("post_trigger_id"), &Wwise::postTriggerID);
	ClassDB::bind_method(D_METHOD("post_external_source"), &Wwise::postExternalSource);
	ClassDB::bind_method(D_METHOD("post_external_source_id"), &Wwise::postExternalSourceID);
	ClassDB::bind_method(D_METHOD("get_source_play_position"), &Wwise::getSourcePlayPosition);
	ClassDB::bind_method(D_METHOD("get_playing_segment_info"), &Wwise::getPlayingSegmentInfo);
	ClassDB::bind_method(D_METHOD("set_game_obj_output_bus_volume"), &Wwise::setGameObjectOutputBusVolume);
	ClassDB::bind_method(D_METHOD("set_game_obj_aux_send_values"), &Wwise::setGameObjectAuxSendValues);
	ClassDB::bind_method(D_METHOD("set_obj_obstruction_and_occlusion"), &Wwise::setObjectObstructionAndOcclusion);
	ClassDB::bind_method(D_METHOD("set_geometry"), &Wwise::setGeometry);
	ClassDB::bind_method(D_METHOD("remove_geometry"), &Wwise::removeGeometry);
	ClassDB::bind_method(D_METHOD("register_spatial_listener"), &Wwise::registerSpatialListener);
	ClassDB::bind_method(D_METHOD("set_room"), &Wwise::setRoom);
	ClassDB::bind_method(D_METHOD("remove_room"), &Wwise::removeRoom);
	ClassDB::bind_method(D_METHOD("set_portal"), &Wwise::setPortal);
	ClassDB::bind_method(D_METHOD("remove_portal"), &Wwise::removePortal);
	ClassDB::bind_method(D_METHOD("set_portal_obstruction_and_occlusion"), &Wwise::setPortalObstructionAndOcclusion);
	ClassDB::bind_method(D_METHOD("set_game_obj_to_portal_obstruction"), &Wwise::setGameObjectToPortalObstruction);
	ClassDB::bind_method(D_METHOD("set_portal_to_portal_obstruction"), &Wwise::setPortalToPortalObstruction);
	ClassDB::bind_method(D_METHOD("set_game_obj_in_room"), &Wwise::setGameObjectInRoom);
	ClassDB::bind_method(D_METHOD("remove_game_obj_from_room"), &Wwise::removeGameObjectFromRoom);
	ClassDB::bind_method(D_METHOD("set_early_reflections_aux_send"), &Wwise::setEarlyReflectionsAuxSend);
	ClassDB::bind_method(D_METHOD("set_early_reflections_volume"), &Wwise::setEarlyReflectionsVolume);
	ClassDB::bind_method(D_METHOD("add_output"), &Wwise::addOutput);
	ClassDB::bind_method(D_METHOD("remove_output"), &Wwise::removeOutput);
	ClassDB::bind_method(D_METHOD("suspend"), &Wwise::suspend);
	ClassDB::bind_method(D_METHOD("wakeup_from_suspend"), &Wwise::wakeupFromSuspend);
}

void Wwise::init()
{
	projectSettings = ProjectSettings::get_singleton();
	AKASSERT(projectSettings);

	bool initialisationResult = initialiseWwiseSystems();

	if (!initialisationResult)
	{
		ERROR_CHECK(AK_Fail, "[Wwise] Wwise systems initialization failed!");
		return;
	}
	else
	{
		UtilityFunctions::print("[Wwise] Initialized Wwise systems");
	}

	String basePath = getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "base_path");

#ifdef AK_WIN
	String platformBanksSuffix = "/Windows";
#elif defined(AK_MAC_OS_X)
	String platformBanksSuffix = "/Mac";
#elif defined(AK_IOS)
	String platformBanksSuffix = "/iOS";
#elif defined(AK_ANDROID)
	String platformBanksSuffix = "/Android";
#elif defined(AK_LINUX)
	String platformBanksSuffix = "/Linux";
#else
#error "Platform not supported"
#endif

	basePath += platformBanksSuffix + "/";

	bool setBasePathResult = setBasePath(basePath);
	AKASSERT(setBasePathResult);

	String startupLanguage = getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "startup_language");

	setCurrentLanguage(startupLanguage);

#if !defined(AK_OPTIMIZED)
	const bool engineLogging =
		static_cast<bool>(getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "engine_logging"));

	if (engineLogging)
	{
		ERROR_CHECK(AK::Monitor::SetLocalOutput(AK::Monitor::ErrorLevel_All,
												static_cast<AK::Monitor::LocalOutputFunc>(LocalOutput)),
					"Failed to set ErrorLevel_All");
	}
#endif

	bool loadInitBank = getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "load_init_bank_at_startup");

	if (loadInitBank)
	{
		AkUInt32 initBankID = AK::SoundEngine::GetIDFromString("Init");
		loadBankID(initBankID);
	}

	suspendAtFocusLoss = getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "suspend_at_focus_loss");
	renderAnyway = getPlatformProjectSetting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "render_during_focus_loss");
}

void Wwise::process(double delta)
{
	ERROR_CHECK(AK::SoundEngine::RenderAudio(), "");
}

void Wwise::shutdown()
{
	if (shutdownWwiseSystems())
	{
		UtilityFunctions::print("[Wwise] Shut down Wwise systems");
	}
}

//void Wwise::_notification(int notification)
//{
//	switch (notification)
//	{
//	case NOTIFICATION_WM_WINDOW_FOCUS_OUT:
//		{
//			if (suspendAtFocusLoss)
//			{
//				Wwise::suspend(renderAnyway);
//			}
//			break;
//		}
//		case NOTIFICATION_APPLICATION_FOCUS_IN:
//		{
//			if (suspendAtFocusLoss)
//			{
//				Wwise::wakeupFromSuspend();
//			}
//			break;
//		}
//		default:
//		{
//			break;
//		}
//	}
//}

bool Wwise::setBasePath(const String basePath)
{
	AKASSERT(!basePath.is_empty());

	lowLevelIO.SetBanksPath(basePath);

	return true;
}

void Wwise::setCurrentLanguage(const String language)
{
	AKASSERT(!language.is_empty());

	lowLevelIO.SetLanguageFolder(language);
}

bool Wwise::loadBank(const String bankName)
{
	AkBankID bankID;
	AKASSERT(!bankName.is_empty());

	return ERROR_CHECK(AK::SoundEngine::LoadBank(bankName.utf8().get_data(), bankID), bankName);
}

bool Wwise::loadBankID(const unsigned int bankID)
{
	return ERROR_CHECK(AK::SoundEngine::LoadBank(bankID), "ID " + String::num_int64(bankID));
}

bool Wwise::loadBankAsync(const String bankName, const Object* cookie)
{
	AKASSERT(!bankName.is_empty());
	AKASSERT(cookie);

	AkBankID bankID = 0;

	return ERROR_CHECK(
		AK::SoundEngine::LoadBank(bankName.utf8().get_data(), (AkBankCallbackFunc)bankCallback, (void*)cookie, bankID),
		"ID " + String::num_int64(bankID));
}

bool Wwise::loadBankAsyncID(const unsigned int bankID, const Object* cookie)
{
	AKASSERT(cookie);

	return ERROR_CHECK(AK::SoundEngine::LoadBank(bankID, (AkBankCallbackFunc)bankCallback, (void*)cookie),
					   "ID " + String::num_int64(bankID));
}

bool Wwise::unloadBank(const String bankName)
{
	AKASSERT(!bankName.is_empty());

	return ERROR_CHECK(AK::SoundEngine::UnloadBank(bankName.utf8().get_data(), NULL), bankName);
}

bool Wwise::unloadBankID(const unsigned int bankID)
{
	return ERROR_CHECK(AK::SoundEngine::UnloadBank(bankID, NULL), "ID " + String::num_int64(bankID) + " failed");
}

bool Wwise::unloadBankAsync(const String bankName, const Object* cookie)
{
	AKASSERT(!bankName.is_empty());
	AKASSERT(cookie);

	return ERROR_CHECK(
		AK::SoundEngine::UnloadBank(bankName.utf8().get_data(), NULL, (AkBankCallbackFunc)bankCallback, (void*)cookie),
		"Loading bank: " + bankName + " failed");
}

bool Wwise::unloadBankAsyncID(const unsigned int bankID, const Object* cookie)
{
	AKASSERT(cookie);

	return ERROR_CHECK(AK::SoundEngine::UnloadBank(bankID, NULL, (AkBankCallbackFunc)bankCallback, (void*)cookie),
					   "ID " + String::num_int64(bankID) + " failed");
}

bool Wwise::registerListener(const Object* gameObject)
{
	AKASSERT(gameObject);

	const AkGameObjectID listener = static_cast<AkGameObjectID>(gameObject->get_instance_id());

	if (!ERROR_CHECK(AK::SoundEngine::RegisterGameObj(listener, "Default Listener"),
					 "ID " + String::num_int64(listener)))
	{
		return false;
	}

	if (!ERROR_CHECK(AK::SoundEngine::SetDefaultListeners(&listener, 1), "ID " + String::num_int64(listener)))
	{
		return false;
	}

        return true;
}

void Wwise::setRandomSeed(const unsigned int seed)
{
	AK::SoundEngine::SetRandomSeed(seed);
}

bool Wwise::registerGameObject(const Object* gameObject, const String gameObjectName)
{
	AKASSERT(gameObject);
	AKASSERT(!gameObjectName.is_empty());

	return ERROR_CHECK(AK::SoundEngine::RegisterGameObj(static_cast<AkGameObjectID>(gameObject->get_instance_id()),
														gameObjectName.utf8().get_data()),
					   gameObjectName);
}

bool Wwise::unregisterGameObject(const Object* gameObject)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SoundEngine::UnregisterGameObj(static_cast<AkGameObjectID>(gameObject->get_instance_id())),
					   "Failed to unregister Game Object: " + String::num_int64(gameObject->get_instance_id()));
}

bool Wwise::setListeners(const Object* emitter, const Object* listener)
{
	AKASSERT(emitter);
	AKASSERT(listener);
	
	static const int kNumLstnrsForEm = 1;
	static const AkGameObjectID aLstnrsForEmitter[kNumLstnrsForEm] = {
		static_cast<AkGameObjectID>(listener->get_instance_id())};
	return ERROR_CHECK(
		AK::SoundEngine::SetListeners(static_cast<AkGameObjectID>(emitter->get_instance_id()), aLstnrsForEmitter, 1),
		"Failed to associate Emitter with listener");
}

bool Wwise::set3DPosition(const Object* gameObject, const Transform3D transform)
{
	AKASSERT(gameObject);

	AkSoundPosition soundPos;

	AkVector position;
	GetAkVector(transform, position, VectorType::POSITION);
	AkVector forward;
	GetAkVector(transform, forward, VectorType::FORWARD);
	AkVector up;
	GetAkVector(transform, up, VectorType::UP);

	soundPos.Set(position, forward, up);

	return ERROR_CHECK(
		AK::SoundEngine::SetPosition(static_cast<AkGameObjectID>(gameObject->get_instance_id()), soundPos),
		"Game object ID " + String::num_int64(gameObject->get_instance_id()));
}

bool Wwise::set2DPosition(const Object* gameObject, const Transform2D transform2D, const float zDepth)
{
	AKASSERT(gameObject);

	AkSoundPosition soundPos;

	Vector2 origin = transform2D.get_origin();

	// todo(alex): implement distance scaling for 2D positions and remove the hardcoded and random 0.1f
	Vector3 position = Vector3(origin.x * 0.1f, origin.y * 0.1f, zDepth);
	Vector3 forward = Vector3(transform2D.columns[1].x, 0.0f, transform2D.columns[1].y).normalized();
	Vector3 up = Vector3(0, 1, 0);

	AkVector akPosition;
	Vector3ToAkVector(position, akPosition);
	AkVector akForward;
	Vector3ToAkVector(forward, akForward);
	AkVector akUp;
	Vector3ToAkVector(up, akUp);

	soundPos.Set(akPosition, akForward, akUp);

	return ERROR_CHECK(
		AK::SoundEngine::SetPosition(static_cast<AkGameObjectID>(gameObject->get_instance_id()), soundPos),
		"Game object ID " + String::num_int64(gameObject->get_instance_id()));
}

bool Wwise::setMultiplePositions3D(const Object* gameObject, const Array positions, const int numPositions,
								   const int multiPositionType)
{
	AKASSERT(gameObject);
	AKASSERT(positions.size() > 0);
	AKASSERT(positions.size() == numPositions);

	auto akPositions = std::make_unique<AkSoundPosition[]>(numPositions);

	for (int i = 0; i < positions.size(); i++)
	{
		Transform3D transform = positions[i];
		AkVector position;
		GetAkVector(transform, position, VectorType::POSITION);
		AkVector forward;
		GetAkVector(transform, forward, VectorType::FORWARD);
		AkVector up;
		GetAkVector(transform, up, VectorType::UP);

		akPositions[i].Set(position, forward, up);
	}

	return ERROR_CHECK(
		AK::SoundEngine::SetMultiplePositions(gameObject->get_instance_id(), akPositions.get(), numPositions,
											  static_cast<AK::SoundEngine::MultiPositionType>(multiPositionType)),
		"Failed to set multiple positions for GameObject with id: " + String::num_int64(gameObject->get_instance_id()));
}

bool Wwise::setMultiplePositions2D(const Object* gameObject, const Array positions, const Array zDepths,
								   const int numPositions, const int multiPositionType)
{
	AKASSERT(gameObject);
	AKASSERT(positions.size() > 0);
	AKASSERT(positions.size() == numPositions);
	AKASSERT(zDepths.size() == numPositions);

	auto akPositions = std::make_unique<AkSoundPosition[]>(numPositions);

	for (int i = 0; i < positions.size(); i++)
	{
		Transform2D transform2D = positions[i];

		Vector2 origin = transform2D.get_origin();
		Vector3 position = Vector3(-origin.x * 0.1f, -origin.y * 0.1f, zDepths[i]);
		Vector3 forward = Vector3(transform2D.columns[1].x, 0, transform2D.columns[1].y).normalized();
		Vector3 up = Vector3(0, 1, 0);

		AkVector akPosition;
		Vector3ToAkVector(position, akPosition);
		AkVector akForward;
		Vector3ToAkVector(forward, akForward);
		AkVector akUp;
		Vector3ToAkVector(up, akUp);

		akPositions[i].Set(akPosition, akForward, akUp);
	}

	return ERROR_CHECK(
		AK::SoundEngine::SetMultiplePositions(gameObject->get_instance_id(), akPositions.get(), numPositions,
											  static_cast<AK::SoundEngine::MultiPositionType>(multiPositionType)),
		"Failed to set multiple positions for GameObject with id: " + String::num_int64(gameObject->get_instance_id()));
}

bool Wwise::setGameObjectRadius(const Object* gameObject, const float outerRadius, const float innerRadius)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SpatialAudio::SetGameObjectRadius(gameObject->get_instance_id(), outerRadius, innerRadius),
					   "Failed to Set GameObject radius for GameObject with id: " +
						   String::num_int64(gameObject->get_instance_id()));
}

unsigned int Wwise::postEvent(const String eventName, const Object* gameObject)
{
	AKASSERT(!eventName.is_empty());
	AKASSERT(gameObject);

	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventName.utf8().get_data(),
													   static_cast<AkGameObjectID>(gameObject->get_instance_id()));

	if (playingID == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK(AK_InvalidID, eventName);
	}

	return static_cast<unsigned int>(playingID);
}

unsigned int Wwise::postEventCallback(const String eventName, const unsigned int flags, const Object* gameObject,
									  const Object* cookie)
{
	AKASSERT(!eventName.is_empty());
	AKASSERT(gameObject);
	AKASSERT(cookie);

	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventName.utf8().get_data(),
													   static_cast<AkGameObjectID>(gameObject->get_instance_id()),
													   flags, eventCallback, (void*)cookie);

	if (playingID == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK(AK_InvalidID, eventName);
	}

	return static_cast<unsigned int>(playingID);
}

unsigned int Wwise::postEventID(const unsigned int eventID, const Object* gameObject)
{
	AKASSERT(gameObject);

	AkPlayingID playingID =
		AK::SoundEngine::PostEvent(eventID, static_cast<AkGameObjectID>(gameObject->get_instance_id()));

	if (playingID == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK(AK_InvalidID, itos(eventID));
	}

	return static_cast<unsigned int>(playingID);
}

unsigned int Wwise::postEventIDCallback(const unsigned int eventID, const unsigned int flags, const Object* gameObject,
										const Object* cookie)
{
	AKASSERT(gameObject);
	AKASSERT(cookie);

	AkPlayingID playingID = AK::SoundEngine::PostEvent(
		eventID, static_cast<AkGameObjectID>(gameObject->get_instance_id()), flags, eventCallback, (void*)cookie);

	if (playingID == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK(AK_InvalidID, itos(eventID));
	}

	return static_cast<unsigned int>(playingID);
}

bool Wwise::stopEvent(const int playingID, const int fadeTime, const int interpolation)
{
	AKASSERT(fadeTime >= 0);

	AK::SoundEngine::ExecuteActionOnPlayingID(AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Stop,
											  static_cast<AkPlayingID>(playingID), static_cast<AkTimeMs>(fadeTime),
											  static_cast<AkCurveInterpolation>(interpolation));

	return true;
}

bool Wwise::setSwitch(const String switchGroup, const String switchState, const Object* gameObject)
{
	AKASSERT(!switchGroup.is_empty());
	AKASSERT(!switchState.is_empty());
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SoundEngine::SetSwitch(switchGroup.utf8().get_data(), switchState.utf8().get_data(),
												  static_cast<AkGameObjectID>(gameObject->get_instance_id())),
					   "Switch " + switchGroup + " and state " + switchState);
}

bool Wwise::setSwitchID(const unsigned int switchGroupID, const unsigned int switchStateID, const Object* gameObject)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SoundEngine::SetSwitch(switchGroupID, switchStateID,
												  static_cast<AkGameObjectID>(gameObject->get_instance_id())),
					   "Switch ID " + String::num_int64(switchGroupID) + " and switch state ID " +
						   String::num_int64(switchStateID));
}

bool Wwise::setState(const String stateGroup, const String stateValue)
{
	AKASSERT(!stateGroup.is_empty());
	AKASSERT(!stateValue.is_empty());

	return ERROR_CHECK(AK::SoundEngine::SetState(stateGroup.utf8().get_data(), stateValue.utf8().get_data()),
					   "Failed to set state " + stateGroup + " and value " + stateValue);
}

bool Wwise::setStateID(const unsigned int stateGroupID, const unsigned int stateValueID)
{
	return ERROR_CHECK(AK::SoundEngine::SetState(stateGroupID, stateValueID),
					   "Failed to set state ID" + String::num_int64(stateGroupID) + " and value " +
						   String::num_int64(stateValueID));
}

float Wwise::getRTPCValue(const String rtpcName, const Object* gameObject)
{
	AKASSERT(!rtpcName.is_empty());
	AkRtpcValue value;
	AK::SoundEngine::Query::RTPCValue_type type = AK::SoundEngine::Query::RTPCValue_Default;
	AkGameObjectID gameObjectID = AK_INVALID_GAME_OBJECT;

	if (gameObject)
	{
		type = AK::SoundEngine::Query::RTPCValue_GameObject;
		gameObjectID = static_cast<AkGameObjectID>(gameObject->get_instance_id());
	}
	else
	{
		type = AK::SoundEngine::Query::RTPCValue_Global;
		gameObjectID = AK_INVALID_GAME_OBJECT;
	}

	if (!ERROR_CHECK(AK::SoundEngine::Query::GetRTPCValue(rtpcName.utf8().get_data(), gameObjectID,
														  static_cast<AkPlayingID>(0), value, type),
					 rtpcName))
	{
		return INVALID_RTPC_VALUE;
	}

	return static_cast<float>(value);
}

float Wwise::getRTPCValueID(const unsigned int rtpcID, const Object* gameObject)
{
	AkRtpcValue value;
	AK::SoundEngine::Query::RTPCValue_type type = AK::SoundEngine::Query::RTPCValue_Default;
	AkGameObjectID gameObjectID = AK_INVALID_GAME_OBJECT;

	if (gameObject)
	{
		type = AK::SoundEngine::Query::RTPCValue_GameObject;
		gameObjectID = static_cast<AkGameObjectID>(gameObject->get_instance_id());
	}
	else
	{
		type = AK::SoundEngine::Query::RTPCValue_Global;
		gameObjectID = AK_INVALID_GAME_OBJECT;
	}

	if (!ERROR_CHECK(
			AK::SoundEngine::Query::GetRTPCValue(rtpcID, gameObjectID, static_cast<AkPlayingID>(0), value, type),
			String::num_int64(rtpcID)))
	{
		return INVALID_RTPC_VALUE;
	}

	return static_cast<float>(value);
}

bool Wwise::setRTPCValue(const String rtpcName, const float rtpcValue, const Object* gameObject)
{
	AKASSERT(!rtpcName.is_empty());
	AkGameObjectID gameObjectID = AK_INVALID_GAME_OBJECT;

	if (gameObject)
	{
		gameObjectID = static_cast<AkGameObjectID>(gameObject->get_instance_id());
	}
	else
	{
		gameObjectID = AK_INVALID_GAME_OBJECT;
	}

	return ERROR_CHECK(
		AK::SoundEngine::SetRTPCValue(rtpcName.utf8().get_data(), static_cast<AkRtpcValue>(rtpcValue), gameObjectID),
		rtpcName);
}

bool Wwise::setRTPCValueID(const unsigned int rtpcID, const float rtpcValue, const Object* gameObject)
{
	AkGameObjectID gameObjectID = AK_INVALID_GAME_OBJECT;

	if (gameObject)
	{
		gameObjectID = static_cast<AkGameObjectID>(gameObject->get_instance_id());
	}
	else
	{
		gameObjectID = AK_INVALID_GAME_OBJECT;
	}

	return ERROR_CHECK(AK::SoundEngine::SetRTPCValue(rtpcID, static_cast<AkRtpcValue>(rtpcValue), gameObjectID),
					   String::num_int64(rtpcID));
}

bool Wwise::postTrigger(const String triggerName, const Object* gameObject)
{
	AKASSERT(!triggerName.is_empty());
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SoundEngine::PostTrigger(triggerName.utf8().get_data(),
													static_cast<AkGameObjectID>(gameObject->get_instance_id())),
					   "Failed to post trigger " + triggerName);
}

bool Wwise::postTriggerID(const unsigned int triggerID, const Object* gameObject)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(
		AK::SoundEngine::PostTrigger(triggerID, static_cast<AkGameObjectID>(gameObject->get_instance_id())),
		"Failed to post trigger ID " + String::num_int64(triggerID));
}

unsigned int Wwise::postExternalSource(const String eventName, const Object* gameObject, const String sourceObjectName,
									   const String fileName, const unsigned int idCodec)
{
	AKASSERT(!eventName.is_empty());
	AKASSERT(gameObject);
	AKASSERT(!sourceObjectName.is_empty());
	AKASSERT(!fileName.is_empty());

	AkExternalSourceInfo source;
	source.iExternalSrcCookie = AK::SoundEngine::GetIDFromString(sourceObjectName.utf8().get_data());

	AkOSChar* szFileOsString = nullptr;

	CONVERT_CHAR_TO_OSCHAR(fileName.utf8().get_data(), szFileOsString);

	source.szFile = szFileOsString;
	source.idCodec = idCodec;

	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventName.utf8().get_data(),
													   static_cast<AkGameObjectID>(gameObject->get_instance_id()), 0,
													   nullptr, 0, 1, &source);

	if (playingID == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK(AK_InvalidID, eventName);
		return static_cast<unsigned int>(AK_INVALID_PLAYING_ID);
	}

	return static_cast<unsigned int>(playingID);
}

unsigned int Wwise::postExternalSourceID(const unsigned int eventID, const Object* gameObject,
										 const unsigned int sourceObjectID, const String fileName,
										 const unsigned int idCodec)
{
	AKASSERT(gameObject);
	AKASSERT(!fileName.is_empty());

	AkExternalSourceInfo source;
	source.iExternalSrcCookie = sourceObjectID;

	AkOSChar* szFileOsString = nullptr;

	CONVERT_CHAR_TO_OSCHAR(fileName.utf8().get_data(), szFileOsString);

	source.szFile = szFileOsString;
	source.idCodec = idCodec;

	AkPlayingID playingID = AK::SoundEngine::PostEvent(
		eventID, static_cast<AkGameObjectID>(gameObject->get_instance_id()), 0, NULL, 0, 1, &source);

	if (playingID == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK(AK_InvalidID, itos(eventID));
		return static_cast<unsigned int>(AK_INVALID_PLAYING_ID);
	}

	return static_cast<unsigned int>(playingID);
}

int Wwise::getSourcePlayPosition(const unsigned int playingID, const bool extrapolate)
{
	AkTimeMs position;
	AKRESULT result =
		AK::SoundEngine::GetSourcePlayPosition(static_cast<AkPlayingID>(playingID), &position, extrapolate);

	if (result == AK_Fail)
	{
		ERROR_CHECK(result, "Failed to get Source Play Position for playing ID " + String::num_int64(playingID));
		return static_cast<int>(AK_INVALID_PLAYING_ID);
	}

	return static_cast<int>(position);
}

Dictionary Wwise::getPlayingSegmentInfo(const unsigned int playingID, const bool extrapolate)
{
	AkSegmentInfo segmentInfo;
	AKRESULT result =
		AK::MusicEngine::GetPlayingSegmentInfo(static_cast<AkPlayingID>(playingID), segmentInfo, extrapolate);

	if (result == AK_Fail)
	{
		ERROR_CHECK(result, "Failed to get playing Segment Info for playing ID " + String::num_int64(playingID));
	}

	Dictionary segment;
	segment["fBarDuration"] = static_cast<float>(segmentInfo.fBarDuration);
	segment["fBeatDuration"] = static_cast<float>(segmentInfo.fBeatDuration);
	segment["fGridDuration"] = static_cast<float>(segmentInfo.fGridDuration);
	segment["fGridOffset"] = static_cast<float>(segmentInfo.fGridOffset);
	segment["iActiveDuration"] = static_cast<int>(segmentInfo.iActiveDuration);
	segment["iCurrentPosition"] = static_cast<int>(segmentInfo.iCurrentPosition);
	segment["iPostExitDuration"] = static_cast<int>(segmentInfo.iPostExitDuration);
	segment["iPreEntryDuration"] = static_cast<int>(segmentInfo.iPreEntryDuration);
	segment["iRemainingLookAheadTime"] = static_cast<int>(segmentInfo.iRemainingLookAheadTime);

	return segment;
}

bool Wwise::setGameObjectOutputBusVolume(const unsigned int gameObjectID, const unsigned int listenerID,
										 float fControlValue)
{
	return ERROR_CHECK(AK::SoundEngine::SetGameObjectOutputBusVolume(static_cast<AkGameObjectID>(gameObjectID),
																	 static_cast<AkGameObjectID>(listenerID),
																	 fControlValue),
					   "Could not set the Game Object Outpus Bus Volume");
}

bool Wwise::setGameObjectAuxSendValues(const unsigned int gameObjectID, const Array akAuxSendValues,
									   const unsigned int sendValues)
{
	AkAuxSendValue environments[AK_MAX_ENVIRONMENTS];

	for (int i = 0; i < akAuxSendValues.size(); i++)
	{
		Dictionary auxBusData = akAuxSendValues[i];
		environments[i].auxBusID = static_cast<const unsigned int>(auxBusData["aux_bus_id"]);
		environments[i].fControlValue = static_cast<float>(auxBusData["control_value"]);
		environments[i].listenerID = AK_INVALID_GAME_OBJECT;
	}

	return ERROR_CHECK(AK::SoundEngine::SetGameObjectAuxSendValues(static_cast<AkGameObjectID>(gameObjectID),
																   environments, sendValues),
					   "Could not set the Game Object Aux Send Values");
}

bool Wwise::setObjectObstructionAndOcclusion(const unsigned int gameObjectID, const unsigned int listenerID,
											 float fCalculatedObs, float fCalculatedOcc)
{
	return ERROR_CHECK(AK::SoundEngine::SetObjectObstructionAndOcclusion(static_cast<AkGameObjectID>(gameObjectID),
																		 static_cast<AkGameObjectID>(listenerID),
																		 fCalculatedObs, fCalculatedOcc),
					   "Could not set Obstruction and Occlusion");
}

bool Wwise::setGeometry(const Array vertices, const Array triangles, const Resource* acousticTexture,
						const float transmissionLossValue, const Object* gameObject, bool enableDiffraction,
						bool enableDiffractionOnBoundaryEdges, bool enableTriangles)
{
	AKASSERT(!vertices.is_empty());
	AKASSERT(!triangles.is_empty());
	AKASSERT(gameObject);

	AkGeometryParams geometry;

	int vertexCount = vertices.size();
	auto vertRemap = std::make_unique<int[]>(vertexCount);
	PackedVector3Array uniqueVerts;
	Dictionary vertDict;

	// removing duplicate verts
	for (int v = 0; v < vertexCount; ++v)
	{
		int vertIdx = 0;

		if (!FindMatchingVertex(vertices[v], vertDict, vertIdx))
		{
			vertIdx = uniqueVerts.size();
			uniqueVerts.append(vertices[v]);
			vertDict[vertices[v]] = vertIdx;
		}
		vertRemap[v] = vertIdx;
	}

	vertexCount = uniqueVerts.size();
	auto akVertices = std::make_unique<AkVertex[]>(vertexCount);

	for (int i = 0; i < vertexCount; i++)
	{
		Vector3 point = uniqueVerts[i];
		AkVertex v;
		v.X = -point.x; // Seems to be flipped in Wwise otherwise
		v.Y = point.y;
		v.Z = point.z;

		akVertices[i] = v;
	}

	int numTriangles = triangles.size() / 3;

	if ((numTriangles % 3) != 0)
	{
		UtilityFunctions::print("Wrong number of triangles on mesh {0}", String::num_int64(gameObject->get_instance_id()));
	}

	auto akTriangles = std::make_unique<AkTriangle[]>(numTriangles);

	if (acousticTexture)
	{
		AkAcousticSurface akSurfaces[1];

		geometry.NumSurfaces = 1;

		// AkAcousticTexture akAcousticTexture;
		String acousticTextureName = acousticTexture->get("name");
		// akAcousticTexture.ID = AK::SoundEngine::GetIDFromString(acousticTextureName.utf8().get_data());

		// // Not possible to get the acoustic texture values through AK::SoundEngine, maybe looking at WAAPI
		// akAcousticTexture.fAbsorptionHigh = static_cast<float>(acousticTexture->get("absorption_high"));
		// akAcousticTexture.fAbsorptionLow = static_cast<float>(acousticTexture->get("absorption_low"));
		// akAcousticTexture.fAbsorptionMidHigh = static_cast<float>(acousticTexture->get("absorption_mid_high"));
		// akAcousticTexture.fAbsorptionMidLow = static_cast<float>(acousticTexture->get("absorption_mid_low"));
		// akAcousticTexture.fAbsorptionOffset = static_cast<float>(acousticTexture->get("absorption_offset"));
		// akAcousticTexture.fScattering = static_cast<float>(acousticTexture->get("scattering"));

		akSurfaces[0].textureID = AK::SoundEngine::GetIDFromString(acousticTextureName.utf8().get_data());
		akSurfaces[0].transmissionLoss = transmissionLossValue;
		akSurfaces[0].strName = acousticTextureName.utf8().get_data();

		geometry.Surfaces = akSurfaces;
	}

	int triangleIdx = 0;

	for (int i = 0; i < numTriangles; i++)
	{
		AkTriangle t;
		t.point0 = vertRemap[static_cast<unsigned int>(triangles[3 * i + 0])];
		t.point1 = vertRemap[static_cast<unsigned int>(triangles[3 * i + 1])];
		t.point2 = vertRemap[static_cast<unsigned int>(triangles[3 * i + 2])];
		t.surface = acousticTexture ? 0 : AK_INVALID_SURFACE;

		akTriangles[triangleIdx] = t;

		if (t.point0 != t.point1 && t.point0 != t.point2 && t.point1 != t.point2)
		{
			++triangleIdx;
		}
		else
		{
			UtilityFunctions::print("Skipped degenerate triangles on mesh {0}", String::num_int64(gameObject->get_instance_id()));
		}
	}

	geometry.Vertices = akVertices.get();
	geometry.NumVertices = vertexCount;
	geometry.Triangles = akTriangles.get();
	geometry.NumTriangles = triangleIdx;

	geometry.EnableDiffraction = enableDiffraction;
	geometry.EnableDiffractionOnBoundaryEdges = enableDiffractionOnBoundaryEdges;

	geometry.EnableTriangles = enableTriangles;

	return ERROR_CHECK(
		AK::SpatialAudio::SetGeometry(static_cast<AkGeometrySetID>(gameObject->get_instance_id()), geometry),
		"Failed to register geometry");
}

bool Wwise::removeGeometry(const Object* gameObject)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SpatialAudio::RemoveGeometry(static_cast<AkGeometrySetID>(gameObject->get_instance_id())),
					   "Failed to remove geometry");
}

bool Wwise::setGeometryInstance(const Object* gameObject, const Transform3D transform,
									   const Object* associatedGeometry, const Object* associatedRoom)
{
	AkGeometryInstanceParams params;
	params.GeometrySetID = gameObject->get_instance_id();
	// todo(alex);
	//params.PositionAndOrientation;
	params.RoomID = associatedRoom ? static_cast<AkRoomID>(associatedRoom->get_instance_id())
									 : static_cast<AkRoomID>(INVALID_ROOM_ID);
	Vector3ToAkVector(transform.get_basis().get_scale(), params.Scale);
	return ERROR_CHECK(AK::SpatialAudio::SetGeometryInstance(static_cast<AkGeometryInstanceID>(gameObject->get_instance_id()), params),
					   "Failed to set Geometry Instance");
}

bool Wwise::removeGeometryInstance(const Object* gameObject)
{
	return ERROR_CHECK(AK::SpatialAudio::RemoveGeometryInstance(static_cast<AkGeometryInstanceID>(gameObject->get_instance_id())), "Failed to remove Geometry Instance");
}

bool Wwise::registerSpatialListener(const Object* gameObject)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SpatialAudio::RegisterListener(static_cast<AkGameObjectID>(gameObject->get_instance_id())),
					   "Failed to register Spatial Audio Listener");
}

bool Wwise::setRoom(const Object* gameObject, const unsigned int akAuxBusID, const float reverbLevel,
					const float transmissionLoss, const Vector3 frontVector, const Vector3 upVector, bool keepRegistered,
					const int associatedGeometry)
{
	AKASSERT(gameObject);

	AkRoomParams roomParams;
	roomParams.ReverbAuxBus = akAuxBusID;
	roomParams.ReverbLevel = reverbLevel;
	roomParams.TransmissionLoss = transmissionLoss;

	AkVector front;
	Vector3ToAkVector(frontVector, front);
	AkVector up;
	Vector3ToAkVector(upVector, up);

	roomParams.Front = front;
	roomParams.Up = up;
	roomParams.RoomGameObj_KeepRegistered = keepRegistered;
	roomParams.RoomGameObj_AuxSendLevelToSelf = 0.0f; // todo(alex): check this;
	roomParams.GeometryInstanceID = static_cast<AkGeometrySetID>(associatedGeometry);

	return ERROR_CHECK(AK::SpatialAudio::SetRoom(static_cast<AkRoomID>(gameObject->get_instance_id()), roomParams),
					   "Failed to set Room for Game Object: " + String::num_int64(gameObject->get_instance_id()));
}

bool Wwise::removeRoom(const Object* gameObject)
{
	return ERROR_CHECK(AK::SpatialAudio::RemoveRoom(static_cast<AkRoomID>(gameObject->get_instance_id())),
					   "Failed to remove Room for Game Object: " + String::num_int64(gameObject->get_instance_id()));
}

bool Wwise::setPortal(const Object* gameObject, const Transform3D transform,
 const Vector3 extent,
					  const Object* frontRoom, const Object* backRoom, bool enabled)
{
	AKASSERT(gameObject);

	AkVector position;
	GetAkVector(transform, position, VectorType::POSITION);
	AkVector forward;
	GetAkVector(transform, forward, VectorType::FORWARD);
	AkVector up;
	GetAkVector(transform, up, VectorType::UP);

	AkTransform akTransform;
	akTransform.Set(position, forward, up);

	AkRoomID roomID;

	AkPortalParams portalParams;
	AkExtent portalExtent;

	portalExtent.halfWidth = extent.x * 0.25f;
	portalExtent.halfHeight = extent.y * 0.25f;
	portalExtent.halfDepth = extent.z * 0.25f;

	portalParams.Transform = akTransform;
	portalParams.Extent = portalExtent;
	portalParams.FrontRoom =
		frontRoom ? static_cast<AkRoomID>(frontRoom->get_instance_id()) : AK::SpatialAudio::kOutdoorRoomID;
	portalParams.BackRoom =
		backRoom ? static_cast<AkRoomID>(backRoom->get_instance_id()) : AK::SpatialAudio::kOutdoorRoomID;
	portalParams.bEnabled = enabled;

	return ERROR_CHECK(
		AK::SpatialAudio::SetPortal(static_cast<AkPortalID>(gameObject->get_instance_id()), portalParams),
		"Failed to set Portal on GameObject: " + String::num_int64(gameObject->get_instance_id()));
}

bool Wwise::removePortal(const Object* gameObject)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SpatialAudio::RemovePortal(static_cast<AkPortalID>(gameObject->get_instance_id())),
					   "Failed to remove Portal for Game Object: " + String::num_int64(gameObject->get_instance_id()));
}

bool Wwise::setPortalObstructionAndOcclusion(const Object* portal, const float obstructionValue,
											 const float occlusionValue)
{
	AKASSERT(portal);

	return ERROR_CHECK(AK::SpatialAudio::SetPortalObstructionAndOcclusion(
						   static_cast<AkPortalID>(portal->get_instance_id()), obstructionValue, occlusionValue),
					   "Failed to set Portal Obstruction and Occlusion for Portal with ID: " +
						   String::num_int64(portal->get_instance_id()));
}

bool Wwise::setGameObjectToPortalObstruction(const Object* gameObject, const Object* portal,
											 const float obstructionValue)
{
	AKASSERT(gameObject);
	AKASSERT(portal);

	return ERROR_CHECK(AK::SpatialAudio::SetGameObjectToPortalObstruction(gameObject->get_instance_id(),
																		  portal->get_instance_id(), obstructionValue),
					   "Failed to set GameObject to Portal Obstruction");
}

bool Wwise::setPortalToPortalObstruction(const Object* portal0, const Object* portal1, const float obstructionValue)
{
	AKASSERT(portal0);
	AKASSERT(portal1);

	return ERROR_CHECK(AK::SpatialAudio::SetPortalToPortalObstruction(portal0->get_instance_id(),
																	  portal1->get_instance_id(), obstructionValue),
					   "Failed to set Portal to Portal Obstruction");
}

bool Wwise::setGameObjectInRoom(const Object* gameObject, const Object* room)
{
	AKASSERT(gameObject);
	AKASSERT(room);
	return ERROR_CHECK(AK::SpatialAudio::SetGameObjectInRoom(static_cast<AkGameObjectID>(gameObject->get_instance_id()),
															 static_cast<AkRoomID>(room->get_instance_id())),
					   "Failed to set Game Object in Room: " + String::num_int64(room->get_instance_id()));
}

bool Wwise::removeGameObjectFromRoom(const Object* gameObject)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SpatialAudio::SetGameObjectInRoom(static_cast<AkGameObjectID>(gameObject->get_instance_id()),
															 INVALID_ROOM_ID),
					   "Failed to remove Game Object from Room: " + String::num_int64(gameObject->get_instance_id()));
}

bool Wwise::setEarlyReflectionsAuxSend(const Object* gameObject, const unsigned int auxBusID)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(
		AK::SpatialAudio::SetEarlyReflectionsAuxSend(static_cast<AkGameObjectID>(gameObject->get_instance_id()),
													 static_cast<AkAuxBusID>(auxBusID)),
		"Failed to set Early Reflections Aux Send");
}

bool Wwise::setEarlyReflectionsVolume(const Object* gameObject, const float volume)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(
		AK::SpatialAudio::SetEarlyReflectionsVolume(static_cast<AkGameObjectID>(gameObject->get_instance_id()), volume),
		"Failed to set Early Reflections volume");
}

bool Wwise::addOutput(const String shareSet, const unsigned int outputID)
{
	AkOutputSettings outputSettings(shareSet.utf8().get_data(), outputID);

	return ERROR_CHECK(AK::SoundEngine::AddOutput(outputSettings),
					   "Failed to add share set to output ID: " + String::num_int64(outputID));
}

bool Wwise::removeOutput(const unsigned int outputID)
{
	return ERROR_CHECK(AK::SoundEngine::RemoveOutput(outputID),
					   "Failed to remove output ID: " + String::num_int64(outputID));
}

bool Wwise::suspend(bool renderAnyway)
{
	return ERROR_CHECK(AK::SoundEngine::Suspend(renderAnyway), "Failed to suspend SoundEngine");
}

bool Wwise::wakeupFromSuspend()
{
	return ERROR_CHECK(AK::SoundEngine::WakeupFromSuspend(), "Failed to wake up SoundEngine from suspend");
}

void Wwise::eventCallback(AkCallbackType callbackType, AkCallbackInfo* callbackInfo)
{
	AkAutoLock<CAkLock> ScopedLock(callbackDataLock);

	Callable* cookie = static_cast<Callable*>(callbackInfo->pCookie);

	if (!cookie)
	{
		ERROR_CHECK(AK_Fail, "The Event Callback cookie is not valid.");
		return;
	}
	else if (!cookie->is_valid())
	{
		return;
	}

	Array args = Array();
	Dictionary callbackData;
	callbackData["callbackType"] = static_cast<unsigned int>(callbackType);

	switch (callbackType)
	{
	case AK_EndOfEvent:
	{
		AkEventCallbackInfo* eventInfo = static_cast<AkEventCallbackInfo*>(callbackInfo);
		callbackData["eventID"] = static_cast<unsigned int>(eventInfo->eventID);
		callbackData["gameObjID"] = static_cast<unsigned int>(eventInfo->gameObjID);
		callbackData["playingID"] = static_cast<unsigned int>(eventInfo->playingID);
		break;
	}
	case AK_EndOfDynamicSequenceItem:
	{
		AkDynamicSequenceItemCallbackInfo* dynamicSequenceItemInfo =
			static_cast<AkDynamicSequenceItemCallbackInfo*>(callbackInfo);
		callbackData["audioNodeID"] = static_cast<unsigned int>(dynamicSequenceItemInfo->audioNodeID);
		callbackData["gameObjID"] = static_cast<unsigned int>(dynamicSequenceItemInfo->gameObjID);
		callbackData["playingID"] = static_cast<unsigned int>(dynamicSequenceItemInfo->playingID);
		break;
	}
	case AK_Marker:
	{
		AkMarkerCallbackInfo* markerInfo = static_cast<AkMarkerCallbackInfo*>(callbackInfo);
		callbackData["uIdentifier"] = static_cast<unsigned int>(markerInfo->uIdentifier);
		callbackData["uPosition"] = static_cast<unsigned int>(markerInfo->uPosition);
		callbackData["strLabel"] = String(markerInfo->strLabel);
		break;
	}
	case AK_Duration:
	{
		AkDurationCallbackInfo* durationInfo = static_cast<AkDurationCallbackInfo*>(callbackInfo);

		callbackData["audioNodeID"] = static_cast<unsigned int>(durationInfo->audioNodeID);
		callbackData["bStreaming"] = durationInfo->bStreaming;
		callbackData["eventID"] = static_cast<unsigned int>(durationInfo->eventID);
		callbackData["fDuration"] = static_cast<float>(durationInfo->fDuration);
		callbackData["fEstimatedDuration"] = static_cast<float>(durationInfo->fEstimatedDuration);
		callbackData["gameObjID"] = static_cast<unsigned int>(durationInfo->gameObjID);
		callbackData["mediaID"] = static_cast<unsigned int>(durationInfo->mediaID);
		callbackData["playingID"] = static_cast<unsigned int>(durationInfo->playingID);
		break;
	}
	case AK_SpeakerVolumeMatrix:
	{
		AkSpeakerVolumeMatrixCallbackInfo* speakerVolumeMatrixInfo =
			static_cast<AkSpeakerVolumeMatrixCallbackInfo*>(callbackInfo);
		callbackData["eventID"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->eventID);
		callbackData["gameObjID"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->gameObjID);

		Dictionary inputConfig;
		inputConfig["uNumChannels"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->inputConfig.uNumChannels);
		inputConfig["eConfigType"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->inputConfig.eConfigType);
		inputConfig["uChannelMask"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->inputConfig.uChannelMask);
		callbackData["inputConfig"] = inputConfig;

		Dictionary outputConfig;
		outputConfig["uNumChannels"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->outputConfig.uNumChannels);
		outputConfig["eConfigType"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->outputConfig.eConfigType);
		outputConfig["uChannelMask"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->outputConfig.uChannelMask);
		callbackData["outputConfig"] = outputConfig;

		callbackData["playingID"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->playingID);
		break;
	}
	case AK_Starvation:
	{
		AkEventCallbackInfo* eventInfo = static_cast<AkEventCallbackInfo*>(callbackInfo);
		callbackData["eventID"] = static_cast<unsigned int>(eventInfo->eventID);
		callbackData["gameObjID"] = static_cast<unsigned int>(eventInfo->gameObjID);
		callbackData["playingID"] = static_cast<unsigned int>(eventInfo->playingID);
		break;
	}
	case AK_MusicPlaylistSelect:
	{
		AkMusicPlaylistCallbackInfo* musicPlaylistInfo = static_cast<AkMusicPlaylistCallbackInfo*>(callbackInfo);
		callbackData["eventID"] = static_cast<unsigned int>(musicPlaylistInfo->eventID);
		callbackData["gameObjID"] = static_cast<unsigned int>(musicPlaylistInfo->gameObjID);
		callbackData["playingID"] = static_cast<unsigned int>(musicPlaylistInfo->playingID);
		callbackData["playlistID"] = static_cast<unsigned int>(musicPlaylistInfo->playlistID);
		callbackData["uNumPlaylistItems"] = static_cast<unsigned int>(musicPlaylistInfo->uNumPlaylistItems);
		callbackData["uPlaylistItemDone"] = static_cast<unsigned int>(musicPlaylistInfo->uPlaylistItemDone);
		callbackData["uPlaylistSelection"] = static_cast<unsigned int>(musicPlaylistInfo->uPlaylistSelection);
		break;
	}
	case AK_MusicPlayStarted:
	{
		AkEventCallbackInfo* eventInfo = static_cast<AkEventCallbackInfo*>(callbackInfo);
		callbackData["eventID"] = static_cast<unsigned int>(eventInfo->eventID);
		callbackData["gameObjID"] = static_cast<unsigned int>(eventInfo->gameObjID);
		callbackData["playingID"] = static_cast<unsigned int>(eventInfo->playingID);
		break;
	}
	case AK_MusicSyncBeat:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		callbackData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		callbackData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		callbackData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		callbackData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

		Dictionary segmentInfo;
		segmentInfo["fBarDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBarDuration);
		segmentInfo["fBeatDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBeatDuration);
		segmentInfo["fGridDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridDuration);
		segmentInfo["fGridOffset"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridOffset);
		segmentInfo["iActiveDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iActiveDuration);
		segmentInfo["iCurrentPosition"] = static_cast<int>(musicSyncInfo->segmentInfo.iCurrentPosition);
		segmentInfo["iPostExitDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPostExitDuration);
		segmentInfo["iPreEntryDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPreEntryDuration);
		segmentInfo["iRemainingLookAheadTime"] = static_cast<int>(musicSyncInfo->segmentInfo.iRemainingLookAheadTime);
		callbackData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncBar:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		callbackData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		callbackData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		callbackData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		callbackData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

		Dictionary segmentInfo;
		segmentInfo["fBarDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBarDuration);
		segmentInfo["fBeatDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBeatDuration);
		segmentInfo["fGridDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridDuration);
		segmentInfo["fGridOffset"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridOffset);
		segmentInfo["iActiveDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iActiveDuration);
		segmentInfo["iCurrentPosition"] = static_cast<int>(musicSyncInfo->segmentInfo.iCurrentPosition);
		segmentInfo["iPostExitDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPostExitDuration);
		segmentInfo["iPreEntryDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPreEntryDuration);
		segmentInfo["iRemainingLookAheadTime"] = static_cast<int>(musicSyncInfo->segmentInfo.iRemainingLookAheadTime);
		callbackData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncEntry:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		callbackData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		callbackData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		callbackData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		callbackData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

		Dictionary segmentInfo;
		segmentInfo["fBarDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBarDuration);
		segmentInfo["fBeatDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBeatDuration);
		segmentInfo["fGridDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridDuration);
		segmentInfo["fGridOffset"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridOffset);
		segmentInfo["iActiveDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iActiveDuration);
		segmentInfo["iCurrentPosition"] = static_cast<int>(musicSyncInfo->segmentInfo.iCurrentPosition);
		segmentInfo["iPostExitDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPostExitDuration);
		segmentInfo["iPreEntryDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPreEntryDuration);
		segmentInfo["iRemainingLookAheadTime"] = static_cast<int>(musicSyncInfo->segmentInfo.iRemainingLookAheadTime);
		callbackData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncExit:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		callbackData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		callbackData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		callbackData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		callbackData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

		Dictionary segmentInfo;
		segmentInfo["fBarDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBarDuration);
		segmentInfo["fBeatDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBeatDuration);
		segmentInfo["fGridDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridDuration);
		segmentInfo["fGridOffset"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridOffset);
		segmentInfo["iActiveDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iActiveDuration);
		segmentInfo["iCurrentPosition"] = static_cast<int>(musicSyncInfo->segmentInfo.iCurrentPosition);
		segmentInfo["iPostExitDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPostExitDuration);
		segmentInfo["iPreEntryDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPreEntryDuration);
		segmentInfo["iRemainingLookAheadTime"] = static_cast<int>(musicSyncInfo->segmentInfo.iRemainingLookAheadTime);
		callbackData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncGrid:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		callbackData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		callbackData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		callbackData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		callbackData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

		Dictionary segmentInfo;
		segmentInfo["fBarDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBarDuration);
		segmentInfo["fBeatDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBeatDuration);
		segmentInfo["fGridDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridDuration);
		segmentInfo["fGridOffset"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridOffset);
		segmentInfo["iActiveDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iActiveDuration);
		segmentInfo["iCurrentPosition"] = static_cast<int>(musicSyncInfo->segmentInfo.iCurrentPosition);
		segmentInfo["iPostExitDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPostExitDuration);
		segmentInfo["iPreEntryDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPreEntryDuration);
		segmentInfo["iRemainingLookAheadTime"] = static_cast<int>(musicSyncInfo->segmentInfo.iRemainingLookAheadTime);
		callbackData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncUserCue:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		callbackData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		callbackData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		callbackData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		callbackData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

		Dictionary segmentInfo;
		segmentInfo["fBarDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBarDuration);
		segmentInfo["fBeatDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBeatDuration);
		segmentInfo["fGridDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridDuration);
		segmentInfo["fGridOffset"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridOffset);
		segmentInfo["iActiveDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iActiveDuration);
		segmentInfo["iCurrentPosition"] = static_cast<int>(musicSyncInfo->segmentInfo.iCurrentPosition);
		segmentInfo["iPostExitDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPostExitDuration);
		segmentInfo["iPreEntryDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPreEntryDuration);
		segmentInfo["iRemainingLookAheadTime"] = static_cast<int>(musicSyncInfo->segmentInfo.iRemainingLookAheadTime);
		callbackData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncPoint:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		callbackData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		callbackData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		callbackData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		callbackData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

		Dictionary segmentInfo;
		segmentInfo["fBarDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBarDuration);
		segmentInfo["fBeatDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBeatDuration);
		segmentInfo["fGridDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridDuration);
		segmentInfo["fGridOffset"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridOffset);
		segmentInfo["iActiveDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iActiveDuration);
		segmentInfo["iCurrentPosition"] = static_cast<int>(musicSyncInfo->segmentInfo.iCurrentPosition);
		segmentInfo["iPostExitDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPostExitDuration);
		segmentInfo["iPreEntryDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPreEntryDuration);
		segmentInfo["iRemainingLookAheadTime"] = static_cast<int>(musicSyncInfo->segmentInfo.iRemainingLookAheadTime);
		callbackData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncAll:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		callbackData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		callbackData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		callbackData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		callbackData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

		Dictionary segmentInfo;
		segmentInfo["fBarDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBarDuration);
		segmentInfo["fBeatDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fBeatDuration);
		segmentInfo["fGridDuration"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridDuration);
		segmentInfo["fGridOffset"] = static_cast<float>(musicSyncInfo->segmentInfo.fGridOffset);
		segmentInfo["iActiveDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iActiveDuration);
		segmentInfo["iCurrentPosition"] = static_cast<int>(musicSyncInfo->segmentInfo.iCurrentPosition);
		segmentInfo["iPostExitDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPostExitDuration);
		segmentInfo["iPreEntryDuration"] = static_cast<int>(musicSyncInfo->segmentInfo.iPreEntryDuration);
		segmentInfo["iRemainingLookAheadTime"] = static_cast<int>(musicSyncInfo->segmentInfo.iRemainingLookAheadTime);
		callbackData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MIDIEvent:
	{
		AkMIDIEventCallbackInfo* midiEventInfo = static_cast<AkMIDIEventCallbackInfo*>(callbackInfo);
		callbackData["eventID"] = static_cast<unsigned int>(midiEventInfo->eventID);
		callbackData["gameObjID"] = static_cast<unsigned int>(midiEventInfo->gameObjID);

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
		programChange["byProgramNum"] = static_cast<unsigned char>(midiEventInfo->midiEvent.ProgramChange.byProgramNum);

		midiEvent["cc"] = cc;
		midiEvent["chanAftertouch"] = chanAftertouch;
		midiEvent["gen"] = gen;
		midiEvent["noteAftertouch"] = noteAftertouch;
		midiEvent["noteOnOff"] = noteOnOff;
		midiEvent["pitchBend"] = pitchBend;
		midiEvent["programChange"] = programChange;

		callbackData["midiEvent"] = midiEvent;

		callbackData["playingID"] = static_cast<unsigned int>(midiEventInfo->playingID);
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

	args.push_back(callbackData);
	cookie->callv(args);
}

void Wwise::bankCallback(AkUInt32 bankID, const void* inMemoryBankPtr, AKRESULT loadResult, void* in_pCookie)
{
	AkAutoLock<CAkLock> ScopedLock(callbackDataLock);

	Callable* cookie = static_cast<Callable*>(in_pCookie);

	if (!cookie)
	{
		ERROR_CHECK(AK_Fail, "The Bank Callback cookie is not valid.");
		return;
	}
	else if (!cookie->is_valid())
	{
		UtilityFunctions::push_warning("The Bank Callback cookie object no longer exists.", __FUNCTION__, __FILE__,
									   itos( __LINE__));
		return;
	}

	Array args = Array();
	Dictionary callbackData;
	callbackData["bankID"] = static_cast<unsigned int>(bankID);
	callbackData["result"] = static_cast<unsigned int>(loadResult);

	args.push_back(callbackData);
	cookie->callv(args);
}

Variant Wwise::getPlatformProjectSetting(const String setting)
{
	AKASSERT(projectSettings);
	AKASSERT(!setting.is_empty());

	String platformSetting = setting;

#ifdef AK_WIN
	platformSetting += GODOT_WINDOWS_SETTING_POSTFIX;
#elif defined(AK_MAC_OS_X)
	platformSetting += GODOT_MAC_OSX_SETTING_POSTFIX;
#elif defined(AK_IOS)
	platformSetting += GODOT_IOS_SETTING_POSTFIX;
#elif defined(AK_ANDROID)
	platformSetting += GODOT_ANDROID_SETTING_POSTFIX;
#elif defined(AK_LINUX)
	platformSetting += GODOT_LINUX_SETTING_POSTFIX;
#else
#error "Platform not supported"
#endif

	// Try to get the platform-specific setting, if it exists
	if (projectSettings && projectSettings->has_setting(platformSetting))
	{
		return projectSettings->get(platformSetting);
	}

	// Otherwise, try to get the default platform-agnostic setting
	if (projectSettings && projectSettings->has_setting(setting))
	{
		return projectSettings->get(setting);
	}
	else
	{
		AKASSERT(false);
		UtilityFunctions::print("Failed to get setting " + platformSetting);
		return "";
	}
}

bool Wwise::initialiseWwiseSystems()
{
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);
	if (!ERROR_CHECK(AK::MemoryMgr::Init(&memSettings), "Memory manager initialisation failed"))
	{
		return false;
	}

	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);
	if (!AK::StreamMgr::Create(stmSettings))
	{
		return false;
	}

	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	deviceSettings.bUseStreamCache =
		static_cast<bool>(getPlatformProjectSetting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "use_stream_cache"));

	deviceSettings.fTargetAutoStmBufferLength = static_cast<float>(
		getPlatformProjectSetting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "target_auto_stream_buffer_length_ms"));

	deviceSettings.uIOMemorySize =
		static_cast<unsigned int>(getPlatformProjectSetting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "IO_memory_size"));

	deviceSettings.uMaxCachePinnedBytes = static_cast<unsigned int>(
		getPlatformProjectSetting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "maximum_pinned_bytes_in_cache"));

	deviceSettings.uSchedulerTypeFlags = AK_SCHEDULER_BLOCKING;

	if (!ERROR_CHECK(lowLevelIO.Init(deviceSettings), "Initialising Low level IO failed"))
	{
		return false;
	}

	AkInitSettings initSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);

#if defined(AK_ENABLE_ASSERTS)
	initSettings.pfnAssertHook = WwiseAssertHook;
#endif

	initSettings.bDebugOutOfRangeCheckEnabled = static_cast<bool>(
		getPlatformProjectSetting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "debug_out_of_range_check_enabled"));

	initSettings.bEnableGameSyncPreparation = static_cast<bool>(
		getPlatformProjectSetting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "enable_game_sync_preparation"));

	initSettings.fDebugOutOfRangeLimit =
		static_cast<float>(getPlatformProjectSetting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "debug_out_of_range_limit"));

	String audioDeviceShareSet =
		getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "main_output/audio_device_shareset");
	initSettings.settingsMainOutput.audioDeviceShareset =
		AK::SoundEngine::GetIDFromString(audioDeviceShareSet.utf8().get_data());

	const unsigned int channelConfigType = static_cast<unsigned int>(
		getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "main_output/channel_config/channel_config_type"));

	const unsigned int numChannels = static_cast<unsigned int>(
		getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "main_output/channel_config/number_of_channels"));

	if (channelConfigType == AK_ChannelConfigType_Anonymous)
	{
		initSettings.settingsMainOutput.channelConfig.SetAnonymous(numChannels);
	}
	else if (channelConfigType == AK_ChannelConfigType_Standard)
	{
		initSettings.settingsMainOutput.channelConfig.SetStandard(numChannels);
	}
	else if (channelConfigType == AK_ChannelConfigType_Ambisonic)
	{
		initSettings.settingsMainOutput.channelConfig.SetAmbisonic(numChannels);
	}
	else
	{
		AKASSERT(false);
	}

	initSettings.settingsMainOutput.idDevice =
		static_cast<unsigned int>(getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "main_output/device_id"));

	initSettings.settingsMainOutput.ePanningRule = static_cast<AkPanningRule>(static_cast<unsigned int>(
		getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "main_output/panning_rule")));

	initSettings.uCommandQueueSize =
		static_cast<unsigned int>(getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "command_queue_size"));

	initSettings.uContinuousPlaybackLookAhead = static_cast<unsigned int>(
		getPlatformProjectSetting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "continuous_playback_look_ahead"));

	initSettings.uMaxHardwareTimeoutMs = static_cast<unsigned int>(
		getPlatformProjectSetting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "maximum_hardware_timeout_ms"));

	initSettings.uMaxNumPaths = static_cast<unsigned int>(
		getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "maximum_number_of_positioning_paths"));

	initSettings.uMonitorQueuePoolSize = static_cast<unsigned int>(
		getPlatformProjectSetting(WWISE_COMMON_ADVANCED_SETTINGS_PATH + "monitor_queue_pool_size"));

	const unsigned int numSamplesPerFrameEnum =
		static_cast<unsigned int>(getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "samples_per_frame"));

	switch (numSamplesPerFrameEnum)
	{
	case SamplesPerFrame::SAMPLES_256:
		initSettings.uNumSamplesPerFrame = 256;
		break;
	case SamplesPerFrame::SAMPLES_512:
		initSettings.uNumSamplesPerFrame = 512;
		break;
	case SamplesPerFrame::SAMPLES_1024:
		initSettings.uNumSamplesPerFrame = 1024;
		break;
	case SamplesPerFrame::SAMPLES_2048:
		initSettings.uNumSamplesPerFrame = 2048;
		break;
	default:
		AKASSERT(false);
		break;
	}

	initSettings.eFloorPlane = AkFloorPlane_Default; // todo(alex): check this;

	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	// Common platform settings
	const unsigned int numRefillsInVoiceEnum = static_cast<unsigned int>(
		getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "number_of_refills_in_voice"));

	switch (numRefillsInVoiceEnum)
	{
	case NumRefillsInVoice::REFILLS_2:
		platformInitSettings.uNumRefillsInVoice = 2;
		break;
	case NumRefillsInVoice::REFILLS_4:
		platformInitSettings.uNumRefillsInVoice = 4;
		break;
	default:
		AKASSERT(false);
		break;
	}

	const unsigned int sampleRateEnum =
		static_cast<unsigned int>(getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "sample_rate"));

	switch (sampleRateEnum)
	{
	case SampleRate::RATE_16000:
		platformInitSettings.uSampleRate = 16000;
		break;
	case SampleRate::RATE_24000:
		platformInitSettings.uSampleRate = 24000;
		break;
	case SampleRate::RATE_32000:
		platformInitSettings.uSampleRate = 32000;
		break;
	case SampleRate::RATE_44100:
		platformInitSettings.uSampleRate = 44100;
		break;
	case SampleRate::RATE_48000:
		platformInitSettings.uSampleRate = 48000;
		break;
	default:
		AKASSERT(false);
		break;
	}

	// Platform-specific settings
#ifdef AK_WIN
	int64_t handle = DisplayServer::get_singleton()->window_get_native_handle(DisplayServer::HandleType::WINDOW_HANDLE);
	HWND hwnd = reinterpret_cast<HWND>(handle);
	platformInitSettings.hWnd = hwnd;

#elif defined(AK_MAC_OS_X)

#elif defined(AK_IOS)
	const unsigned int sessionCategoryEnum =
		static_cast<unsigned int>(getPlatformProjectSetting("wwise/ios_advanced_settings/audio_session_category"));

	platformInitSettings.audioSession.eCategory = static_cast<AkAudioSessionCategory>(sessionCategoryEnum);

	const unsigned int sessionCategoryFlags = static_cast<unsigned int>(
		getPlatformProjectSetting("wwise/ios_advanced_settings/audio_session_category_options"));

	platformInitSettings.audioSession.eCategoryOptions =
		static_cast<AkAudioSessionCategoryOptions>(sessionCategoryFlags);

	const unsigned int audioSessionModeEnum =
		static_cast<unsigned int>(getPlatformProjectSetting("wwise/ios_advanced_settings/audio_session_mode"));

	platformInitSettings.audioSession.eMode = static_cast<AkAudioSessionMode>(audioSessionModeEnum);

#elif defined(AK_ANDROID)

	platformInitSettings.eAudioAPI = static_cast<AkAudioAPIAndroid>(
		static_cast<unsigned int>(getPlatformProjectSetting("wwise/android_advanced_settings/audio_API")));

	platformInitSettings.bRoundFrameSizeToHWSize =
		static_cast<bool>(getPlatformProjectSetting("wwise/android_advanced_settings/round_frame_size_to_hw_size"));

	JNIEnv* env = godot::android_api->godot_android_get_env();

	JavaVM* javaVM;
	env->GetJavaVM(&javaVM);
	platformInitSettings.pJavaVM = javaVM;

	jobject activity = godot::android_api->godot_android_get_activity();
	platformInitSettings.jActivity = activity;

#elif defined(AK_LINUX)

	platformInitSettings.eAudioAPI = static_cast<AkAudioAPILinux>(
		static_cast<unsigned int>(getPlatformProjectSetting("wwise/linux_advanced_settings/audio_API")));

#else
#error "Platform not supported"
#endif

	if (!ERROR_CHECK(AK::SoundEngine::Init(&initSettings, &platformInitSettings), "Sound engine initialisation failed"))
	{
		return false;
	}

	AkMusicSettings musicInitSettings;
	AK::MusicEngine::GetDefaultInitSettings(musicInitSettings);

	musicInitSettings.fStreamingLookAheadRatio =
		static_cast<float>(getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + "streaming_look_ahead_ratio"));

	if (!ERROR_CHECK(AK::MusicEngine::Init(&musicInitSettings), "Music engine initialisation failed"))
	{
		return false;
	}

	AkSpatialAudioInitSettings spatialSettings;

	spatialSettings.uMaxSoundPropagationDepth = static_cast<unsigned int>(getPlatformProjectSetting(
		WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "max_sound_propagation_depth"));

	spatialSettings.bCalcEmitterVirtualPosition = static_cast<bool>(getPlatformProjectSetting(
		WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "calc_emitter_virtual_position"));

	spatialSettings.fMovementThreshold = static_cast<float>(
		getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "movement_threshold"));

	spatialSettings.uNumberOfPrimaryRays = static_cast<unsigned int>(getPlatformProjectSetting(
		WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "number_of_primary_rays"));

	spatialSettings.uMaxReflectionOrder = static_cast<unsigned int>(
		getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "max_reflection_order"));

	spatialSettings.fMaxPathLength = static_cast<float>(
		getPlatformProjectSetting(WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "max_path_length"));

	spatialSettings.bEnableGeometricDiffractionAndTransmission = static_cast<bool>(getPlatformProjectSetting(
		WWISE_COMMON_USER_SETTINGS_PATH + WWISE_SPATIAL_AUDIO_PATH + "enable_geometric_diffraction_and_transmission"));

	if (!ERROR_CHECK(AK::SpatialAudio::Init(spatialSettings), "Spatial Audio initialisation failed"))
	{
		return false;
	}

#ifndef AK_OPTIMIZED
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);

	commSettings.bInitSystemLib =
		static_cast<bool>(getPlatformProjectSetting(WWISE_COMMUNICATION_SETTINGS_PATH + "initialize_system_comms"));

	// note(alex): Default value: 24024
	commSettings.ports.uCommand =
		static_cast<unsigned int>(getPlatformProjectSetting(WWISE_COMMUNICATION_SETTINGS_PATH + "command_port"));

	commSettings.ports.uDiscoveryBroadcast = static_cast<unsigned int>(
		getPlatformProjectSetting(WWISE_COMMUNICATION_SETTINGS_PATH + "discovery_broadcast_port"));

	// todo(alex): remove notification_port from settings

	String networkName = getPlatformProjectSetting(WWISE_COMMUNICATION_SETTINGS_PATH + "network_name");
	AKPLATFORM::SafeStrCpy(commSettings.szAppNetworkName, networkName.utf8().get_data(),
						   AK_COMM_SETTINGS_MAX_STRING_SIZE);

	ERROR_CHECK(AK::Comm::Init(commSettings), "Comm initialisation failed");
#endif

	return true;
}

bool Wwise::shutdownWwiseSystems()
{
#ifndef AK_OPTIMIZED
	AK::Comm::Term();
#endif

	if (!ERROR_CHECK(AK::SoundEngine::UnregisterAllGameObj(), "Unregister all game obj failed"))
	{
		return false;
	}

	if (!ERROR_CHECK(AK::SoundEngine::ClearBanks(), "Clear banks failed"))
	{
		return false;
	}

	AK::MusicEngine::Term();

	AK::SoundEngine::Term();

	lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}

	AK::MemoryMgr::Term();

	return true;
}
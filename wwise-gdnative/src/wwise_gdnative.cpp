#include "wwise_gdnative.h"

#include <AK/Plugin/AllPluginsFactories.h>

using namespace godot;

Mutex* Wwise::signalDataMutex;
Array* Wwise::signalDataArray;

namespace AK
{
	void* __cdecl AllocHook(size_t in_size)
	{
		Godot::print("AK::AllocHook called with size: " + String::num_int64(in_size));
		AKASSERT(api);
		return api->godot_alloc(static_cast<int>(in_size));
	}

	void __cdecl FreeHook(void* in_ptr)
	{
		Godot::print("AK::FreeHook called");
		AKASSERT(api);
		api->godot_free(in_ptr);
	}

#if defined(_WIN32)
	void* __cdecl VirtualAllocHook(
		void* in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
	)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void __cdecl VirtualFreeHook(
		void* in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
	)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}

Wwise::~Wwise() 
{
	shutdownWwiseSystems();

	signalDataMutex->free();
	delete signalDataArray;

	Godot::print("Wwise has shut down");
}

#define REGISTER_GODOT_SIGNAL(callbackType) \
	register_signal<Wwise>(WwiseCallbackToSignal(callbackType), "data", GODOT_VARIANT_TYPE_DICTIONARY);

void Wwise::_register_methods()
{
	register_method("_process", &Wwise::_process);
	register_method("load_bank", &Wwise::loadBank);
	register_method("load_bank_id", &Wwise::loadBankID);
	register_method("unload_bank", &Wwise::unloadBank);
	register_method("unload_bank_id", &Wwise::unloadBankID);
	register_method("register_listener", &Wwise::registerListener);
	register_method("register_game_obj", &Wwise::registerGameObject);
	register_method("set_3d_position", &Wwise::set3DPosition);
	register_method("set_2d_position", &Wwise::set2DPosition);
	register_method("post_event", &Wwise::postEvent);
	register_method("post_event_callback", &Wwise::postEventCallback);
	register_method("post_event_id", &Wwise::postEventID);
	register_method("post_event_id_callback", &Wwise::postEventIDCallback);
	register_method("stop_event", &Wwise::stopEvent);
	register_method("set_switch", &Wwise::setSwitch);
	register_method("set_switch_id", &Wwise::setSwitchID);
	register_method("set_state", &Wwise::setState);
	register_method("set_state_id", &Wwise::setStateID);
	register_method("get_rtpc", &Wwise::getRTPCValue);
	register_method("get_rtpc_id", &Wwise::getRTPCValueID);
	register_method("set_rtpc", &Wwise::setRTPCValue);
	register_method("set_rtpc_id", &Wwise::setRTPCValueID);
	register_method("post_trigger", &Wwise::postTrigger);
	register_method("post_trigger_id", &Wwise::postTriggerID);
	register_method("post_external_source", &Wwise::postExternalSource);
	register_method("post_external_source_id", &Wwise::postExternalSourceID);
	register_method("get_source_play_position", &Wwise::getSourcePlayPosition);
	register_method("get_playing_segment_info", &Wwise::getPlayingSegmentInfo);
	register_method("set_game_obj_output_bus_volume", &Wwise::setGameObjectOutputBusVolume);
	register_method("set_game_obj_aux_send_values", &Wwise::setGameObjectAuxSendValues);
	register_method("set_obj_obstruction_and_occlusion", &Wwise::setObjectObstructionAndOcclusion); 

	REGISTER_GODOT_SIGNAL(AK_EndOfEvent);
	REGISTER_GODOT_SIGNAL(AK_EndOfDynamicSequenceItem);
	REGISTER_GODOT_SIGNAL(AK_Marker);
	REGISTER_GODOT_SIGNAL(AK_Duration);
	REGISTER_GODOT_SIGNAL(AK_SpeakerVolumeMatrix);
	REGISTER_GODOT_SIGNAL(AK_Starvation);
	REGISTER_GODOT_SIGNAL(AK_MusicPlaylistSelect);
	REGISTER_GODOT_SIGNAL(AK_MusicPlayStarted);
	REGISTER_GODOT_SIGNAL(AK_MusicSyncBeat);
	REGISTER_GODOT_SIGNAL(AK_MusicSyncBar);
	REGISTER_GODOT_SIGNAL(AK_MusicSyncEntry);
	REGISTER_GODOT_SIGNAL(AK_MusicSyncExit);
	REGISTER_GODOT_SIGNAL(AK_MusicSyncGrid);
	REGISTER_GODOT_SIGNAL(AK_MusicSyncUserCue);
	REGISTER_GODOT_SIGNAL(AK_MusicSyncPoint);
	REGISTER_GODOT_SIGNAL(AK_MusicSyncAll);
	REGISTER_GODOT_SIGNAL(AK_MIDIEvent);
	REGISTER_GODOT_SIGNAL(AK_CallbackBits);
	REGISTER_GODOT_SIGNAL(AK_EnableGetSourcePlayPosition);
	REGISTER_GODOT_SIGNAL(AK_EnableGetMusicPlayPosition);
	REGISTER_GODOT_SIGNAL(AK_EnableGetSourceStreamBuffering);
}

void Wwise::_init()
{
	signalDataMutex = Mutex::_new();
	signalDataArray = new Array();
	projectSettings = ProjectSettings::get_singleton();
	AKASSERT(projectSettings);

	bool initialisationResult = initialiseWwiseSystems();

	if (!initialisationResult)
	{
		ERROR_CHECK(AK_Fail, "Wwise systems initialisation failed!");
	}
	else
	{
		Godot::print("Wwise systems initialisation succeeded");
	}

	String basePath = getPlatformProjectSetting("wwise/common_user_settings/base_path");
	MAP_PATH(basePath);

#ifdef AK_WIN
	basePath += "/Windows";
#elif defined(AK_MAC_OS_X)
	basePath += "/Mac";
#elif defined(AK_IOS)
	basePath += "/iOS";
#elif defined(AK_ANDROID)
	basePath += "/Android";
#else
#error "Platform not supported"
#endif

	setBasePath(basePath);
}

void Wwise::_process(const float delta)
{
	emitSignals();
	ERROR_CHECK(AK::SoundEngine::RenderAudio(), "");
}

bool Wwise::setBasePath(const String basePath)
{
	AKASSERT(!basePath.empty());

	AkOSChar* basePathOsString = nullptr;

	const wchar_t* basePathChar = basePath.unicode_str();
	CONVERT_WIDE_TO_OSCHAR(basePathChar, basePathOsString);
	AKASSERT(basePathOsString);

	return ERROR_CHECK(lowLevelIO.SetBasePath(basePathOsString), basePath);
}

bool Wwise::loadBank(const String bankName)
{
	AkBankID bankID;
	AKASSERT(!bankName.empty());

	return ERROR_CHECK(AK::SoundEngine::LoadBank(bankName.unicode_str(), bankID), bankName);
}

bool Wwise::loadBankID(const unsigned int bankID)
{
	return ERROR_CHECK(AK::SoundEngine::LoadBank(bankID), "ID " + String::num_int64(bankID));
}

bool Wwise::unloadBank(const String bankName)
{
	AKASSERT(!bankName.empty());

	return ERROR_CHECK(AK::SoundEngine::UnloadBank(bankName.unicode_str(), NULL), bankName);
}

bool Wwise::unloadBankID(const unsigned int bankID)
{
	return ERROR_CHECK(AK::SoundEngine::UnloadBank(bankID, NULL), "ID " + String::num_int64(bankID) + " failed");
}

bool Wwise::registerListener(const Object* gameObject)
{
	AKASSERT(gameObject);

	const AkGameObjectID listener = static_cast<AkGameObjectID>(gameObject->get_instance_id());

	if (!ERROR_CHECK(AK::SoundEngine::RegisterGameObj(listener, "Default Listener"), "ID " + String::num_int64(listener)))
	{
		return false;
	}

	if (!ERROR_CHECK(AK::SoundEngine::SetDefaultListeners(&listener, 1), "ID " + String::num_int64(listener)))
	{
		return false;
	}

	return true;
}

bool Wwise::registerGameObject(const Object* gameObject, const String gameObjectName)
{
	AKASSERT(gameObject);
	AKASSERT(!gameObjectName.empty());

	return ERROR_CHECK(AK::SoundEngine::RegisterGameObj(static_cast<AkGameObjectID>(gameObject->get_instance_id()), 
						gameObjectName.alloc_c_string()), gameObjectName);
}

bool Wwise::set3DPosition(const Object* gameObject, const Transform transform)
{
	AKASSERT(gameObject);

	AkSoundPosition soundPos;

	AkVector position;	GetAkVector(transform, position, VectorType::POSITION);
	AkVector forward;	GetAkVector(transform, forward, VectorType::FORWARD);
	AkVector up;		GetAkVector(transform, up, VectorType::UP);

	soundPos.Set(position, forward, up);

	return ERROR_CHECK(AK::SoundEngine::SetPosition(static_cast<AkGameObjectID>(gameObject->get_instance_id()), soundPos),
						"Game object ID " + String::num_int64(gameObject->get_instance_id()));
}

bool Wwise::set2DPosition(const Object* gameObject, const Transform2D transform2D, const float zDepth)
{
	AKASSERT(gameObject);

	AkSoundPosition soundPos;

	Vector2 origin = transform2D.get_origin();

	Vector3 position = Vector3(origin.x * 0.1f, -origin.y * 0.1f, zDepth);
	Vector3 forward = Vector3(transform2D.elements[1].x, 0, transform2D.elements[1].y).normalized();
	Vector3 up = Vector3(0, 1, 0);

	AkVector akPosition;	Vector3ToAkVector(position, akPosition);
	AkVector akForward;		Vector3ToAkVector(forward, akForward);
	AkVector akUp;			Vector3ToAkVector(up, akUp);

	soundPos.Set(akPosition, akForward, akUp);

	return ERROR_CHECK(AK::SoundEngine::SetPosition(static_cast<AkGameObjectID>(gameObject->get_instance_id()), soundPos),
						"Game object ID " + String::num_int64(gameObject->get_instance_id()));
}

unsigned int Wwise::postEvent(const String eventName, const Object* gameObject)
{
	AKASSERT(!eventName.empty());
	AKASSERT(gameObject);

	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventName.unicode_str(), static_cast<AkGameObjectID>(gameObject->get_instance_id()));

	if (playingID == AK_INVALID_PLAYING_ID) 
	{
		ERROR_CHECK(AK_InvalidID, eventName);
		return static_cast<unsigned int>(AK_INVALID_PLAYING_ID);
	}

	return static_cast<unsigned int>(playingID);
}

unsigned int Wwise::postEventCallback(const String eventName, const unsigned int flags, const Object* gameObject)
{
	AKASSERT(!eventName.empty());
	AKASSERT(gameObject);

	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventName.unicode_str(), static_cast<AkGameObjectID>(gameObject->get_instance_id()),
														flags, eventCallback);

	if (playingID == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK(AK_InvalidID, eventName);
		return static_cast<unsigned int>(AK_INVALID_PLAYING_ID);
	}

	return static_cast<unsigned int>(playingID);
}

unsigned int Wwise::postEventID(const unsigned int eventID, const Object* gameObject)
{
	AKASSERT(gameObject);

	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventID, static_cast<AkGameObjectID>(gameObject->get_instance_id()));

	if (playingID == AK_INVALID_PLAYING_ID) 
	{
		return static_cast<unsigned int>(AK_INVALID_PLAYING_ID);
	}

	return static_cast<unsigned int>(playingID);
}

unsigned int Wwise::postEventIDCallback(const unsigned int eventID, const unsigned int flags, const Object* gameObject)
{
	AKASSERT(gameObject);

	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventID, static_cast<AkGameObjectID>(gameObject->get_instance_id()), flags,
														eventCallback);

	if (playingID == AK_INVALID_PLAYING_ID)
	{
		return static_cast<unsigned int>(AK_INVALID_PLAYING_ID);
	}

	return static_cast<unsigned int>(playingID);
}

bool Wwise::stopEvent(const int playingID, const int fadeTime, const int interpolation)
{
	AKASSERT(fadeTime >= 0);

	AK::SoundEngine::StopPlayingID(static_cast<AkPlayingID>(playingID), static_cast<AkTimeMs>(fadeTime), 
									static_cast<AkCurveInterpolation>(interpolation));

	return true;
}

bool Wwise::setSwitch(const String switchGroup, const String switchState, const Object* gameObject)
{
	AKASSERT(!switchGroup.empty());
	AKASSERT(!switchState.empty());
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SoundEngine::SetSwitch(switchGroup.unicode_str(), 
						switchState.unicode_str(), 
						static_cast<AkGameObjectID>(gameObject->get_instance_id())),
						"Switch " + switchGroup + " and state " + switchState);
}

bool Wwise::setSwitchID(const unsigned int switchGroupID, const unsigned int switchStateID, const Object* gameObject)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SoundEngine::SetSwitch(switchGroupID, switchStateID, static_cast<AkGameObjectID>(gameObject->get_instance_id())),
						"Switch ID " + String::num_int64(switchGroupID) + 
						" and switch state ID " + String::num_int64(switchStateID));
}

bool Wwise::setState(const String stateGroup, const String stateValue)
{
	AKASSERT(!stateGroup.empty());
	AKASSERT(!stateValue.empty());

	return ERROR_CHECK(AK::SoundEngine::SetState(stateGroup.unicode_str(), stateValue.unicode_str()),
						"Failed to set state " + stateGroup + " and value " + stateValue);
}

bool Wwise::setStateID(const unsigned int stateGroupID, const unsigned int stateValueID)
{
	return ERROR_CHECK(AK::SoundEngine::SetState(stateGroupID, stateValueID),
						"Failed to set state ID" + String::num_int64(stateGroupID) + " and value " + String::num_int64(stateValueID));
}

// todo: global rtpc
float Wwise::getRTPCValue(const String rtpcName, const Object* gameObject)
{
	AKASSERT(!rtpcName.empty());
	AKASSERT(gameObject);

	AkRtpcValue value;
	AK::SoundEngine::Query::RTPCValue_type type = AK::SoundEngine::Query::RTPCValue_GameObject;

	if (!ERROR_CHECK(AK::SoundEngine::Query::GetRTPCValue(rtpcName.unicode_str(), 
		static_cast<AkGameObjectID>(gameObject->get_instance_id()),
		static_cast<AkPlayingID>(0), value, type), rtpcName))
	{
		return INVALID_RTPC_VALUE;
	}

	return static_cast<float>(value);
}

float Wwise::getRTPCValueID(const unsigned int rtpcID, const Object* gameObject)
{
	AKASSERT(gameObject);

	AkRtpcValue value;
	AK::SoundEngine::Query::RTPCValue_type type = AK::SoundEngine::Query::RTPCValue_GameObject;

	if (!ERROR_CHECK(AK::SoundEngine::Query::GetRTPCValue(rtpcID, static_cast<AkGameObjectID>(gameObject->get_instance_id()),
		static_cast<AkPlayingID>(0), value, type), String::num_int64(rtpcID)))
	{
		return INVALID_RTPC_VALUE;
	}

	return static_cast<float>(value);
}

// todo: global rtpc
bool Wwise::setRTPCValue(const String rtpcName, const float rtpcValue, const Object* gameObject)
{
	AKASSERT(!rtpcName.empty());
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SoundEngine::SetRTPCValue(rtpcName.unicode_str(), static_cast<AkRtpcValue>(rtpcValue), 
						static_cast<AkGameObjectID>(gameObject->get_instance_id())), rtpcName);
}

bool Wwise::setRTPCValueID(const unsigned int rtpcID, const float rtpcValue, const Object* gameObject)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SoundEngine::SetRTPCValue(rtpcID, static_cast<AkRtpcValue>(rtpcValue), 
						static_cast<AkGameObjectID>(gameObject->get_instance_id())), String::num_int64(rtpcID));
}

bool Wwise::postTrigger(const String triggerName, const Object* gameObject)
{
	AKASSERT(!triggerName.empty());
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SoundEngine::PostTrigger(triggerName.unicode_str(),
						static_cast<AkGameObjectID>(gameObject->get_instance_id())), "Failed to post trigger " + triggerName);
}

bool Wwise::postTriggerID(const unsigned int triggerID, const Object* gameObject)
{
	AKASSERT(gameObject);

	return ERROR_CHECK(AK::SoundEngine::PostTrigger(triggerID,
						static_cast<AkGameObjectID>(gameObject->get_instance_id())), "Failed to post trigger ID " + String::num_int64(triggerID));
}

unsigned int Wwise::postExternalSource(const String eventName, const Object* gameObject, const String sourceObjectName, const String fileName, 
	const unsigned int idCodec)
{
	AKASSERT(!eventName.empty());
	AKASSERT(gameObject);
	AKASSERT(!sourceObjectName.empty());
	AKASSERT(!fileName.empty());

	AkExternalSourceInfo source;
	source.iExternalSrcCookie = AK::SoundEngine::GetIDFromString(sourceObjectName.unicode_str());

	AkOSChar* szFileOsString = nullptr;

	CONVERT_WIDE_TO_OSCHAR(fileName.unicode_str(), szFileOsString);

	source.szFile = szFileOsString;
	source.idCodec = idCodec;

	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventName.unicode_str(), static_cast<AkGameObjectID>(gameObject->get_instance_id()), 0, nullptr, 0, 1, &source);

	if (playingID == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK(AK_InvalidID, eventName);
		return static_cast<unsigned int>(AK_INVALID_PLAYING_ID);
	}

	return static_cast<unsigned int>(playingID);
}

unsigned int Wwise::postExternalSourceID(const unsigned int eventID, const Object* gameObject, const unsigned int sourceObjectID, 
	const String fileName, const unsigned int idCodec)
{
	AKASSERT(gameObject);
	AKASSERT(!fileName.empty());

	AkExternalSourceInfo source;
	source.iExternalSrcCookie = sourceObjectID;

	AkOSChar* szFileOsString = nullptr;

	CONVERT_WIDE_TO_OSCHAR(fileName.unicode_str(), szFileOsString);

	source.szFile = szFileOsString;
	source.idCodec = idCodec;

	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventID, static_cast<AkGameObjectID>(gameObject->get_instance_id()), 0, NULL, 0, 1, &source);

	if (playingID == AK_INVALID_PLAYING_ID)
	{
		ERROR_CHECK(AK_InvalidID, eventID);
		return static_cast<unsigned int>(AK_INVALID_PLAYING_ID);
	}

	return static_cast<unsigned int>(playingID);
}

int Wwise::getSourcePlayPosition(const unsigned int playingID, const bool extrapolate)
{
	AkTimeMs position;
	AKRESULT result = AK::SoundEngine::GetSourcePlayPosition(static_cast<AkPlayingID>(playingID), &position, extrapolate);

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
	AKRESULT result = AK::MusicEngine::GetPlayingSegmentInfo(static_cast<AkPlayingID>(playingID), segmentInfo, extrapolate);

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

bool Wwise::setGameObjectOutputBusVolume(const unsigned int eventID, const unsigned int listenerID, float fControlValue)
{
	return ERROR_CHECK(AK::SoundEngine::SetGameObjectOutputBusVolume(static_cast<AkGameObjectID>(eventID), 
																		static_cast<AkGameObjectID>(listenerID), fControlValue), "Could not set the Game Object Outpus Bus Volume");
}

bool Wwise::setGameObjectAuxSendValues(const unsigned int eventID, const Array akAuxSendValues, const unsigned int sendValues)
{
	AkAuxSendValue environments[AK_MAX_ENVIRONMENTS];

	for (int i = 0; i < akAuxSendValues.size(); i++)
	{
		Dictionary auxBusData = akAuxSendValues[i];
		environments[i].auxBusID = static_cast<const unsigned int>(auxBusData["aux_bus_id"]);
		environments[i].fControlValue = static_cast<float>(auxBusData["control_value"]);
		environments[i].listenerID = AK_INVALID_GAME_OBJECT;
	}

	return ERROR_CHECK(AK::SoundEngine::SetGameObjectAuxSendValues(static_cast<AkGameObjectID>(eventID),
																	environments, sendValues), "Could not set the Game Object Aux Send Values");
}

bool Wwise::setObjectObstructionAndOcclusion(const unsigned int eventID, const unsigned int listenerID, float fCalculatedObs, float fCalculatedOcc)
{
	return ERROR_CHECK(AK::SoundEngine::SetObjectObstructionAndOcclusion(static_cast<AkGameObjectID>(eventID), 
																			static_cast<AkGameObjectID>(listenerID), fCalculatedObs, fCalculatedOcc), "Could not set Obstruction and Occlusion");
}

void Wwise::eventCallback(AkCallbackType callbackType, AkCallbackInfo* callbackInfo)
{
	signalDataMutex->lock();

	Dictionary signalData;
	signalData["callbackType"] = static_cast<unsigned int>(callbackType);

	switch (callbackType)
	{
	case AK_EndOfEvent:
	{
		AkEventCallbackInfo* eventInfo = static_cast<AkEventCallbackInfo*>(callbackInfo);
		signalData["eventID"] = static_cast<unsigned int>(eventInfo->eventID);
		signalData["gameObjID"] = static_cast<unsigned int>(eventInfo->gameObjID);
		signalData["playingID"] = static_cast<unsigned int>(eventInfo->playingID);
		break;
	}
	case AK_EndOfDynamicSequenceItem:
	{
		AkDynamicSequenceItemCallbackInfo* dynamicSequenceItemInfo = static_cast<AkDynamicSequenceItemCallbackInfo*>(callbackInfo);
		signalData["audioNodeID"] = static_cast<unsigned int>(dynamicSequenceItemInfo->audioNodeID);
		signalData["gameObjID"] = static_cast<unsigned int>(dynamicSequenceItemInfo->gameObjID);
		signalData["playingID"] = static_cast<unsigned int>(dynamicSequenceItemInfo->playingID);
		break;
	}
	case AK_Marker:
	{
		AkMarkerCallbackInfo* markerInfo = static_cast<AkMarkerCallbackInfo*>(callbackInfo);
		signalData["uIdentifier"] = static_cast<unsigned int>(markerInfo->uIdentifier);
		signalData["uPosition"] = static_cast<unsigned int>(markerInfo->uPosition);
		signalData["strLabel"] = String(markerInfo->strLabel);
		break;
	}
	case AK_Duration:
	{
		AkDurationCallbackInfo* durationInfo = static_cast<AkDurationCallbackInfo*>(callbackInfo);
		signalData["audioNodeID"] = static_cast<unsigned int>(durationInfo->audioNodeID);
		signalData["bStreaming"] = durationInfo->bStreaming;
		signalData["eventID"] = static_cast<unsigned int>(durationInfo->eventID);
		signalData["fDuration"] = static_cast<float>(durationInfo->fDuration);
		signalData["fEstimatedDuration"] = static_cast<float>(durationInfo->fEstimatedDuration);
		signalData["gameObjID"] = static_cast<unsigned int>(durationInfo->gameObjID);
		signalData["mediaID"] = static_cast<unsigned int>(durationInfo->mediaID);
		signalData["playingID"] = static_cast<unsigned int>(durationInfo->playingID);
		break;
	}
	case AK_SpeakerVolumeMatrix:
	{
		AkSpeakerVolumeMatrixCallbackInfo* speakerVolumeMatrixInfo = static_cast<AkSpeakerVolumeMatrixCallbackInfo*>(callbackInfo);
		signalData["eventID"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->eventID);
		signalData["gameObjID"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->gameObjID);

		Dictionary inputConfig;
		inputConfig["uNumChannels"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->inputConfig.uNumChannels);
		inputConfig["eConfigType"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->inputConfig.eConfigType);
		inputConfig["uChannelMask"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->inputConfig.uChannelMask);
		signalData["inputConfig"] = inputConfig;

		Dictionary outputConfig;
		outputConfig["uNumChannels"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->outputConfig.uNumChannels);
		outputConfig["eConfigType"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->outputConfig.eConfigType);
		outputConfig["uChannelMask"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->outputConfig.uChannelMask);
		signalData["outputConfig"] = outputConfig;

		signalData["playingID"] = static_cast<unsigned int>(speakerVolumeMatrixInfo->playingID);
		break;
	}
	case AK_Starvation:
	{
		AkEventCallbackInfo* eventInfo = static_cast<AkEventCallbackInfo*>(callbackInfo);
		signalData["eventID"] = static_cast<unsigned int>(eventInfo->eventID);
		signalData["gameObjID"] = static_cast<unsigned int>(eventInfo->gameObjID);
		signalData["playingID"] = static_cast<unsigned int>(eventInfo->playingID);
		break;
	}
	case AK_MusicPlaylistSelect:
	{
		AkMusicPlaylistCallbackInfo* musicPlaylistInfo = static_cast<AkMusicPlaylistCallbackInfo*>(callbackInfo);
		signalData["eventID"] = static_cast<unsigned int>(musicPlaylistInfo->eventID);
		signalData["gameObjID"] = static_cast<unsigned int>(musicPlaylistInfo->gameObjID);
		signalData["playingID"] = static_cast<unsigned int>(musicPlaylistInfo->playingID);
		signalData["playlistID"] = static_cast<unsigned int>(musicPlaylistInfo->playlistID);
		signalData["uNumPlaylistItems"] = static_cast<unsigned int>(musicPlaylistInfo->uNumPlaylistItems);
		signalData["uPlaylistItemDone"] = static_cast<unsigned int>(musicPlaylistInfo->uPlaylistItemDone);
		signalData["uPlaylistSelection"] = static_cast<unsigned int>(musicPlaylistInfo->uPlaylistSelection);
		break;
	}
	case AK_MusicPlayStarted:
	{
		AkEventCallbackInfo* eventInfo = static_cast<AkEventCallbackInfo*>(callbackInfo);
		signalData["eventID"] = static_cast<unsigned int>(eventInfo->eventID);
		signalData["gameObjID"] = static_cast<unsigned int>(eventInfo->gameObjID);
		signalData["playingID"] = static_cast<unsigned int>(eventInfo->playingID);
		break;
	}
	case AK_MusicSyncBeat:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		signalData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		signalData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		signalData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		signalData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

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
		signalData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncBar:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		signalData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		signalData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		signalData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		signalData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

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
		signalData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncEntry:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		signalData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		signalData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		signalData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		signalData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

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
		signalData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncExit:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		signalData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		signalData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		signalData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		signalData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

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
		signalData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncGrid:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		signalData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		signalData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		signalData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		signalData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

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
		signalData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncUserCue:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		signalData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		signalData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		signalData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		signalData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

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
		signalData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncPoint:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		signalData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		signalData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		signalData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		signalData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

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
		signalData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MusicSyncAll:
	{
		AkMusicSyncCallbackInfo* musicSyncInfo = static_cast<AkMusicSyncCallbackInfo*>(callbackInfo);
		signalData["gameObjID"] = static_cast<unsigned int>(musicSyncInfo->gameObjID);
		signalData["musicSyncType"] = static_cast<unsigned int>(musicSyncInfo->musicSyncType);
		signalData["playingID"] = static_cast<unsigned int>(musicSyncInfo->playingID);
		signalData["pszUserCueName"] = String(musicSyncInfo->pszUserCueName);

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
		signalData["segmentInfo"] = segmentInfo;
		break;
	}
	case AK_MIDIEvent:
	{
		AkMIDIEventCallbackInfo* midiEventInfo = static_cast<AkMIDIEventCallbackInfo*>(callbackInfo);
		signalData["eventID"] = static_cast<unsigned int>(midiEventInfo->eventID);
		signalData["gameObjID"] = static_cast<unsigned int>(midiEventInfo->gameObjID);


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
		pitchBend["byProgramNum"] = static_cast<unsigned char>(midiEventInfo->midiEvent.ProgramChange.byProgramNum);

		midiEvent["cc"] = cc;
		midiEvent["chanAftertouch"] = chanAftertouch;
		midiEvent["gen"] = gen;
		midiEvent["noteAftertouch"] = noteAftertouch;
		midiEvent["noteOnOff"] = noteOnOff;
		midiEvent["pitchBend"] = pitchBend;
		midiEvent["programChange"] = programChange;

		signalData["midiEvent"] = midiEvent;

		signalData["playingID"] = static_cast<unsigned int>(midiEventInfo->playingID);
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

	signalDataArray->append(signalData);

	signalDataMutex->unlock();
}

void Wwise::emitSignals()
{
	signalDataMutex->lock();

	const int initialArraySize = signalDataArray->size();

	for (int signalIndex = 0; signalIndex < initialArraySize; ++signalIndex)
	{
		const Dictionary data = signalDataArray->pop_front();
		const unsigned int callbackType = static_cast<unsigned int>(data["callbackType"]);

		emit_signal(WwiseCallbackToSignal(static_cast<AkCallbackType>(callbackType)), data);
	}

	AKASSERT(signalDataArray->size() == 0);

	signalDataMutex->unlock();
}

Variant Wwise::getPlatformProjectSetting(const String setting)
{
	AKASSERT(projectSettings);
	AKASSERT(!setting.empty());

	String platformSetting = setting;

#ifdef AK_WIN
	platformSetting += GODOT_WINDOWS_SETTING_POSTFIX;
#elif defined(AK_MAC_OS_X)
	platformSetting += GODOT_MAC_OSX_SETTING_POSTFIX;
#elif defined(AK_IOS)
	platformSetting += GODOT_IOS_SETTING_POSTFIX;
#elif defined(AK_ANDROID)
	platformSetting += GODOT_ANDROID_SETTING_POSTFIX;
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
	if (!ERROR_CHECK(lowLevelIO.Init(deviceSettings), "Low level IO failed"))
	{
		return false;
	}

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
	if (!ERROR_CHECK(AK::SoundEngine::Init(&initSettings, &platformInitSettings), "Sound engine initialisation failed"))
	{
		return false;
	}

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);
	if (!ERROR_CHECK(AK::MusicEngine::Init(&musicInit), "Music engine initialisation failed"))
	{
		return false;
	}

#ifndef AK_OPTIMIZED
	AkCommSettings settingsComm;
	AK::Comm::GetDefaultInitSettings(settingsComm);
	if (!ERROR_CHECK(AK::Comm::Init(settingsComm), "Comm initialisation failed"))
	{
		return false;
	}
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

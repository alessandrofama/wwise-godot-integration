#include "wwise_gdnative.h"

#include <AK/Plugin/AkSineSourceFactory.h>
#include <AK/Plugin/AkToneSourceFactory.h>
#include <AK/Plugin/AkSilenceSourceFactory.h>
#include <AK/Plugin/AkAudioInputPlugin.h>
#include <AK/Plugin/AkDelayFXFactory.h>
#include <AK/Plugin/AkParametricEQFXFactory.h>
#include <AK/Plugin/AkMatrixReverbFXFactory.h>
#include <AK/Plugin/AkCompressorFXFactory.h>
#include <AK/Plugin/AkExpanderFXFactory.h>
#include <AK/Plugin/AkPeakLimiterFXFactory.h>
#include <AK/Plugin/AkRoomVerbFXFactory.h>
#include <AK/Plugin/AkPitchShifterFXFactory.h>
#include <AK/Plugin/AkMeterFXFactory.h>
#include <AK/Plugin/AkVorbisDecoderFactory.h>
#include <AK/Plugin/AkFlangerFXFactory.h>
#include <AK/Plugin/AkGuitarDistortionFXFactory.h>
#include <AK/Plugin/AkTremoloFXFactory.h>
#include <AK/Plugin/AkTimeStretchFXFactory.h>
#include <AK/Plugin/AkStereoDelayFXFactory.h>
#include <AK/Plugin/AkHarmonizerFXFactory.h>
#include <AK/Plugin/AkGainFXFactory.h>

#include "wwise_utils.h"

using namespace godot;

namespace AK
{
	void* __cdecl AllocHook(size_t in_size)
	{
		Godot::print("AK::AllocHook called with size: " + String::num_int64(in_size));
		return api->godot_alloc(static_cast<int>(in_size));
	}

	void __cdecl FreeHook(void* in_ptr)
	{
		Godot::print("AK::FreeHook called");
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

#ifndef AK_OPTIMIZED
AkMemPoolId g_poolComm = AK_INVALID_POOL_ID;
#define COMM_POOL_SIZE (256 * 1024)
#define COMM_POOL_BLOCK_SIZE (48)
#endif

Wwise::~Wwise() 
{
	shutdownWwiseSystems();
	Godot::print("Wwise Shutdown....");
}

void Wwise::_register_methods()
{
	register_method("_process", &Wwise::_process);
	register_method("set_base_path", &Wwise::setBasePath);
	register_method("load_bank", &Wwise::loadBank);
	register_method("load_bank_id", &Wwise::loadBankID);
	register_method("unload_bank", &Wwise::unloadBank);
	register_method("unload_bank_id", &Wwise::unloadBankID);
	register_method("register_listener", &Wwise::registerListener);
	register_method("register_game_obj", &Wwise::registerGameObject);
	register_method("set_3d_position", &Wwise::set3DPosition);
	register_method("set_2d_position", &Wwise::set2DPosition);
	register_method("post_event", &Wwise::postEvent);
	register_method("post_event_id", &Wwise::postEventID);
	register_method("stop_event", &Wwise::stopEvent);
	register_method("set_switch", &Wwise::setSwitch);
	register_method("set_switch_id", &Wwise::setSwitchID);
	register_method("set_state", &Wwise::setState);
	register_method("set_state_id", &Wwise::setStateID);
	register_method("get_rtpc", &Wwise::getRTPCValue);
	register_method("get_rtpc_id", &Wwise::getRTPCValueID);
	register_method("set_rtpc", &Wwise::setRTPCValue);
	register_method("set_rtpc_id", &Wwise::setRTPCValueID);
}

void Wwise::_init()
{
	bool initialisationResult = initialiseWwiseSystems();

	if (!initialisationResult)
	{
		Godot::print("Wwise system initialisation failed!");
	}
	else
	{
		Godot::print("Wwise system initialisation succeeded");
	}
}

void Wwise::_process(const float delta)
{
	ERROR_CHECK(AK::SoundEngine::RenderAudio());
}

bool Wwise::initialiseWwiseSystems()
{
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (!ERROR_CHECK(AK::MemoryMgr::Init(&memSettings)))
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
	if (!ERROR_CHECK(lowLevelIO.Init(deviceSettings)))
	{
		return false;
	}

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
	if (!ERROR_CHECK(AK::SoundEngine::Init(&initSettings, &platformInitSettings)))
	{
		return false;
	}

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);
	if (!ERROR_CHECK(AK::MusicEngine::Init(&musicInit)))
	{
		return false;
	}

#ifndef AK_OPTIMIZED
	AkCommSettings settingsComm;
	AK::Comm::GetDefaultInitSettings(settingsComm);
	if (!ERROR_CHECK(AK::Comm::Init(settingsComm)))
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

	if (!ERROR_CHECK(AK::SoundEngine::UnregisterAllGameObj()))
	{
		return false;
	}

	if (!ERROR_CHECK(AK::SoundEngine::ClearBanks()))
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

bool Wwise::setBasePath(const String basePath)
{
	AkOSChar* basePathOsString = nullptr;

	const wchar_t* basePathChar = basePath.unicode_str();
	CONVERT_WIDE_TO_OSCHAR(basePathChar, basePathOsString);

	return ERROR_CHECK(lowLevelIO.SetBasePath(basePathOsString));
}

bool Wwise::loadBank(const String bankName)
{
	AkBankID bankID;
	return ERROR_CHECK(AK::SoundEngine::LoadBank(bankName.unicode_str(), bankID));
}

bool Wwise::loadBankID(const unsigned int bankID)
{
	return ERROR_CHECK(AK::SoundEngine::LoadBank(bankID));
}

bool Wwise::unloadBank(const String bankName)
{
	return ERROR_CHECK(AK::SoundEngine::UnloadBank(bankName.unicode_str(), NULL));
}

bool Wwise::unloadBankID(const unsigned int bankID)
{
	return ERROR_CHECK(AK::SoundEngine::UnloadBank(bankID, NULL));
}

// todo: multiple listeners
bool Wwise::registerListener(const Object* gameObject)
{
	AkGameObjectID LISTENER = static_cast<AkGameObjectID>(gameObject->get_instance_id());

	if (!ERROR_CHECK(AK::SoundEngine::RegisterGameObj(LISTENER, "My Default Listener")))
	{
		return false;
	}

	if (!ERROR_CHECK(AK::SoundEngine::SetDefaultListeners(&LISTENER, 1)))
	{
		return false;
	}

	return true;
}

bool Wwise::registerGameObject(const Object* gameObject, const String gameObjectName)
{
	return ERROR_CHECK(AK::SoundEngine::RegisterGameObj(static_cast<AkGameObjectID>(gameObject->get_instance_id()), 
						gameObjectName.alloc_c_string()));
}

bool Wwise::set3DPosition(const Object* gameObject, const Transform transform)
{
	AkSoundPosition soundPos;

	AkVector position;
	GetAkVector(transform, position, VectorType::POSITION);
	AkVector forward;
	GetAkVector(transform, forward, VectorType::FORWARD);
	AkVector up;
	GetAkVector(transform, up, VectorType::UP);

	soundPos.Set(position, forward, up);

	return ERROR_CHECK(AK::SoundEngine::SetPosition(static_cast<AkGameObjectID>(gameObject->get_instance_id()), soundPos));
}

bool Wwise::set2DPosition(const Object* gameObject, const Transform2D transform2D, const float zDepth)
{
	AkSoundPosition soundPos;

	Vector2 origin = transform2D.get_origin();

	Vector3 position = Vector3(origin.x * 0.1f, -origin.y * 0.1f, zDepth);
	Vector3 forward = Vector3(transform2D.elements[1].x, 0, transform2D.elements[1].y).normalized();
	Vector3 up = Vector3(0, 1, 0);

	AkVector akPosition;
	Vector3ToAkVector(position, akPosition);
	AkVector akForward;
	Vector3ToAkVector(forward, akForward);
	AkVector akUp;
	Vector3ToAkVector(up, akUp);

	soundPos.Set(akPosition, akForward, akUp);

	return ERROR_CHECK(AK::SoundEngine::SetPosition(static_cast<AkGameObjectID>(gameObject->get_instance_id()), soundPos));
}

unsigned int Wwise::postEvent(const String eventName, const Object* gameObject)
{
	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventName.unicode_str(), static_cast<AkGameObjectID>(gameObject->get_instance_id()));

	if (playingID == AK_INVALID_PLAYING_ID) 
	{
		return static_cast<unsigned int>(AK_INVALID_PLAYING_ID);
	}

	return static_cast<unsigned int>(playingID);
}

unsigned int Wwise::postEventID(const unsigned int eventID, const Object* gameObject)
{
	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventID, static_cast<AkGameObjectID>(gameObject->get_instance_id()));

	if (playingID == AK_INVALID_PLAYING_ID) 
	{
		return static_cast<unsigned int>(AK_INVALID_PLAYING_ID);
	}

	return static_cast<unsigned int>(playingID);
}

// any way to check if AK::SoundEngine::StopPlayingID succeded?
bool Wwise::stopEvent(const int playingID, const int fadeTime, const int interpolation)
{
	AK::SoundEngine::StopPlayingID(static_cast<AkPlayingID>(playingID), static_cast<AkTimeMs>(fadeTime), 
									static_cast<AkCurveInterpolation>(interpolation));

	return true;
}

bool Wwise::setSwitch(const String switchGroup, const String switchState, const Object* gameObject)
{
	return ERROR_CHECK(AK::SoundEngine::SetSwitch(switchGroup.unicode_str(), 
						switchState.unicode_str(), 
						static_cast<AkGameObjectID>(gameObject->get_instance_id())));
}

bool Wwise::setSwitchID(const unsigned int switchGroupID, const unsigned int switchStateID, const Object* gameObject)
{
	return ERROR_CHECK(AK::SoundEngine::SetSwitch(switchGroupID, switchStateID, static_cast<AkGameObjectID>(gameObject->get_instance_id())));
}

bool Wwise::setState(const String stateGroup, const String stateValue)
{
	return ERROR_CHECK(AK::SoundEngine::SetState(stateGroup.unicode_str(), stateValue.unicode_str()));
}

bool Wwise::setStateID(const unsigned int stateGroupID, const unsigned int stateValueID)
{
	return ERROR_CHECK(AK::SoundEngine::SetState(stateGroupID, stateValueID));
}

// todo: global rtpc
float Wwise::getRTPCValue(const String rtpcName, const Object* gameObject)
{
	AkRtpcValue value;
	AK::SoundEngine::Query::RTPCValue_type type = AK::SoundEngine::Query::RTPCValue_GameObject;

	if (!ERROR_CHECK(AK::SoundEngine::Query::GetRTPCValue(rtpcName.unicode_str(), 
		static_cast<AkGameObjectID>(gameObject->get_instance_id()),
		static_cast<AkPlayingID>(0), value, type)))
	{
		return -1.0f;
	}

	return static_cast<float>(value);
}

float Wwise::getRTPCValueID(const unsigned int rtpcID, const Object* gameObject)
{
	AkRtpcValue value;
	AK::SoundEngine::Query::RTPCValue_type type = AK::SoundEngine::Query::RTPCValue_GameObject;

	if (!ERROR_CHECK(AK::SoundEngine::Query::GetRTPCValue(rtpcID, static_cast<AkGameObjectID>(gameObject->get_instance_id()),
		static_cast<AkPlayingID>(0), value, type)))
	{
		return -1.0f;
	}

	return static_cast<float>(value);
}

// todo: global rtpc
bool Wwise::setRTPCValue(const String rtpcName, const float rtpcValue, const Object* gameObject)
{
	return ERROR_CHECK(AK::SoundEngine::SetRTPCValue(rtpcName.unicode_str(), static_cast<AkRtpcValue>(rtpcValue), 
						static_cast<AkGameObjectID>(gameObject->get_instance_id())));
}

bool Wwise::setRTPCValueID(const unsigned int rtpcID, const float rtpcValue, const Object* gameObject)
{
	return ERROR_CHECK(AK::SoundEngine::SetRTPCValue(rtpcID, static_cast<AkRtpcValue>(rtpcValue), 
						static_cast<AkGameObjectID>(gameObject->get_instance_id())));
}

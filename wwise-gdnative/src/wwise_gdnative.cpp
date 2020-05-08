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
}

#ifndef AK_OPTIMIZED
	AkMemPoolId g_poolComm = AK_INVALID_POOL_ID;
	#define COMM_POOL_SIZE (256 * 1024)
	#define COMM_POOL_BLOCK_SIZE (48)
#endif

#endif

bool Wwise::checkError(AKRESULT result, const char* function, const char* file, int line) 
{
	if (result != AK_Success)
	{
		Godot::print_error(String(Wwise_ErrorString(result)), function, file, line);
		return false;
	}
	return true;
}

Wwise::Wwise() = default;

void Wwise::_register_methods()
{
	register_method("_process", &Wwise::_process);
	register_method("set_base_path", &Wwise::setBasePath);
	register_method("load_bank", &Wwise::loadBank);
	register_method("unload_bank", &Wwise::unloadBank);
	register_method("register_listener", &Wwise::registerListener);
	register_method("set_listener_position", &Wwise::setListenerPosition);
	register_method("register_game_obj", &Wwise::registerGameObj);
	register_method("set_3d_position", &Wwise::set3DPosition);
	register_method("post_event", &Wwise::postEvent);
	register_method("stop_event", &Wwise::stopEvent);
	register_method("set_switch", &Wwise::setSwitch);
	register_method("set_state", &Wwise::setState);
	register_method("get_rtpc", &Wwise::getRTPCValue);
	register_method("set_rtpc", &Wwise::setRTPCValue);
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

void Wwise::_process(float delta)
{
	ERROR_CHECK(AK::SoundEngine::RenderAudio());
}

Wwise::~Wwise() {
	shutdownWwiseSystems();
	Godot::print("Wwise Shutdown....");
}

bool Wwise::initialiseWwiseSystems()
{
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (!ERROR_CHECK(AK::MemoryMgr::Init(&memSettings)))
		return false;

	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);
	if (!AK::StreamMgr::Create(stmSettings))
		return false;

	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
	if (!ERROR_CHECK(g_lowLevelIO.Init(deviceSettings)))
		return false;

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
	if (!ERROR_CHECK(AK::SoundEngine::Init(&initSettings, &platformInitSettings)))
		return false;

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
		return false;
#endif

	return true;
}

bool Wwise::shutdownWwiseSystems()
{
#ifndef AK_OPTIMIZED
	AK::Comm::Term();
#endif

	if (!ERROR_CHECK(AK::SoundEngine::UnregisterAllGameObj()))
		return false;

	if (!ERROR_CHECK(AK::SoundEngine::ClearBanks()))
		return false;

	AK::MusicEngine::Term();

	AK::SoundEngine::Term();

	g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}

	AK::MemoryMgr::Term();

	return true;
}

bool Wwise::setBasePath(String basePath)
{
	AkOSChar* basePathOsString = NULL;

	const wchar_t* basePathChar = basePath.unicode_str();
	CONVERT_WIDE_TO_OSCHAR(basePathChar, basePathOsString);

	return ERROR_CHECK(g_lowLevelIO.SetBasePath(basePathOsString));
}

bool Wwise::loadBank(String bankName)
{
	AkBankID bankID;
	const wchar_t* bank = bankName.unicode_str();
	return ERROR_CHECK(AK::SoundEngine::LoadBank(bank, bankID));
}

bool Wwise::unloadBank(String bankName)
{
	const wchar_t* bank = bankName.unicode_str();
	return ERROR_CHECK(AK::SoundEngine::UnloadBank(bank, NULL));
}

// todo: multiple listeners
bool Wwise::registerListener()
{
	AkGameObjectID LISTENER = 0;

	AK::SoundEngine::RegisterGameObj(LISTENER, "My Default Listener");
	AK::SoundEngine::SetDefaultListeners(&LISTENER, 1);

	return true;
}

bool Wwise::setListenerPosition(Object* gameObject)
{
	AkSoundPosition listenerPosition;
	Spatial* s = Object::cast_to<Spatial>(gameObject);
	Transform t = s->get_global_transform();

	AkVector position = GetAkVector(t, VectorType::POSITION);
	AkVector forward = GetAkVector(t, VectorType::FORWARD);
	AkVector up = GetAkVector(t, VectorType::UP);

	listenerPosition.Set(position, forward, up);

	return ERROR_CHECK(AK::SoundEngine::SetPosition(0, listenerPosition));
}

// todo: figure out how to convert String to const char*
bool Wwise::registerGameObj(Object* gameObject, String gameObjectName)
{
	const char* goName = gameObjectName.alloc_c_string();
	return ERROR_CHECK(AK::SoundEngine::RegisterGameObj(static_cast<AkGameObjectID>(gameObject->get_instance_id()), goName));
}

// todo: 2d stuff
bool Wwise::set3DPosition(Object* gameObject)
{
	AkSoundPosition soundPos;
	Spatial* s = Object::cast_to<Spatial>(gameObject);
	Transform t = s->get_transform();

	AkVector position = GetAkVector(t, VectorType::POSITION);
	AkVector forward = GetAkVector(t, VectorType::FORWARD);
	AkVector up = GetAkVector(t, VectorType::UP);

	soundPos.Set(position, forward, up);

	return ERROR_CHECK(AK::SoundEngine::SetPosition(static_cast<AkGameObjectID>(gameObject->get_instance_id()), soundPos));
}

int Wwise::postEvent(String eventName, Object* gameObject)
{
	const wchar_t* event = eventName.unicode_str();
	auto playingID = AK::SoundEngine::PostEvent(event, static_cast<AkGameObjectID>(gameObject->get_instance_id()));
	if (playingID == AK_INVALID_PLAYING_ID) {
		return static_cast<int>(AK_INVALID_PLAYING_ID);
	}
	return static_cast<int>(playingID);
}

// any way to check if AK::SoundEngine::StopPlayingID succeded?
bool Wwise::stopEvent(int playingID, int fadeTime, int interpolation)
{
	AkTimeMs ms = static_cast<AkTimeMs>(fadeTime);
	int curve = static_cast<int>(interpolation);

	AK::SoundEngine::StopPlayingID(static_cast<AkPlayingID>(playingID), ms, (AkCurveInterpolation)curve);

	return true;
}

bool Wwise::setSwitch(String switchGroup, String switchState, Object* gameObject)
{
	const wchar_t* group = switchGroup.unicode_str();
	const wchar_t* state = switchState.unicode_str();

	return ERROR_CHECK(AK::SoundEngine::SetSwitch(group, state, static_cast<AkGameObjectID>(gameObject->get_instance_id())));
}

bool Wwise::setState(String stateGroup, String stateValue)
{
	const wchar_t* group = stateGroup.unicode_str();
	const wchar_t* value = stateValue.unicode_str();

	return ERROR_CHECK(AK::SoundEngine::SetState(group, value));
}

// todo: global rtpc
float Wwise::getRTPCValue(String rtpcName, Object* gameObject)
{
	AkRtpcValue value;

	AK::SoundEngine::Query::RTPCValue_type type = AK::SoundEngine::Query::RTPCValue_GameObject;
	const wchar_t* rtpc = rtpcName.unicode_str();

	if (!ERROR_CHECK(AK::SoundEngine::Query::GetRTPCValue(rtpc, static_cast<AkGameObjectID>(gameObject->get_instance_id()), static_cast<AkPlayingID>(0), value, type)))
		return -1.f;
	else return static_cast<float>(value);
}

// todo: global rtpc
bool Wwise::setRTPCValue(String rtpcName, float rtpcValue, Object* gameObject)
{
	AkRtpcValue value = static_cast<AkRtpcValue>(rtpcValue);
	const wchar_t* rtpc = rtpcName.unicode_str();

	return ERROR_CHECK(AK::SoundEngine::SetRTPCValue(rtpc, value, static_cast<AkGameObjectID>(gameObject->get_instance_id())));
}
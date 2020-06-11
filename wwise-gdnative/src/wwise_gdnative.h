#ifndef WWISE_GDNATIVE_H
#define WWISE_GDNATIVE_H

#include <Godot.hpp>
#include <GodotGlobal.hpp>
#include <Node.hpp>
#include <Object.hpp>
#include <Spatial.hpp>
#include <Mutex.hpp>
#include <ProjectSettings.hpp>

#include <AK/SoundEngine/Common/AkSoundEngine.h> 
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>
#include <AK/SpatialAudio/Common/AkSpatialAudio.h> 
#include <AK/Soundengine/Common/AkVirtualAcoustics.h>
#include <AkDefaultIOHookBlocking.h>

#include "wwise_utils.h"

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif

namespace godot
{
	class Wwise : public Node
	{
		GODOT_CLASS(Wwise, Node)

	public:
		explicit Wwise() = default;
		~Wwise();

		static void _register_methods();
		void _init();
		void _process(const float delta);

		bool setBasePath(const String basePath);
		bool setCurrentLanguage(const String language);
		bool loadBank(const String bankName);
		bool loadBankID(const unsigned int bankID);
		bool unloadBank(const String bankName);
		bool unloadBankID(const unsigned int bankID);

		bool registerListener(const Object* gameObject);
		bool registerGameObject(const Object* gameObject, const String gameObjectName);

		bool SetListeners(const Object* emitter, const Object* listener);

		bool set3DPosition(const Object* gameObject, const Transform transform);
		bool set2DPosition(const Object* gameObject, const Transform2D transform2D, const float zDepth);

		unsigned int postEvent(const String eventName, const Object* gameObject);
		unsigned int postEventCallback(const String eventName, const unsigned int flags, const Object* gameObject);
		unsigned int postEventID(const unsigned int eventID, const Object* gameObject);
		unsigned int postEventIDCallback(const unsigned int eventID, const unsigned int flags, const Object* gameObject);
		bool stopEvent(const int playingID, const int fadeTime, const int interpolation);

		bool setSwitch(const String switchGroup, const String switchState, const Object* gameObject);
		bool setSwitchID(const unsigned int switchGroupID, const unsigned int switchStateID, const Object* gameObject);
		bool setState(const String stateGroup, const String stateValue);
		bool setStateID(const unsigned int stateGroupID, const unsigned int stateValueID);

		float getRTPCValue(const String rtpcName, const Object* gameObject);
		float getRTPCValueID(const unsigned int rtpcID, const Object* gameObject);
		bool setRTPCValue(const String rtpcName, const float rtpcValue, const Object* gameObject);
		bool setRTPCValueID(const unsigned int rtpcID, const float rtpcValue, const Object* gameObject);

		bool postTrigger(const String triggerName, const Object* gameObject);
		bool postTriggerID(const unsigned int triggerID, const Object* gameObject);

		unsigned int postExternalSource(const String eventName, const Object* gameObject, const String sourceObjectName, 
										const String fileName, const unsigned int idCodec);
		unsigned int postExternalSourceID(const unsigned int eventID, const Object* gameObject, const unsigned int sourceObjectID, 
										const String fileName, const unsigned int idCodec);

		int getSourcePlayPosition(const unsigned int playingID, const bool extrapolate);
		Dictionary getPlayingSegmentInfo(const unsigned int playingID, const bool extrapolate);

		bool setGameObjectOutputBusVolume(const unsigned int eventID, const unsigned int listenerID, float fControlValue);
		bool setGameObjectAuxSendValues(const unsigned int eventID, const Array akAuxSendValues, const unsigned int sendValues);
		bool setObjectObstructionAndOcclusion(const unsigned int eventID, const unsigned int listenerID, float fCalculatedObs, float fCalculatedOcc);

		bool setGeometry(const Array vertices, const Array triangles, const String acousticTexture, const float occlusionValue, 
						 const Object* gameObject, bool enableDiffraction, bool enableDiffractionOnBoundaryEdges, const Object* associatedRoom);
		bool removeGeometry(const Object* gameObject);
		bool registerSpatialListener(const Object* gameObject);
		bool setRoom(const Object* gameObject, const unsigned int akAuxBusID, const String gameObjectName);
		bool removeRoom(const Object* gameObject);
		bool setPortal(const Object* gameObject, const Transform transform, const Vector3 extent, const Object* frontRoom, const Object* backRoom, bool enabled, const String portalName);
		bool removePortal(const Object* gameObject);
		bool setGameObjectInRoom(const Object* gameObject, const Object* room);
		bool removeGameObjectFromRoom(const Object* gameObject);
		bool setEarlyReflectionsAuxSend(const Object* gameObject, const unsigned int auxBusID);

		bool addOutput(const String shareSet, const unsigned int outputID);
		bool removeOutput(const unsigned int outputID);

	private:
		const String GODOT_WINDOWS_SETTING_POSTFIX = ".Windows";
		const String GODOT_MAC_OSX_SETTING_POSTFIX = ".OSX";
		const String GODOT_IOS_SETTING_POSTFIX = ".iOS";
		const String GODOT_ANDROID_SETTING_POSTFIX = ".Android";

		const String WWISE_COMMON_USER_SETTINGS_PATH = "wwise/common_user_settings/";
		const String WWISE_COMMON_ADVANCED_SETTINGS_PATH = "wwise/common_advanced_settings/";
		const String WWISE_SPATIAL_AUDIO_PATH = "spatial_audio/";

		static void eventCallback(AkCallbackType callbackType, AkCallbackInfo* callbackInfo);
		void emitSignals();

		Variant getPlatformProjectSetting(const String setting);

		bool initialiseWwiseSystems();
		bool shutdownWwiseSystems();

		static Mutex* signalDataMutex;
		static Array* signalDataArray;

		ProjectSettings* projectSettings;
		CAkDefaultIOHookBlocking lowLevelIO;
	};
}

#endif
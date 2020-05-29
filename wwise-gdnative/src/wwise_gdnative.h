#ifndef WWISE_GDNATIVE_H
#define WWISE_GDNATIVE_H

#include <Godot.hpp>
#include <GodotGlobal.hpp>
#include <Node.hpp>
#include <Object.hpp>
#include <Spatial.hpp>
#include <Mutex.hpp>

#include <AK/SoundEngine/Common/AkSoundEngine.h> 
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>
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
		bool loadBank(const String bankName);
		bool loadBankID(const unsigned int bankID);
		bool unloadBank(const String bankName);
		bool unloadBankID(const unsigned int bankID);

		bool registerListener(const Object* gameObject);
		bool registerGameObject(const Object* gameObject, const String gameObjectName);

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

	private:
		static void eventCallback(AkCallbackType callbackType, AkCallbackInfo* callbackInfo);

		void emitSignals();
		bool initialiseWwiseSystems();
		bool shutdownWwiseSystems();

		static Mutex* signalDataMutex;
		static Array* signalDataArray;

		CAkDefaultIOHookBlocking lowLevelIO;
	};
}

#endif
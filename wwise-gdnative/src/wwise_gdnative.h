#ifndef WWISE_GDNATIVE_H
#define WWISE_GDNATIVE_H

#include <Godot.hpp>
#include <GodotGlobal.hpp>
#include <Node.hpp>
#include <Object.hpp>
#include <Spatial.hpp>

#include <AkDefaultIOHookBlocking.h>

#include <AK/SoundEngine/Common/AkSoundEngine.h> 
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>

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
		void _process(float delta);

		bool setBasePath(const String basePath);
		bool loadBank(const String bankName);
		bool loadBankID(const unsigned int bankID);
		bool unloadBank(const String bankName);
		bool unloadBankID(const unsigned int bankID);

		bool registerListener();
		bool setListenerPosition(const Object* gameObject);

		bool registerGameObj(const Object* gameObject, const String gameObjectName);

		bool set3DPosition(const Object* gameObject);

		int postEvent(const String eventName, const Object* gameObject);
		int postEventID(const unsigned int eventID, const Object* gameObject);
		bool stopEvent(const int playingID, const int fadeTime, const int interpolation);

		bool setSwitch(const String switchGroup, const String switchState, const Object* gameObject);
		bool setSwitchID(const unsigned int switchGroup, const unsigned int switchState, const Object* gameObject);
		bool setState(const String stateGroup, const String stateValue);
		bool setStateID(const unsigned int stateGroup, const unsigned int stateValue);

		float getRTPCValue(const String rtpcName, const Object* gameObject);
		float getRTPCValueID(const unsigned int rtpc, const Object* gameObject);
		bool setRTPCValue(const String rtpcName, const float rtpcValue, const Object* gameObject);
		bool setRTPCValueID(const unsigned int rtpc, float rtpcValue, Object* gameObject);

	private:
		bool initialiseWwiseSystems();
		bool shutdownWwiseSystems();
		CAkDefaultIOHookBlocking lowLevelIO;
	};
}

#endif
#ifndef WWISE_GDNATIVE_H
#define WWISE_GDNATIVE_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkVirtualAcoustics.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>
#include <AK/Tools/Common/AkAutoLock.h>
#include <AK/Tools/Common/AkLock.h>
#include <AK/Tools/Common/AkMonitorError.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>

#include "wwise_godot_io.h"
#include "wwise_utils.h"

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif

#include <memory>


namespace godot
{

class Wwise : public Object
{
	GDCLASS(Wwise, Object);

	protected:
	static Wwise* singleton;
	static void _bind_methods();
	//void _notification(int notification);

  public:
	Wwise();
	~Wwise();
	static Wwise* get_singleton();

	void init();
	void process(double delta);
	void shutdown();

	bool setBasePath(const String basePath);
	void setCurrentLanguage(const String language);
	bool loadBank(const String bankName);
	bool loadBankID(const unsigned int bankID);
	bool loadBankAsync(const String bankName, const Object* cookie);
	bool loadBankAsyncID(const unsigned int bankID, const Object* cookie);
	bool unloadBank(const String bankName);
	bool unloadBankID(const unsigned int bankID);
	bool unloadBankAsync(const String bankName, const Object* cookie);
	bool unloadBankAsyncID(const unsigned int bankID, const Object* cookie);

	bool registerListener(const Object* gameObject);
	bool registerGameObject(const Object* gameObject, const String gameObjectName);
	bool unregisterGameObject(const Object* gameObject);

	bool setListeners(const Object* emitter, const Object* listener);
	void setRandomSeed(const unsigned int seed);

	bool set3DPosition(const Object* gameObject, const Transform3D transform);
	bool set2DPosition(const Object* gameObject, const Transform2D transform2D, const float zDepth);
	bool setMultiplePositions3D(const Object* gameObject, const Array positions, const int numPositions,
								const int multiPositionType);
	bool setMultiplePositions2D(const Object* gameObject, const Array positions, const Array zDepths,
								const int numPositions, const int multiPositionType);
	bool setGameObjectRadius(const Object* gameObject, const float outerRadius, const float innerRadius);

	unsigned int postEvent(const String eventName, const Object* gameObject);
	unsigned int postEventCallback(const String eventName, const unsigned int flags, const Object* gameObject,
								   const Object* cookie);
	unsigned int postEventID(const unsigned int eventID, const Object* gameObject);
	unsigned int postEventIDCallback(const unsigned int eventID, const unsigned int flags, const Object* gameObject,
									 const Object* cookie);
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
	unsigned int postExternalSourceID(const unsigned int eventID, const Object* gameObject,
									  const unsigned int sourceObjectID, const String fileName,
									  const unsigned int idCodec);

	int getSourcePlayPosition(const unsigned int playingID, const bool extrapolate);
	Dictionary getPlayingSegmentInfo(const unsigned int playingID, const bool extrapolate);

	bool setGameObjectOutputBusVolume(const unsigned int gameObjectID, const unsigned int listenerID,
									  float fControlValue);
	bool setGameObjectAuxSendValues(const unsigned int gameObjectID, const Array akAuxSendValues,
									const unsigned int sendValues);
	bool setObjectObstructionAndOcclusion(const unsigned int gameObjectID, const unsigned int listenerID,
										  float fCalculatedObs, float fCalculatedOcc);

	bool setGeometry(const Array vertices, const Array triangles, const Resource* acousticTexture,
					 const float tranmissionLossValue, const Object* gameObject, bool enableDiffraction,
					 bool enableDiffractionOnBoundaryEdges, bool enableTriangles);
	bool removeGeometry(const Object* gameObject);
	bool setGeometryInstance(const Object* gameObject, const Transform3D transform, const Object* associatedGeometry,
							 const Object* associatedRoom);
	bool removeGeometryInstance(const Object* gameObject);
	bool registerSpatialListener(const Object* gameObject);
	bool setRoom(const Object* gameObject, const unsigned int akAuxBusID, const float reverbLevel,
				 const float transmissionLoss, const Vector3 frontVector, const Vector3 upVector, bool keepRegistered,
				 const int associatedGeometry);
	bool removeRoom(const Object* gameObject);
	bool setPortal(const Object* gameObject, const Transform3D transform, const Vector3 extent,
 const Object* frontRoom,
				   const Object* backRoom, bool enabled);
	bool removePortal(const Object* gameObject);
	bool setPortalObstructionAndOcclusion(const Object* portal, const float obstructionValue,
										  const float occlusionValue);
	bool setGameObjectToPortalObstruction(const Object* gameObject, const Object* portal, const float obstructionValue);
	bool setPortalToPortalObstruction(const Object* portal0, const Object* portal1, const float obstructionValue);
	bool setGameObjectInRoom(const Object* gameObject, const Object* room);
	bool removeGameObjectFromRoom(const Object* gameObject);
	bool setEarlyReflectionsAuxSend(const Object* gameObject, const unsigned int auxBusID);
	bool setEarlyReflectionsVolume(const Object* gameObject, const float volume);

	bool addOutput(const String shareSet, const unsigned int outputID);
	bool removeOutput(const unsigned int outputID);
	bool suspend(bool renderAnyway);
	bool wakeupFromSuspend();

  private:
	String GODOT_WINDOWS_SETTING_POSTFIX = ".Windows";
	String GODOT_MAC_OSX_SETTING_POSTFIX = ".OSX";
	String GODOT_IOS_SETTING_POSTFIX = ".iOS";
	String GODOT_ANDROID_SETTING_POSTFIX = ".Android";
	String GODOT_LINUX_SETTING_POSTFIX = ".Linux";

	String WWISE_COMMON_USER_SETTINGS_PATH = "wwise/common_user_settings/";
	String WWISE_COMMON_ADVANCED_SETTINGS_PATH = "wwise/common_advanced_settings/";
	String WWISE_SPATIAL_AUDIO_PATH = "spatial_audio/";
	String WWISE_COMMUNICATION_SETTINGS_PATH = "wwise/communication_settings/";

	static void eventCallback(AkCallbackType callbackType, AkCallbackInfo* callbackInfo);
	static void bankCallback(AkUInt32 bankID, const void* inMemoryBankPtr, AKRESULT loadResult, void* in_pCookie);

	Variant getPlatformProjectSetting(const String setting);

	bool initialiseWwiseSystems();
	bool shutdownWwiseSystems();

	static CAkLock callbackDataLock;

	ProjectSettings* projectSettings;
	CAkFileIOHandlerGodot lowLevelIO;

	bool suspendAtFocusLoss = false;
	bool renderAnyway = false;
};

}

#endif
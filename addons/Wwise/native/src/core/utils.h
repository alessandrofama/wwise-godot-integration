#pragma once

#include "AK/SoundEngine/Common/AkTypes.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

const float INVALID_RTPC_VALUE = 1.0f;
const unsigned int AK_MAX_ENVIRONMENTS = 4;
const int INVALID_ROOM_ID = -1;
const AkGameObjectID OUTDOORS_ROOM_ID = (AkGameObjectID)-4;

enum SamplesPerFrame
{
	SAMPLES_256 = 0,
	SAMPLES_512,
	SAMPLES_1024,
	SAMPLES_2048
};

enum NumRefillsInVoice
{
	REFILLS_2 = 0,
	REFILLS_4
};

enum SampleRate
{
	RATE_16000 = 0,
	RATE_24000,
	RATE_32000,
	RATE_44100,
	RATE_48000
};

static const char* wwise_error_string(AKRESULT errcode)
{
	switch (errcode)
	{
		case AK_NotImplemented:
			return "AK_NotImplemented";
		case AK_Success:
			return "AK_Success";
		case AK_Fail:
			return "AK_Fail";
		case AK_PartialSuccess:
			return "AK_PartialSuccess";
		case AK_NotCompatible:
			return "AK_NotCompatible";
		case AK_AlreadyConnected:
			return "AK_AlreadyConnected";
		case AK_InvalidFile:
			return "AK_InvalidFile";
		case AK_AudioFileHeaderTooLarge:
			return "AK_AudioFileHeaderTooLarge";
		case AK_MaxReached:
			return "AK_MaxReached";
		case AK_InvalidID:
			return "AK_InvalidID";
		case AK_IDNotFound:
			return "AK_IDNotFound";
		case AK_InvalidInstanceID:
			return "AK_InvalidInstanceID";
		case AK_NoMoreData:
			return "AK_NoMoreData";
		case AK_InvalidStateGroup:
			return "AK_InvalidStateGroup";
		case AK_ChildAlreadyHasAParent:
			return "AK_ChildAlreadyHasAParent";
		case AK_InvalidLanguage:
			return "AK_InvalidLanguage";
		case AK_CannotAddItselfAsAChild:
			return "AK_CannotAddItselfAsAChild";
		case AK_InvalidParameter:
			return "AK_InvalidParameter";
		case AK_ElementAlreadyInList:
			return "AK_ElementAlreadyInList";
		case AK_PathNotFound:
			return "AK_PathNotFound";
		case AK_PathNoVertices:
			return "AK_PathNoVertices";
		case AK_PathNotRunning:
			return "AK_PathNotRunning";
		case AK_PathNotPaused:
			return "AK_PathNotPaused";
		case AK_PathNodeAlreadyInList:
			return "AK_PathNodeAlreadyInList";
		case AK_PathNodeNotInList:
			return "AK_PathNodeNotInList";
		case AK_DataNeeded:
			return "AK_DataNeeded";
		case AK_NoDataNeeded:
			return "AK_NoDataNeeded";
		case AK_DataReady:
			return "AK_DataReady";
		case AK_NoDataReady:
			return "AK_NoDataReady";
		case AK_InsufficientMemory:
			return "AK_InsufficientMemory";
		case AK_Cancelled:
			return "AK_Cancelled";
		case AK_UnknownBankID:
			return "AK_UnknownBankID";
		case AK_BankReadError:
			return "AK_BankReadError";
		case AK_InvalidSwitchType:
			return "AK_InvalidSwitchType";
		case AK_FormatNotReady:
			return "AK_FormatNotReady";
		case AK_WrongBankVersion:
			return "AK_WrongBankVersion";
		case AK_FileNotFound:
			return "AK_FileNotFound";
		case AK_DeviceNotReady:
			return "AK_DeviceNotReady";
		case AK_BankAlreadyLoaded:
			return "AK_BankAlreadyLoaded";
		case AK_RenderedFX:
			return "AK_RenderedFX";
		case AK_ProcessNeeded:
			return "AK_ProcessNeeded";
		case AK_ProcessDone:
			return "AK_ProcessDone";
		case AK_MemManagerNotInitialized:
			return "AK_MemManagerNotInitialized";
		case AK_StreamMgrNotInitialized:
			return "AK_StreamMgrNotInitialized";
		case AK_SSEInstructionsNotSupported:
			return "AK_SSEInstructionsNotSupported";
		case AK_Busy:
			return "AK_Busy";
		case AK_UnsupportedChannelConfig:
			return "AK_UnsupportedChannelConfig";
		case AK_PluginMediaNotAvailable:
			return "AK_PluginMediaNotAvailable";
		case AK_MustBeVirtualized:
			return "AK_MustBeVirtualized";
		case AK_CommandTooLarge:
			return "AK_CommandTooLarge";
		case AK_RejectedByFilter:
			return "AK_RejectedByFilter";
		case AK_InvalidCustomPlatformName:
			return "AK_InvalidCustomPlatformName";
		case AK_DLLCannotLoad:
			return "AK_DLLCannotLoad";
		case AK_DLLPathNotFound:
			return "AK_DLLPathNotFound";
		case AK_NoJavaVM:
			return "AK_NoJavaVM";
		case AK_OpenSLError:
			return "AK_OpenSLError";
		case AK_PluginNotRegistered:
			return "AK_PluginNotRegistered";
		case AK_DataAlignmentError:
			return "AK_DataAlignmentError";
		case AK_DeviceNotCompatible:
			return "AK_DeviceNotCompatible";
		case AK_DuplicateUniqueID:
			return "AK_DuplicateUniqueID";
		case AK_InitBankNotLoaded:
			return "AK_InitBankNotLoaded";
		case AK_DeviceNotFound:
			return "AK_DeviceNotFound";
		case AK_PlayingIDNotFound:
			return "AK_PlayingIDNotFound";
		case AK_InvalidFloatValue:
			return "AK_InvalidFloatValue";
		case AK_FileFormatMismatch:
			return "AK_FileFormatMismatch";
		case AK_NoDistinctListener:
			return "AK_NoDistinctListener";
		case AK_ResourceInUse:
			return "AK_ResourceInUse";
		case AK_InvalidBankType:
			return "AK_InvalidBankType";
		case AK_AlreadyInitialized:
			return "AK_AlreadyInitialized";
		case AK_NotInitialized:
			return "AK_NotInitialized";
		case AK_FilePermissionError:
			return "AK_FilePermissionError";
		case AK_UnknownFileError:
			return "AK_UnknownFileError";
		case AK_TooManyConcurrentOperations:
			return "AK_TooManyConcurrentOperations";
		case AK_InvalidFileSize:
			return "AK_InvalidFileSize";
		case AK_Deferred:
			return "AK_Deferred";
		case AK_FilePathTooLong:
			return "AK_FilePathTooLong";
		case AK_InvalidState:
			return "AK_InvalidState";
		default:
			return "Unknown Error";
	}
}

static bool check_error(
		const AKRESULT result, const char* function, const char* file, const int line, const String& message = "")
{
	if (result != AK_Success)
	{
		String error_message =
				vformat("[%s]: %s in %s:%d. %s", wwise_error_string(result), function, file, line, message);
		UtilityFunctions::push_error(error_message);
		return false;
	}

	return true;
}

#define ERROR_CHECK(result) check_error(result, __FUNCTION__, __FILE__, __LINE__)
#define ERROR_CHECK_MSG(result, message) check_error(result, __FUNCTION__, __FILE__, __LINE__, message)

#define RETURN_IF_EDITOR                                                                                               \
	if (Engine::get_singleton()->is_editor_hint())                                                                     \
	{                                                                                                                  \
		return;                                                                                                        \
	}

#define ADD_ALL_AK_EVENT_SIGNALS                                                                                       \
	ADD_SIGNAL(MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_END_OF_EVENT],  \
			PropertyInfo(Variant::DICTIONARY, "data")));                                                               \
	ADD_SIGNAL(MethodInfo(AkUtils::get_singleton()                                                                     \
								  ->event_callback_signals[AkUtils::AkCallbackType::AK_END_OF_DYNAMIC_SEQUENCE_ITEM],  \
			PropertyInfo(Variant::DICTIONARY, "data")));                                                               \
	ADD_SIGNAL(MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_MARKER],        \
			PropertyInfo(Variant::DICTIONARY, "data")));                                                               \
	ADD_SIGNAL(MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_DURATION],      \
			PropertyInfo(Variant::DICTIONARY, "data")));                                                               \
	ADD_SIGNAL(MethodInfo(                                                                                             \
			AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_SPEAKER_VOLUME_MATRIX],       \
			PropertyInfo(Variant::DICTIONARY, "data")));                                                               \
	ADD_SIGNAL(MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_STARVATION],    \
			PropertyInfo(Variant::DICTIONARY, "data")));                                                               \
	ADD_SIGNAL(MethodInfo(                                                                                             \
			AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_MUSIC_PLAYLIST_SELECT],       \
			PropertyInfo(Variant::DICTIONARY, "data")));                                                               \
	ADD_SIGNAL(MethodInfo(                                                                                             \
			AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_MUSIC_PLAY_STARTED],          \
			PropertyInfo(Variant::DICTIONARY, "data")));                                                               \
	ADD_SIGNAL(                                                                                                        \
			MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_MUSIC_SYNC_BEAT],  \
					PropertyInfo(Variant::DICTIONARY, "data")));                                                       \
	ADD_SIGNAL(                                                                                                        \
			MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_MUSIC_SYNC_BAR],   \
					PropertyInfo(Variant::DICTIONARY, "data")));                                                       \
	ADD_SIGNAL(                                                                                                        \
			MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_MUSIC_SYNC_ENTRY], \
					PropertyInfo(Variant::DICTIONARY, "data")));                                                       \
	ADD_SIGNAL(                                                                                                        \
			MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_MUSIC_SYNC_EXIT],  \
					PropertyInfo(Variant::DICTIONARY, "data")));                                                       \
	ADD_SIGNAL(                                                                                                        \
			MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_MUSIC_SYNC_GRID],  \
					PropertyInfo(Variant::DICTIONARY, "data")));                                                       \
	ADD_SIGNAL(MethodInfo(                                                                                             \
			AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_MUSIC_SYNC_USER_CUE],         \
			PropertyInfo(Variant::DICTIONARY, "data")));                                                               \
	ADD_SIGNAL(                                                                                                        \
			MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_MUSIC_SYNC_POINT], \
					PropertyInfo(Variant::DICTIONARY, "data")));                                                       \
	ADD_SIGNAL(                                                                                                        \
			MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_MUSIC_SYNC_ALL],   \
					PropertyInfo(Variant::DICTIONARY, "data")));                                                       \
	ADD_SIGNAL(MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_MIDI_EVENT],    \
			PropertyInfo(Variant::DICTIONARY, "data")));                                                               \
	ADD_SIGNAL(MethodInfo(AkUtils::get_singleton()->event_callback_signals[AkUtils::AkCallbackType::AK_CALLBACK_BITS], \
			PropertyInfo(Variant::DICTIONARY, "data")));                                                               \
	ADD_SIGNAL(                                                                                                        \
			MethodInfo(AkUtils::get_singleton()                                                                        \
							   ->event_callback_signals[AkUtils::AkCallbackType::AK_ENABLE_GET_SOURCE_PLAY_POSITION],  \
					PropertyInfo(Variant::DICTIONARY, "data")));                                                       \
	ADD_SIGNAL(                                                                                                        \
			MethodInfo(AkUtils::get_singleton()                                                                        \
							   ->event_callback_signals[AkUtils::AkCallbackType::AK_ENABLE_GET_MUSIC_PLAY_POSITION],   \
					PropertyInfo(Variant::DICTIONARY, "data")));                                                       \
	ADD_SIGNAL(MethodInfo(                                                                                             \
			AkUtils::get_singleton()                                                                                   \
					->event_callback_signals[AkUtils::AkCallbackType::AK_ENABLE_GET_SOURCE_STREAM_BUFFERING],          \
			PropertyInfo(Variant::DICTIONARY, "data")))

enum class VectorType
{
	POSITION,
	FORWARD,
	UP
};

static inline void vector3_to_akvector(const Vector3& in_vector, AkVector& out_vector)
{
	out_vector.X = in_vector.x;
	out_vector.Y = in_vector.y;
	out_vector.Z = in_vector.z * -1.0f;
}

static inline void get_akvector(const Transform3D& t, AkVector& out_vector, const VectorType& type)
{
	switch (type)
	{
		case VectorType::POSITION:
			vector3_to_akvector(t.get_origin(), out_vector);
			break;
		case VectorType::FORWARD:
			vector3_to_akvector(-t.get_basis().get_column(2).normalized(), out_vector);
			break;
		case VectorType::UP:
			vector3_to_akvector(t.get_basis().get_column(1).normalized(), out_vector);
			break;
		default:
			break;
	}
}

static inline bool find_matching_vertex(Vector3 vertex, Dictionary vert_dict, int& out_idx)
{
	if (vert_dict.has(vertex))
	{
		out_idx = vert_dict[vertex];
		return true;
	}
	else
	{
		return false;
	}
}
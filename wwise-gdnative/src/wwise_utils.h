#ifndef WWISE_UTILS_H
#define WWISE_UTILS_H

#include "AK/SoundEngine/Common/AkTypes.h"

using namespace godot;

static const char* Wwise_ErrorString(AKRESULT errcode)
{
	switch (errcode)
	{
	case	AK_NotImplemented:				return "AK_NotImplemented";
	case	AK_Success:						return "AK_Success";
	case	AK_Fail:						return "AK_Fail";
	case	AK_PartialSuccess:				return "AK_PartialSuccess";
	case	AK_NotCompatible:				return "AK_NotCompatible";
	case	AK_AlreadyConnected:			return "AK_AlreadyConnected";
	case	AK_InvalidFile:					return "AK_InvalidFile";
	case	AK_AudioFileHeaderTooLarge:		return "AK_AudioFileHeaderTooLarge";
	case	AK_MaxReached:					return "AK_MaxReached";
	case	AK_InvalidID:					return "AK_InvalidID";
	case	AK_IDNotFound:					return "AK_IDNotFound";
	case	AK_InvalidInstanceID:			return "AK_InvalidInstanceID";
	case	AK_NoMoreData:					return "AK_NoMoreData";
	case	AK_InvalidStateGroup:			return "AK_InvalidStateGroup";
	case	AK_ChildAlreadyHasAParent:		return "AK_ChildAlreadyHasAParent";
	case	AK_InvalidLanguage:				return "AK_InvalidLanguage";
	case	AK_CannotAddItseflAsAChild:		return "AK_CannotAddItseflAsAChild";
	case	AK_InvalidParameter:			return "AK_InvalidParameter";
	case	AK_ElementAlreadyInList:		return "AK_ElementAlreadyInList";
	case	AK_PathNotFound:				return "AK_PathNotFound";
	case	AK_PathNoVertices:				return "AK_PathNoVertices";
	case	AK_PathNotRunning:				return "AK_PathNotRunning";
	case	AK_PathNotPaused:				return "AK_PathNotPaused";
	case	AK_PathNodeAlreadyInList:		return "AK_PathNodeAlreadyInList";
	case	AK_PathNodeNotInList:			return "AK_PathNodeNotInList";
	case	AK_DataNeeded:					return "AK_DataNeeded";
	case	AK_NoDataNeeded:				return "AK_NoDataNeeded";
	case	AK_DataReady:					return "AK_DataReady";
	case	AK_NoDataReady:					return "AK_NoDataReady";
	case	AK_InsufficientMemory:			return "AK_InsufficientMemory";
	case	AK_Cancelled:					return "AK_Cancelled";
	case	AK_UnknownBankID:				return "AK_UnknownBankID";
	case	AK_BankReadError:				return "AK_BankReadError";
	case	AK_InvalidSwitchType:			return "AK_InvalidSwitchType";
	case	AK_FormatNotReady:				return "AK_FormatNotReady";
	case	AK_WrongBankVersion:			return "AK_WrongBankVersion	";
	case	AK_FileNotFound:				return "AK_FileNotFound";
	case	AK_DeviceNotReady:				return "AK_DeviceNotReady";
	case	AK_BankAlreadyLoaded:			return "AK_BankAlreadyLoaded";
	case	AK_RenderedFX:					return "AK_RenderedFX";
	case	AK_ProcessNeeded:				return "AK_ProcessNeeded";
	case	AK_ProcessDone:					return "AK_ProcessDone";
	case	AK_MemManagerNotInitialized:	return "AK_MemManagerNotInitialized";
	case	AK_StreamMgrNotInitialized:		return "AK_StreamMgrNotInitialized";
	case	AK_SSEInstructionsNotSupported:	return "AK_SSEInstructionsNotSupported";
	case	AK_Busy:						return "AK_Busy";
	case	AK_UnsupportedChannelConfig:	return "AK_UnsupportedChannelConfig";
	case	AK_PluginMediaNotAvailable:		return "AK_PluginMediaNotAvailable";
	case	AK_MustBeVirtualized:			return "AK_MustBeVirtualized";
	case	AK_CommandTooLarge:				return "AK_CommandTooLarge";
	case	AK_RejectedByFilter:			return "AK_RejectedByFilte";
	case	AK_InvalidCustomPlatformName:	return "AK_InvalidCustomPlatformName";
	case	AK_DLLCannotLoad:				return "AK_DLLCannotLoad";
	case	AK_DLLPathNotFound:				return "AK_DLLPathNotFound";
	case	AK_NoJavaVM:					return "AK_NoJavaVM	";
	case	AK_OpenSLError:					return "AK_OpenSLError";
	case	AK_PluginNotRegistered:			return "AK_PluginNotRegistered";
	case	AK_DataAlignmentError:			return "AK_DataAlignmentError";
	case	AK_DeviceNotCompatible:			return "AK_DeviceNotCompatible";
	case	AK_DuplicateUniqueID:			return "AK_DuplicateUniqueID";
	case	AK_InitBankNotLoaded:			return "AK_InitBankNotLoaded";
	case	AK_DeviceNotFound:				return "AK_DeviceNotFound";
	case	AK_PlayingIDNotFound:			return "AK_PlayingIDNotFound";
	case	AK_InvalidFloatValue:			return "AK_InvalidFloatValue";
	case	AK_FileFormatMismatch:			return "AK_FileFormatMismatch";
	default:                                return "Unknown error";
	};
}

enum class VectorType
{
	POSITION,
	FORWARD,
	UP
};

static AkVector Vector3ToAkVector(const Vector3& inVector)
{
	AkVector outVector;

	outVector.X = inVector.x;
	outVector.Y = inVector.y;
	outVector.Z = inVector.z;

	return outVector;
}

static AkVector GetAkVector(const Transform& t, const VectorType& type)
{
	AkVector outVector;

	switch (type)
	{
	case VectorType::POSITION:
		outVector = Vector3ToAkVector(t.get_origin());
		break;
	case VectorType::FORWARD:
		outVector = Vector3ToAkVector(t.get_basis().elements[2].normalized());
		break;
	case VectorType::UP:
		outVector = Vector3ToAkVector(t.get_basis().elements[1].normalized());
		break;
	default:
		break;
	}

	return outVector;
}

#endif
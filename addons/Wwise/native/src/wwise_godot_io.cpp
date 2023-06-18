#include "wwise_godot_io.h"

#define BLOCKING_DEVICE_NAME AKTEXT("Blocking Device")

using namespace godot;

CAkIOHookBlockingGodot::~CAkIOHookBlockingGodot() { Term(); }

AKRESULT CAkIOHookBlockingGodot::Init(const AkDeviceSettings& in_deviceSettings)
{
	if (in_deviceSettings.uSchedulerTypeFlags != AK_SCHEDULER_BLOCKING)
	{
		AKASSERT(!"CAkIOHookBlockingGodot I/O hook only works with AK_SCHEDULER_BLOCKING devices");
		return AK_Fail;
	}

	deviceID = AK::StreamMgr::CreateDevice(in_deviceSettings, this);
	if (deviceID != AK_INVALID_DEVICE_ID)
		return AK_Success;

	return AK_Fail;
}

void CAkIOHookBlockingGodot::Term()
{
	if (deviceID != AK_INVALID_DEVICE_ID)
	{
		AK::StreamMgr::DestroyDevice(deviceID);
		deviceID = AK_INVALID_DEVICE_ID;
	}
}

AKRESULT CAkIOHookBlockingGodot::Open(const String& filePath, AkOpenMode in_eOpenMode, AkFileDesc& out_fileDesc)
{
	AKRESULT result = AK_Fail;

	FileAccess::ModeFlags openMode;

	switch (in_eOpenMode)
	{
		case AK_OpenModeRead:
		{
			openMode = FileAccess::ModeFlags::READ;

			break;
		}
		case AK_OpenModeWrite:
		{
			openMode = FileAccess::ModeFlags::WRITE;

			break;
		}
		case AK_OpenModeWriteOvrwr:
		{
			openMode = FileAccess::ModeFlags::WRITE_READ;

			break;
		}
		case AK_OpenModeReadWrite:
		{
			openMode = FileAccess::ModeFlags::READ_WRITE;

			break;
		}
		default:
		{
			AKASSERT(!"Unknown open mode");

			break;
		}
	}

	FileHandle* const file_handle = memnew(FileHandle);
	Ref<FileAccess> file = FileAccess::open(filePath, openMode);
	file_handle->file = file;

	if (file->get_open_error() == Error::OK)
	{
		out_fileDesc.iFileSize = static_cast<AkInt64>(file->get_length());
		out_fileDesc.hFile = reinterpret_cast<AkFileHandle>(file_handle);
		out_fileDesc.uSector = 0;
		out_fileDesc.deviceID = deviceID;
		out_fileDesc.pCustomParam = nullptr;
		out_fileDesc.uCustomParamSize = 0;

		result = AK_Success;
	}

	return result;
}

AKRESULT CAkIOHookBlockingGodot::Read(AkFileDesc& in_fileDesc, const AkIoHeuristics& in_heuristics, void* out_pBuffer,
		AkIOTransferInfo& io_transferInfo)
{
	AKASSERT(out_pBuffer != nullptr && in_fileDesc.hFile != AkFileHandle(-1));

	FileHandle* const fileHandle = reinterpret_cast<FileHandle*>(in_fileDesc.hFile);
	Ref<FileAccess> file = fileHandle->file;

	const int64_t fileReadPosition = file->get_position();
	const int64_t wantedFileReadPosition = io_transferInfo.uFilePosition;

	if (fileReadPosition != wantedFileReadPosition)
	{
		file->seek(wantedFileReadPosition);
	}

	PackedByteArray fileBuffer = file->get_buffer(io_transferInfo.uRequestedSize);
	int size = fileBuffer.size();
	const uint8_t* data = fileBuffer.ptr();
	memcpy(out_pBuffer, data, size * sizeof(uint8_t));

	const size_t bytesRead = size;
	AKASSERT(bytesRead == static_cast<size_t>(io_transferInfo.uRequestedSize));

	return (bytesRead > 0) ? AK_Success : AK_Fail;
}

AKRESULT CAkIOHookBlockingGodot::Write(
		AkFileDesc& in_fileDesc, const AkIoHeuristics& in_heuristics, void* in_pData, AkIOTransferInfo& io_transferInfo)
{
	AKASSERT(in_pData != nullptr && in_fileDesc.hFile != AkFileHandle(-1));

	FileHandle* const fileHandle = reinterpret_cast<FileHandle*>(in_fileDesc.hFile);
	Ref<FileAccess> file = fileHandle->file;
	const int64_t fileWritePosition = file->get_position();
	const int64_t wantedFileWritePosition = static_cast<long>(io_transferInfo.uFilePosition);

	if (fileWritePosition != wantedFileWritePosition)
	{
		file->seek(wantedFileWritePosition);
	}

	int bytesLength = io_transferInfo.uRequestedSize;
	PackedByteArray bytes;
	bytes.resize(bytesLength);

	memcpy((void*)bytes.ptr(), in_pData, bytesLength * sizeof(uint8_t));
	file->store_buffer(bytes);
	const size_t bytesWritten = file->get_length();

	AKASSERT(bytesWritten == static_cast<size_t>(io_transferInfo.uRequestedSize));

	return (bytesWritten > 0) ? AK_Success : AK_Fail;
}

AKRESULT CAkIOHookBlockingGodot::Close(AkFileDesc& in_fileDesc)
{
	AKASSERT(in_fileDesc.hFile != AkFileHandle(-1));

	AKRESULT result = AK_Fail;

	FileHandle* const fileHandle = reinterpret_cast<FileHandle*>(in_fileDesc.hFile);
	Ref<FileAccess> file = fileHandle->file;
	file->close();

	if (file->get_error() == Error::OK)
	{
		memfree(fileHandle);
		result = AK_Success;
	}

	return result;
}

AkUInt32 CAkIOHookBlockingGodot::GetBlockSize(AkFileDesc& in_fileDesc) { return 1; }

void CAkIOHookBlockingGodot::GetDeviceDesc(AkDeviceDesc&
#ifndef AK_OPTIMIZED
				out_deviceDesc
#endif
)
{
#ifndef AK_OPTIMIZED
	out_deviceDesc.deviceID = deviceID;
	out_deviceDesc.bCanRead = true;
	out_deviceDesc.bCanWrite = true;

	AK_OSCHAR_TO_UTF16(out_deviceDesc.szDeviceName, BLOCKING_DEVICE_NAME, AK_MONITOR_DEVICENAME_MAXLENGTH);
	out_deviceDesc.uStringSize = (AkUInt32)AKPLATFORM::AkUtf16StrLen(out_deviceDesc.szDeviceName) + 1;
#endif
}

AkUInt32 CAkIOHookBlockingGodot::GetDeviceData() { return 1; }

CAkFileIOHandlerGodot::CAkFileIOHandlerGodot() : asyncOpen(false) {}

AKRESULT CAkFileIOHandlerGodot::Init(const AkDeviceSettings& in_deviceSettings)
{
	if (!AK::StreamMgr::GetFileLocationResolver())
	{
		AK::StreamMgr::SetFileLocationResolver(this);
	}

	if (!blockingDevice.Init(in_deviceSettings))
	{
		return AK_Fail;
	}
	return AK_Success;
}

void CAkFileIOHandlerGodot::Term()
{
	if (AK::StreamMgr::GetFileLocationResolver() == this)
	{
		AK::StreamMgr::SetFileLocationResolver(nullptr);
	}

	blockingDevice.Term();
}

AKRESULT CAkFileIOHandlerGodot::Open(const AkOSChar* in_pszFileName, AkOpenMode in_eOpenMode,
		AkFileSystemFlags* in_pFlags, bool& io_bSyncOpen, AkFileDesc& out_fileDesc)
{
	AKRESULT result = AK_Fail;

	if (io_bSyncOpen || !asyncOpen)
	{
		io_bSyncOpen = true;
		char* fileName;
		CONVERT_OSCHAR_TO_CHAR(in_pszFileName, fileName);
		String finalFilePath = banksPath;

		if (in_pFlags && in_eOpenMode == AK_OpenModeRead)
		{
			if (in_pFlags->uCompanyID == AKCOMPANYID_AUDIOKINETIC && in_pFlags->uCodecID == AKCODECID_BANK &&
					in_pFlags->bIsLanguageSpecific)
			{
				finalFilePath = finalFilePath + languageFolder + "/";
			}
		}

		finalFilePath = finalFilePath + fileName;

		if (blockingDevice.Open(finalFilePath, in_eOpenMode, out_fileDesc) == AK_Success)
		{
			result = AK_Success;
		}
	}
	return result;
}

AKRESULT CAkFileIOHandlerGodot::Open(AkFileID in_fileID, AkOpenMode in_eOpenMode, AkFileSystemFlags* in_pFlags,
		bool& io_bSyncOpen, AkFileDesc& out_fileDesc)
{
	AKRESULT result = AK_Fail;

	if (in_pFlags != nullptr && (io_bSyncOpen || !asyncOpen))
	{
		io_bSyncOpen = true;

		String finalFilePath = banksPath;

		if (in_pFlags && in_eOpenMode == AK_OpenModeRead)
		{
			if (in_pFlags->uCompanyID == AKCOMPANYID_AUDIOKINETIC && in_pFlags->uCodecID == AKCODECID_BANK &&
					in_pFlags->bIsLanguageSpecific)
			{
				finalFilePath = finalFilePath + languageFolder + "/";
			}
		}

		String fileNameFormat = in_pFlags->uCodecID == AKCODECID_BANK ? ".bnk" : ".wem";
		const String fileName = String::num_int64(static_cast<int unsigned>(in_fileID)) + fileNameFormat;
		finalFilePath = finalFilePath + fileName;

		if (blockingDevice.Open(finalFilePath, in_eOpenMode, out_fileDesc) == AK_Success)
		{
			result = AK_Success;
		}
	}
	return result;
}

void CAkFileIOHandlerGodot::SetBanksPath(const String banksPath) { this->banksPath = banksPath; }

void CAkFileIOHandlerGodot::SetLanguageFolder(const String languageFolder) { this->languageFolder = languageFolder; }
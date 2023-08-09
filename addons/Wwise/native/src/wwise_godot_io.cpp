#include "wwise_godot_io.h"

#define BLOCKING_DEVICE_NAME AKTEXT("Blocking Device")

using namespace godot;

CAkIOHookBlockingGodot::~CAkIOHookBlockingGodot() { Term(); }

AKRESULT CAkIOHookBlockingGodot::Init(const AkDeviceSettings& in_device_settings)
{
	if (in_device_settings.uSchedulerTypeFlags != AK_SCHEDULER_BLOCKING)
	{
		AKASSERT(!"CAkIOHookBlockingGodot I/O hook only works with AK_SCHEDULER_BLOCKING devices");
		return AK_Fail;
	}

	device_id = AK::StreamMgr::CreateDevice(in_device_settings, this);
	if (device_id != AK_INVALID_DEVICE_ID)
		return AK_Success;

	return AK_Fail;
}

void CAkIOHookBlockingGodot::Term()
{
	if (device_id != AK_INVALID_DEVICE_ID)
	{
		AK::StreamMgr::DestroyDevice(device_id);
		device_id = AK_INVALID_DEVICE_ID;
	}
}

AKRESULT CAkIOHookBlockingGodot::Open(const String& file_path, AkOpenMode in_open_mode, AkFileDesc& out_file_desc)
{
	AKRESULT result = AK_Fail;

	FileAccess::ModeFlags open_mode;

	switch (in_open_mode)
	{
		case AK_OpenModeRead:
		{
			open_mode = FileAccess::ModeFlags::READ;

			break;
		}
		case AK_OpenModeWrite:
		{
			open_mode = FileAccess::ModeFlags::WRITE;

			break;
		}
		case AK_OpenModeWriteOvrwr:
		{
			open_mode = FileAccess::ModeFlags::WRITE_READ;

			break;
		}
		case AK_OpenModeReadWrite:
		{
			open_mode = FileAccess::ModeFlags::READ_WRITE;

			break;
		}
		default:
		{
			AKASSERT(!"Unknown open mode");

			break;
		}
	}

	FileHandle* const file_handle = memnew(FileHandle);
	Ref<FileAccess> file = FileAccess::open(file_path, open_mode);
	file_handle->file = file;

	if (file->get_open_error() == Error::OK)
	{
		out_file_desc.iFileSize = static_cast<AkInt64>(file->get_length());
		out_file_desc.hFile = reinterpret_cast<AkFileHandle>(file_handle);
		out_file_desc.uSector = 0;
		out_file_desc.deviceID = device_id;
		out_file_desc.pCustomParam = nullptr;
		out_file_desc.uCustomParamSize = 0;

		result = AK_Success;
	}

	return result;
}

AKRESULT CAkIOHookBlockingGodot::Read(AkFileDesc& in_file_desc, const AkIoHeuristics& in_heuristics, void* out_pbuffer,
		AkIOTransferInfo& io_transfer_info)
{
	AKASSERT(out_pbuffer != nullptr && in_file_desc.hFile != AkFileHandle(-1));

	FileHandle* const file_handle = reinterpret_cast<FileHandle*>(in_file_desc.hFile);
	Ref<FileAccess> file = file_handle->file;

	const int64_t file_read_position = file->get_position();
	const int64_t wanted_file_read_position = io_transfer_info.uFilePosition;

	if (file_read_position != wanted_file_read_position)
	{
		file->seek(wanted_file_read_position);
	}

	PackedByteArray file_buffer = file->get_buffer(io_transfer_info.uRequestedSize);
	int size = file_buffer.size();
	const uint8_t* data = file_buffer.ptr();
	memcpy(out_pbuffer, data, size * sizeof(uint8_t));

	const size_t bytes_read = size;
	AKASSERT(bytes_read == static_cast<size_t>(io_transfer_info.uRequestedSize));

	return (bytes_read > 0) ? AK_Success : AK_Fail;
}

AKRESULT CAkIOHookBlockingGodot::Write(AkFileDesc& in_file_desc, const AkIoHeuristics& in_heuristics, void* in_pdata,
		AkIOTransferInfo& io_transfer_info)
{
	AKASSERT(in_pdata != nullptr && in_file_desc.hFile != AkFileHandle(-1));

	FileHandle* const file_handle = reinterpret_cast<FileHandle*>(in_file_desc.hFile);
	Ref<FileAccess> file = file_handle->file;
	const int64_t file_write_position = file->get_position();
	const int64_t wanted_file_write_position = static_cast<long>(io_transfer_info.uFilePosition);

	if (file_write_position != wanted_file_write_position)
	{
		file->seek(wanted_file_write_position);
	}

	int bytes_length = io_transfer_info.uRequestedSize;
	PackedByteArray bytes;
	bytes.resize(bytes_length);

	memcpy((void*)bytes.ptr(), in_pdata, bytes_length * sizeof(uint8_t));
	file->store_buffer(bytes);
	const size_t bytes_written = file->get_length();

	AKASSERT(bytes_written == static_cast<size_t>(io_transfer_info.uRequestedSize));

	return (bytes_written > 0) ? AK_Success : AK_Fail;
}

AKRESULT CAkIOHookBlockingGodot::Close(AkFileDesc& in_file_desc)
{
	AKASSERT(in_file_desc.hFile != AkFileHandle(-1));

	AKRESULT result = AK_Fail;

	FileHandle* const file_handle = reinterpret_cast<FileHandle*>(in_file_desc.hFile);
	Ref<FileAccess> file = file_handle->file;
	file->close();

	if (file->get_error() == Error::OK)
	{
		memfree(file_handle);
		result = AK_Success;
	}

	return result;
}

AkUInt32 CAkIOHookBlockingGodot::GetBlockSize(AkFileDesc& in_file_desc) { return 1; }

void CAkIOHookBlockingGodot::GetDeviceDesc(AkDeviceDesc&
#ifndef AK_OPTIMIZED
				out_device_desc
#endif
)
{
#ifndef AK_OPTIMIZED
	out_device_desc.deviceID = device_id;
	out_device_desc.bCanRead = true;
	out_device_desc.bCanWrite = true;

	AK_OSCHAR_TO_UTF16(out_device_desc.szDeviceName, BLOCKING_DEVICE_NAME, AK_MONITOR_DEVICENAME_MAXLENGTH);
	out_device_desc.uStringSize = (AkUInt32)AKPLATFORM::AkUtf16StrLen(out_device_desc.szDeviceName) + 1;
#endif
}

AkUInt32 CAkIOHookBlockingGodot::GetDeviceData() { return 1; }

CAkFileIOHandlerGodot::CAkFileIOHandlerGodot() : async_open(false) {}

AKRESULT CAkFileIOHandlerGodot::Init(const AkDeviceSettings& in_device_settings)
{
	if (!AK::StreamMgr::GetFileLocationResolver())
	{
		AK::StreamMgr::SetFileLocationResolver(this);
	}

	if (!blocking_device.Init(in_device_settings))
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

	blocking_device.Term();
}

AKRESULT CAkFileIOHandlerGodot::Open(const AkOSChar* in_psz_filename, AkOpenMode in_open_mode,
		AkFileSystemFlags* in_pflags, bool& io_bsync_open, AkFileDesc& out_file_desc)
{
	AKRESULT result = AK_Fail;

	if (io_bsync_open || !async_open)
	{
		io_bsync_open = true;
		char* filename;
		CONVERT_OSCHAR_TO_CHAR(in_psz_filename, filename);
		String final_file_path = banks_path;

		if (in_pflags && in_open_mode == AK_OpenModeRead)
		{
			if (in_pflags->uCompanyID == AKCOMPANYID_AUDIOKINETIC && in_pflags->uCodecID == AKCODECID_BANK &&
					in_pflags->bIsLanguageSpecific)
			{
				final_file_path = final_file_path + language_folder + "/";
			}
		}

		final_file_path = final_file_path + filename;

		if (blocking_device.Open(final_file_path, in_open_mode, out_file_desc) == AK_Success)
		{
			result = AK_Success;
		}
	}
	return result;
}

AKRESULT CAkFileIOHandlerGodot::Open(AkFileID in_file_id, AkOpenMode in_open_mode, AkFileSystemFlags* in_pflags,
		bool& io_bsync_open, AkFileDesc& out_file_desc)
{
	AKRESULT result = AK_Fail;

	if (in_pflags != nullptr && (io_bsync_open || !async_open))
	{
		io_bsync_open = true;

		String final_file_path = banks_path;

		if (in_pflags && in_open_mode == AK_OpenModeRead)
		{
			if (in_pflags->uCompanyID == AKCOMPANYID_AUDIOKINETIC && in_pflags->uCodecID == AKCODECID_BANK &&
					in_pflags->bIsLanguageSpecific)
			{
				final_file_path = final_file_path + language_folder + "/";
			}
		}

		String filename_format = in_pflags->uCodecID == AKCODECID_BANK ? ".bnk" : ".wem";
		const String filename = String::num_int64(static_cast<int unsigned>(in_file_id)) + filename_format;
		final_file_path = final_file_path + filename;

		if (blocking_device.Open(final_file_path, in_open_mode, out_file_desc) == AK_Success)
		{
			result = AK_Success;
		}
	}
	return result;
}

void CAkFileIOHandlerGodot::set_banks_path(const String& banks_path) { this->banks_path = banks_path; }

void CAkFileIOHandlerGodot::set_language_folder(const String& language_folder)
{
	this->language_folder = language_folder;
}
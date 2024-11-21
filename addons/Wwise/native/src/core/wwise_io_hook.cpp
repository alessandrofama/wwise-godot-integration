#include "wwise_io_hook.h"

#define BLOCKING_DEVICE_NAME AKTEXT("Blocking Device")

AkFileDesc* WwiseIOHook::create_descriptor(const AkFileDesc* in_p_copy)
{
	if (!in_p_copy)
		return AkNew(AkMemID_Streaming, AkFileDescType());

	return AkNew(AkMemID_Streaming, AkFileDescType(*(AkFileDescType*)in_p_copy));
}

AKRESULT WwiseIOHook::open(const AkFileOpenData& in_file_open, AkFileDesc*& out_p_file_desc)
{
	AKRESULT result = AK_UnknownFileError;

	out_p_file_desc = create_descriptor();
	if (!out_p_file_desc)
		return AK_InsufficientMemory;

	FileAccess::ModeFlags open_mode{};

	switch (in_file_open.eOpenMode)
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
			open_mode = FileAccess::ModeFlags::WRITE;

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

	bool use_names = in_file_open.fileID == AK_INVALID_FILE_ID;
	String final_file_path{ banks_path };
	String filename_format{};

	if (in_file_open.pFlags)
	{
		if (in_file_open.eOpenMode == AK_OpenModeRead)
		{
			if (in_file_open.pFlags->uCompanyID == AKCOMPANYID_AUDIOKINETIC)
			{
				if (in_file_open.pFlags->uCodecID == AKCODECID_BANK)
				{
					filename_format = ".bnk";
				}
				else
				{
					final_file_path += "Media/";
					filename_format = ".wem";
				}

				if (in_file_open.pFlags->bIsLanguageSpecific)
				{
					final_file_path += language_folder + "/";
				}
			}
			else if (in_file_open.pFlags->uCompanyID == AKCOMPANYID_AUDIOKINETIC_EXTERNAL)
			{
				filename_format = ".wem";
			}
		}
	}

	if (use_names)
	{
		char* filename;
		CONVERT_OSCHAR_TO_CHAR(in_file_open.pszFileName, filename);
		final_file_path += filename;
	}
	else
	{
		final_file_path += String::num_int64(in_file_open.fileID) + filename_format;
	}

	FileHandle* const file_handle = memnew(FileHandle);
	Ref<FileAccess> file = FileAccess::open(final_file_path, open_mode);
	file_handle->file = file;

	if (file->get_open_error() == Error::OK)
	{
		out_p_file_desc->iFileSize = static_cast<AkInt64>(file->get_length());
		out_p_file_desc->hFile = reinterpret_cast<AkFileHandle>(file_handle);
		out_p_file_desc->uSector = 0;
		out_p_file_desc->deviceID = device_id;
		result = AK_Success;
	}
	else
	{
		AkDelete(AkMemID_Streaming, out_p_file_desc);
		memdelete(file_handle);
		out_p_file_desc = nullptr;
	}

	return result;
}

void WwiseIOHook::read(
		AkFileDesc& in_file_desc, const AkIoHeuristics& in_heuristics, AkAsyncIOTransferInfo& io_transfer_info)
{
	AKASSERT(io_transfer_info.pBuffer != nullptr && in_file_desc.hFile != AkFileHandle(-1));

	FileHandle* const file_handle = reinterpret_cast<FileHandle*>(in_file_desc.hFile);
	Ref<FileAccess> file = file_handle->file;

	const int64_t file_read_position = file->get_position();
	const int64_t wanted_file_read_position = io_transfer_info.uFilePosition;

	if (file_read_position != wanted_file_read_position)
	{
		file->seek(wanted_file_read_position);
	}

	PackedByteArray file_buffer = file->get_buffer(io_transfer_info.uRequestedSize);
	int64_t size = file_buffer.size();
	const uint8_t* data = file_buffer.ptr();
	memcpy(io_transfer_info.pBuffer, data, size * sizeof(uint8_t));

	const size_t bytes_read = size;
	AKASSERT(bytes_read == static_cast<size_t>(io_transfer_info.uRequestedSize));

	if (bytes_read == io_transfer_info.uRequestedSize)
	{
		io_transfer_info.pCallback(&io_transfer_info, AK_Success);
		return;
	}

	io_transfer_info.pCallback(&io_transfer_info, AK_Fail);
}

void WwiseIOHook::write(
		AkFileDesc& in_file_desc, const AkIoHeuristics& in_heuristics, AkAsyncIOTransferInfo& io_transfer_info)
{
	AKASSERT(io_transfer_info.pBuffer != nullptr && in_file_desc.hFile != AkFileHandle(-1));

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

	memcpy((void*)bytes.ptr(), io_transfer_info.pBuffer, bytes_length * sizeof(uint8_t));
	file->store_buffer(bytes);
	const size_t bytes_written = file->get_length();

	AKASSERT(bytes_written == static_cast<size_t>(io_transfer_info.uRequestedSize));

	if (bytes_written == io_transfer_info.uRequestedSize)
	{
		io_transfer_info.pCallback(&io_transfer_info, AK_Success);
		return;
	}

	io_transfer_info.pCallback(&io_transfer_info, AK_Fail);
}

void WwiseIOHook::cancel(
		AkFileDesc& in_file_desc, AkAsyncIOTransferInfo& io_transfer_info, bool& io_bcancel_all_transfers_for_this_file)
{
	// not implemented
}

WwiseIOHook::~WwiseIOHook() { term(); }

AKRESULT WwiseIOHook::init(const AkDeviceSettings& in_device_settings)
{
	return AK::StreamMgr::CreateDevice(in_device_settings, this, device_id);
}

void WwiseIOHook::term()
{
	if (device_id != AK_INVALID_DEVICE_ID)
	{
		AK::StreamMgr::DestroyDevice(device_id);
		device_id = AK_INVALID_DEVICE_ID;
	}
}

void WwiseIOHook::BatchOpen(AkUInt32 in_u_num_files, AkAsyncFileOpenData** in_pp_items)
{
	for (int i = 0; i < (int)in_u_num_files; i++)
	{
		AkAsyncFileOpenData* p_item = in_pp_items[i];
		AKRESULT result = open(*p_item, p_item->pFileDesc);
		p_item->pCallback(p_item, result);
	}
}

void WwiseIOHook::BatchRead(AkUInt32 in_u_num_transfers, BatchIoTransferItem* in_p_transfer_items)
{
	for (int i = 0; i < (int)in_u_num_transfers; ++i)
	{
		BatchIoTransferItem io_transfer_item = in_p_transfer_items[i];
		read(*(io_transfer_item.pFileDesc), io_transfer_item.ioHeuristics, *(io_transfer_item.pTransferInfo));
	}
}

void WwiseIOHook::BatchWrite(AkUInt32 in_u_num_transfers, BatchIoTransferItem* in_p_transfer_items)
{
	for (int i = 0; i < (int)in_u_num_transfers; ++i)
	{
		BatchIoTransferItem io_transfer_item = in_p_transfer_items[i];
		write(*(io_transfer_item.pFileDesc), io_transfer_item.ioHeuristics, *(io_transfer_item.pTransferInfo));
	}
}

AKRESULT WwiseIOHook::Close(AkFileDesc* in_file_desc)
{
	AKRESULT result = AK_Fail;

	if (in_file_desc)
	{
		AKASSERT(in_file_desc->hFile != AkFileHandle(-1));

		FileHandle* const file_handle = reinterpret_cast<FileHandle*>(in_file_desc->hFile);
		Ref<FileAccess> file = file_handle->file;
		file->close();

		if (file->get_error() == Error::OK)
		{
			AkDelete(AkMemID_Streaming, in_file_desc);
			memdelete(file_handle);
			result = AK_Success;
		}
	}
	return result;
}

AkUInt32 WwiseIOHook::GetBlockSize(AkFileDesc& in_file_desc) { return 1; }

void WwiseIOHook::GetDeviceDesc(AkDeviceDesc&
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

AkUInt32 WwiseIOHook::GetDeviceData() { return 1; }

AKRESULT WwiseFileIOHandler::init(const AkDeviceSettings& in_device_settings)
{
	if (!AK::StreamMgr::GetFileLocationResolver())
	{
		AK::StreamMgr::SetFileLocationResolver(this);
	}

	if (!device.init(in_device_settings))
	{
		return AK_Fail;
	}
	return AK_Success;
}

void WwiseFileIOHandler::term()
{
	if (AK::StreamMgr::GetFileLocationResolver() == this)
	{
		AK::StreamMgr::SetFileLocationResolver(nullptr);
	}

	device.term();
}

void WwiseFileIOHandler::set_banks_path(const String& banks_path) { device.banks_path = banks_path; }

void WwiseFileIOHandler::set_language_folder(const String& language_folder)
{
	device.language_folder = language_folder;
}
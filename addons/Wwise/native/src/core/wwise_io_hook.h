#pragma once

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/Tools/Common/AkObject.h>

using namespace godot;

class WwiseIOHook : public AK::StreamMgr::IAkLowLevelIOHook
{
	struct FileHandle
	{
		Ref<FileAccess> file;
	};

protected:
	AkDeviceID device_id = AK_INVALID_DEVICE_ID;

private:
	AkFileDesc* create_descriptor(const AkFileDesc* in_p_copy = nullptr);
	AKRESULT open(const AkFileOpenData& in_file_open, AkFileDesc*& out_p_file_desc);
	void read(AkFileDesc& in_file_desc, const AkIoHeuristics& in_heuristics, AkAsyncIOTransferInfo& io_transfer_info);
	void write(AkFileDesc& in_file_desc, const AkIoHeuristics& in_heuristics, AkAsyncIOTransferInfo& io_transfer_info);
	void cancel(AkFileDesc& in_file_desc, AkAsyncIOTransferInfo& io_transfer_info,
			bool& io_bcancel_all_transfers_for_this_file);

public:
	typedef AkFileDesc AkFileDescType;

	String banks_path{};
	String language_folder{};

	~WwiseIOHook() override;

	AKRESULT init(const AkDeviceSettings& in_device_settings);
	void term();

	virtual void BatchOpen(AkUInt32 in_u_num_files, AkAsyncFileOpenData** in_pp_items) override;
	virtual void BatchRead(AkUInt32 in_u_num_transfers, BatchIoTransferItem* in_p_transfer_items) override;
	virtual void BatchWrite(AkUInt32 in_u_num_transfers, BatchIoTransferItem* in_p_transfer_items) override;
	virtual AKRESULT Close(AkFileDesc* in_file_desc) override;
	virtual AkUInt32 GetBlockSize(AkFileDesc& in_file_desc) override;
	virtual void GetDeviceDesc(AkDeviceDesc& out_device_desc) override;
	virtual AkUInt32 GetDeviceData() override;
};

class WwiseFileIOHandler : public AK::StreamMgr::IAkFileLocationResolver
{
private:
	WwiseIOHook device{};

public:
	WwiseFileIOHandler() = default;
	~WwiseFileIOHandler() override = default;

	WwiseFileIOHandler(const WwiseFileIOHandler&) = delete;
	WwiseFileIOHandler& operator=(const WwiseFileIOHandler&) = delete;

	AKRESULT init(const AkDeviceSettings& in_device_settings);
	void term();

	void set_banks_path(const String& banks_path);
	void set_language_folder(const String& language_folder);
};
#ifndef __WWISE_GODOT_IO_H__
#define __WWISE_GODOT_IO_H__

#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <godot_cpp/classes/file_access.hpp>

namespace godot
{
class CAkIOHookBlockingGodot : public AK::StreamMgr::IAkIOHookBlocking
{
	struct FileHandle
	{
		Ref<FileAccess> file;
	};

public:
	~CAkIOHookBlockingGodot() override;

	AKRESULT Init(const AkDeviceSettings& in_device_settings);
	void Term();
	AkDeviceID GetDeviceID() const { return device_id; }
	AKRESULT Open(const String& file_path, AkOpenMode in_open_mode, AkFileDesc& out_file_desc);
	AKRESULT Read(AkFileDesc& in_file_desc, const AkIoHeuristics& in_heuristics, void* out_pbuffer,
			AkIOTransferInfo& io_transfer_info) override;
	AKRESULT Write(AkFileDesc& in_file_desc, const AkIoHeuristics& in_heuristics, void* in_pdata,
			AkIOTransferInfo& io_transfer_info) override;
	AKRESULT Close(AkFileDesc& in_file_desc) override;
	AkUInt32 GetBlockSize(AkFileDesc& in_file_desc) override;
	void GetDeviceDesc(AkDeviceDesc& out_device_desc) override;
	AkUInt32 GetDeviceData() override;

protected:
	AkDeviceID device_id = AK_INVALID_DEVICE_ID;
};

class CAkFileIOHandlerGodot : public AK::StreamMgr::IAkFileLocationResolver
{
public:
	CAkFileIOHandlerGodot();
	~CAkFileIOHandlerGodot() override = default;

	CAkFileIOHandlerGodot(const CAkFileIOHandlerGodot&) = delete;
	CAkFileIOHandlerGodot& operator=(const CAkFileIOHandlerGodot&) = delete;

	AKRESULT Init(const AkDeviceSettings& in_device_settings);
	void Term();

	AKRESULT Open(const AkOSChar* in_psz_filename, AkOpenMode in_open_mode, AkFileSystemFlags* in_pflags,
			bool& io_bsync_open, AkFileDesc& out_file_desc) override;
	AKRESULT Open(AkFileID in_file_id, AkOpenMode in_open_mode, AkFileSystemFlags* in_pflags, bool& io_bsync_open,
			AkFileDesc& out_file_desc) override;

	void set_banks_path(const String& banks_path);
	void set_language_folder(const String& language_folder);

private:
	CAkIOHookBlockingGodot blocking_device;
	String banks_path;
	String language_folder;
	bool async_open;
};

} //namespace godot

#endif
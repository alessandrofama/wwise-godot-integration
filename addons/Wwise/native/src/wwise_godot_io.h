#ifndef __WWISE_GODOT_IO_H__
#define __WWISE_GODOT_IO_H__

#include <godot_cpp/classes/file_access.hpp>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>

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

		AKRESULT Init(const AkDeviceSettings& in_deviceSettings);
		void Term();
		AkDeviceID GetDeviceID() const
		{
			return deviceID;
		}
		AKRESULT Open(const String& filePath, AkOpenMode in_eOpenMode, AkFileDesc& out_fileDesc);
		AKRESULT Read(AkFileDesc& in_fileDesc, const AkIoHeuristics& in_heuristics, void* out_pBuffer,
			AkIOTransferInfo& io_transferInfo) override;
		AKRESULT Write(AkFileDesc& in_fileDesc, const AkIoHeuristics& in_heuristics, void* in_pData,
			AkIOTransferInfo& io_transferInfo) override;
		AKRESULT Close(AkFileDesc& in_fileDesc) override;
		AkUInt32 GetBlockSize(AkFileDesc& in_fileDesc) override;
		void GetDeviceDesc(AkDeviceDesc& out_deviceDesc) override;
		AkUInt32 GetDeviceData() override;

	protected:
		AkDeviceID deviceID = AK_INVALID_DEVICE_ID;
	};

	class CAkFileIOHandlerGodot : public AK::StreamMgr::IAkFileLocationResolver
	{
	public:
		CAkFileIOHandlerGodot();
		~CAkFileIOHandlerGodot() override = default;

		CAkFileIOHandlerGodot(const CAkFileIOHandlerGodot&) = delete;
		CAkFileIOHandlerGodot& operator=(const CAkFileIOHandlerGodot&) = delete;

		AKRESULT Init(const AkDeviceSettings& in_deviceSettings);
		void Term();

		AKRESULT Open(const AkOSChar* in_pszFileName, AkOpenMode in_eOpenMode, AkFileSystemFlags* in_pFlags,
			bool& io_bSyncOpen, AkFileDesc& out_fileDesc) override;
		AKRESULT Open(AkFileID in_fileID, AkOpenMode in_eOpenMode, AkFileSystemFlags* in_pFlags, bool& io_bSyncOpen,
			AkFileDesc& out_fileDesc) override;

		void SetBanksPath(const String banksPath);
		void SetLanguageFolder(const String languageFolder);

	private:
		CAkIOHookBlockingGodot blockingDevice;
		String banksPath;
		String languageFolder;
		bool asyncOpen;
	};

}

#endif
#include "ak_editor_export_plugin.h"

String AkEditorExportPlugin::_get_name() const { return "AkEditorExportPlugin"; }

void AkEditorExportPlugin::_export_begin(
		const PackedStringArray& p_features, bool p_is_debug, const String& p_path, uint32_t p_flags)
{
	WwiseSettings* settings = WwiseSettings::get_singleton();
	String root_output_path = settings->get_setting(settings->common_user_settings.root_output_path);

	std::map<std::string, StringName> platform_settings = { { "windows",
																	settings->project_settings.windows_platform_info },
		{ "macos", settings->project_settings.mac_platform_info },
		{ "linux", settings->project_settings.linux_platform_info },
		{ "ios", settings->project_settings.ios_platform_info },
		{ "android", settings->project_settings.android_platform_info } };

	for (const auto& feature : p_features)
	{
		auto it = platform_settings.find(feature.utf8().get_data());
		if (it != platform_settings.end())
		{
			String platform_info_res_path = settings->get_setting(it->second);
			Ref<WwisePlatformInfo> platform_info = ResourceLoader::get_singleton()->load(platform_info_res_path);
			if (platform_info.is_valid())
			{
				String platform_path = platform_info->get_platform_path();
				String platform_banks_path = root_output_path.path_join(platform_path);
				Ref<DirAccess> dir = DirAccess::open(platform_banks_path);
				if (dir.is_valid())
				{
					add_files_recursive(dir, platform_banks_path);
				}
			}
		}
	}
}

void AkEditorExportPlugin::add_files_recursive(Ref<DirAccess> dir, const String& base_path)
{
	if (!dir.is_valid())
		return;

	dir->list_dir_begin();
	String file_name;

	while ((file_name = dir->get_next()) != String())
	{
		if (file_name == "." || file_name == "..")
			continue;

		String file_path = dir->get_current_dir().path_join(file_name);

		if (dir->current_is_dir())
		{
			Ref<DirAccess> sub_dir = DirAccess::open(file_path);
			if (sub_dir.is_valid())
			{
				add_files_recursive(sub_dir, base_path);
			}
		}
		else
		{
			String extension = file_name.get_extension();
			if (extension == "bnk" || extension == "wem")
			{
				PackedByteArray file_data;
				Ref<FileAccess> file = FileAccess::open(file_path, FileAccess::ModeFlags::READ);
				if (file.is_valid())
				{
					file_data = file->get_buffer(file->get_length());
					file->close();
					add_file(file_path, file_data, false);
				}
			}
		}
	}
	dir->list_dir_end();
}
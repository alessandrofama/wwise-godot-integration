#include "wwise_project_database.h"

WwiseProjectDatabase* WwiseProjectDatabase::singleton = nullptr;
bool WwiseProjectDatabase::project_info_exists = false;

void WwiseProjectDatabase::_bind_methods() { ADD_SIGNAL(MethodInfo("ws_soundbank_directory_updated")); }

WwiseProjectDatabase* WwiseProjectDatabase::get_singleton() { return singleton; }

WwiseProjectDatabase::WwiseProjectDatabase()
{
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

WwiseProjectDatabase::~WwiseProjectDatabase()
{
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}

void WwiseProjectDatabase::init_check(const String& p_directory)
{
	String project_json_path = p_directory.path_join(AkEditorSettings::get_wwise_project_info_file_name());
	project_info_exists = godot::FileAccess::file_exists(project_json_path);
}

void WwiseProjectDatabase::post_init_callback()
{
	const auto* platform_data = DataStructure.Platforms.Find(CurrentPlatform);

	if (platform_data)
	{
		bool subfolders_for_generated_files =
				platform_data->PlatformRef.GetPlatformInfo()->Settings.bSubFoldersForGeneratedFiles;

		WwiseSettings* settings = WwiseSettings::get_singleton();

		settings->set_setting(
				settings->project_settings.create_subfolders_for_generated_files, subfolders_for_generated_files);

		bool use_soundbank_names = platform_data->PlatformRef.GetPlatformInfo()->Settings.bUseSoundBankNames;
		settings->set_setting(settings->project_settings.use_soundbank_names, use_soundbank_names);

		std::map<WwiseDBString, StringName> platform_data_map = {
			{ "Windows", settings->project_settings.windows_platform_info },
			{ "Mac", settings->project_settings.mac_platform_info },
			{ "Linux", settings->project_settings.linux_platform_info },
			{ "iOS", settings->project_settings.ios_platform_info },
			{ "Android", settings->project_settings.android_platform_info },
		};

		const auto& project_info = platform_data->PlatformRef.ProjectInfo.GetProjectInfo();

		for (const auto& platform : project_info->Platforms)
		{
			auto data_it = platform_data_map.find(platform.BasePlatform);

			if (data_it != platform_data_map.end())
			{
				Ref<WwisePlatformInfo> platform_info;
				platform_info.instantiate();
				platform_info->set_platform_name(platform.Name.String.c_str());
				platform_info->set_platform_path(platform.Path.String.c_str());

				TypedArray<WwisePluginInfo> plugin_info_array;
				platform_info->set_plugin_info(plugin_info_array);

				String save_path = AkEditorSettings::get_wwise_resource_relative_path().path_join(
						vformat("%s.tres", platform_info->get_platform_name()));
				ResourceSaver::get_singleton()->save(platform_info, save_path);
				settings->set_setting(data_it->second, save_path);
			}
		}

		ProjectSettings::get_singleton()->save();
	}

	emit_signal("ws_soundbank_directory_updated");
}

bool WwiseProjectDatabase::init(const String& p_directory_path, const String& p_directory_platform_name)
{
	init_check(p_directory_path);
	if (!project_info_exists)
		return false;

	Init(ProjectSettings::get_singleton()->globalize_path(p_directory_path).utf8().get_data(),
			p_directory_platform_name.utf8().get_data());

	return true;
}

void WwiseProjectDatabase::set_current_language(const godot::String& p_language_name)
{
	SetCurrentLanguage(p_language_name.utf8().get_data());
}
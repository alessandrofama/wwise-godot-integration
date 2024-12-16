#pragma once

#include "core/wwise_platform_info.h"
#include "core/wwise_settings.h"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

namespace AkEditorSettings
{
String get_wwise_resource_relative_path() { return "res://Wwise/resources"; }

String get_wwise_project_data_file_name() { return "WwiseProjectData.tres"; }

String get_wwise_project_info_file_name() { return "ProjectInfo.json"; }

String get_wwise_ids_file_name() { return "wwise_ids.gd"; }

String get_project_data_path()
{
	return get_wwise_resource_relative_path().path_join(get_wwise_project_data_file_name());
}

String get_wwise_ids_path() { return get_wwise_resource_relative_path().path_join(get_wwise_ids_file_name()); }

String get_platform_name(String custom_platform_name = String())
{
	WwiseSettings* settings = WwiseSettings::get_singleton();
	String platform_info_res_path;
	Ref<WwisePlatformInfo> platform_info;
	String platform_name;

	auto load_platform_name = [&](const String& setting) -> String
	{
		platform_info_res_path = settings->get_setting(setting);
		if (!platform_info_res_path.is_empty() && FileAccess::file_exists(platform_info_res_path))
		{
			platform_info = ResourceLoader::get_singleton()->load(platform_info_res_path);
		}
		if (platform_info.is_valid())
		{
			return platform_info->get_platform_name();
		}
		return String();
	};

	if (!custom_platform_name.is_empty())
	{
		return custom_platform_name;
	}

#if defined(AK_WIN)
	platform_name = load_platform_name(settings->project_settings.windows_platform_info);
	return platform_name.is_empty() ? "Windows" : platform_name;
#elif defined(AK_MAC_OS_X)
	platform_name = load_platform_name(settings->project_settings.mac_platform_info);
	return platform_name.is_empty() ? "Mac" : platform_name;
#elif defined(AK_LINUX)
	platform_name = load_platform_name(settings->project_settings.linux_platform_info);
	return platform_name.is_empty() ? "Linux" : platform_name;
#else
	UtilityFunctions::push_warning("WwiseGodot: Trying to get platform name for unsupported platform in the editor.");
	return "Windows";
#endif
}
} //namespace AkEditorSettings
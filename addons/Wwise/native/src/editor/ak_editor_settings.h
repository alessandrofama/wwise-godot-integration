#pragma once

#include "core/wwise_settings.h"
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

String get_platform_name()
{
	WwiseSettings* settings = WwiseSettings::get_singleton();

#if defined(AK_WIN)
	String platform_name = settings->get_setting(settings->project_settings.windows_platform_name);
	return platform_name.is_empty() ? "Windows" : platform_name;
#elif defined(AK_MAC_OS_X)
	String platform_name = settings->get_setting(settings->project_settings.mac_platform_name);
	return platform_name.is_empty() ? "Mac" : platform_name;
#elif defined(AK_LINUX)
	String platform_name = settings->get_setting(settings->project_settings.linux_platform_name);
	return platform_name.is_empty() ? "Linux" : platform_name;
#else
	UtilityFunctions::push_warning("WwiseGodot: Trying to get platform name for unsupported platform in the editor.");
	return "Windows";
#endif
}
} //namespace AkEditorSettings
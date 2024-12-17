#include "ak_android_export_plugin.h"

String AkAndroidExportPlugin::_get_name() const { return "AkAndroidExportPlugin"; }

bool AkAndroidExportPlugin::_supports_platform(const Ref<EditorExportPlatform>& platform) const
{
	return platform->get_os_name() == "Android";
}

PackedStringArray AkAndroidExportPlugin::_get_android_libraries(
		const Ref<EditorExportPlatform>& platform, bool debug) const
{
	String lib_path = "res://addons/Wwise/native/lib/android/";
	if (debug)
	{
		lib_path += "debug/WwiseAndroidPlugin-debug.aar";
	}
	else
	{
		lib_path += "release/WwiseAndroidPlugin-release.aar";
	}

	PackedStringArray packed;

	Array arr;
	arr.append(lib_path);
	packed.append_array(arr);
	return packed;
}

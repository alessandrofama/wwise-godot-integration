#include "wwise_platform_info.h"

void WwisePlatformInfo::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_platform_name", "name"), &WwisePlatformInfo::set_platform_name);
	ClassDB::bind_method(D_METHOD("get_platform_name"), &WwisePlatformInfo::get_platform_name);

	ClassDB::bind_method(D_METHOD("set_platform_path", "path"), &WwisePlatformInfo::set_platform_path);
	ClassDB::bind_method(D_METHOD("get_platform_path"), &WwisePlatformInfo::get_platform_path);

	ClassDB::bind_method(D_METHOD("set_plugin_info", "plugin_info"), &WwisePlatformInfo::set_plugin_info);
	ClassDB::bind_method(D_METHOD("get_plugin_info"), &WwisePlatformInfo::get_plugin_info);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "platform_name"), "set_platform_name", "get_platform_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "platform_path"), "set_platform_path", "get_platform_path");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "plugin_info", PROPERTY_HINT_TYPE_STRING,
						 vformat("%d/%d:WwisePluginInfo", Variant::OBJECT, PROPERTY_HINT_RESOURCE_TYPE)),
			"set_plugin_info", "get_plugin_info");
}

void WwisePlatformInfo::set_platform_name(const String& p_name)
{
	platform_name = p_name;
	emit_changed();
}

String WwisePlatformInfo::get_platform_name() const { return platform_name; }

void WwisePlatformInfo::set_platform_path(const String& p_path)
{
	platform_path = p_path;
	emit_changed();
}

String WwisePlatformInfo::get_platform_path() const { return platform_path; }

void WwisePlatformInfo::set_plugin_info(const TypedArray<WwisePluginInfo>& p_plugin_info)
{
	plugin_info = p_plugin_info;
	emit_changed();
}

TypedArray<WwisePluginInfo> WwisePlatformInfo::get_plugin_info() const { return plugin_info; }

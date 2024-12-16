#include "wwise_plugin_info.h"

void WwisePluginInfo::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_plugin_name", "name"), &WwisePluginInfo::set_plugin_name);
	ClassDB::bind_method(D_METHOD("get_plugin_name"), &WwisePluginInfo::get_plugin_name);

	ClassDB::bind_method(D_METHOD("set_plugin_id", "id"), &WwisePluginInfo::set_plugin_id);
	ClassDB::bind_method(D_METHOD("get_plugin_id"), &WwisePluginInfo::get_plugin_id);

	ClassDB::bind_method(D_METHOD("set_dll_name", "name"), &WwisePluginInfo::set_dll_name);
	ClassDB::bind_method(D_METHOD("get_dll_name"), &WwisePluginInfo::get_dll_name);

	ClassDB::bind_method(D_METHOD("set_static_lib_name", "name"), &WwisePluginInfo::set_static_lib_name);
	ClassDB::bind_method(D_METHOD("get_static_lib_name"), &WwisePluginInfo::get_static_lib_name);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "plugin_name"), "set_plugin_name", "get_plugin_name");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "plugin_id"), "set_plugin_id", "get_plugin_id");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "dll_name"), "set_dll_name", "get_dll_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "static_lib_name"), "set_static_lib_name", "get_static_lib_name");
}

void WwisePluginInfo::set_plugin_name(const String& p_name)
{
	plugin_name = p_name;
	emit_changed();
}

String WwisePluginInfo::get_plugin_name() const
{
	return plugin_name;
}

void WwisePluginInfo::set_plugin_id(uint32_t p_id)
{
	plugin_id = p_id;
	emit_changed();
}

uint32_t WwisePluginInfo::get_plugin_id() const { return plugin_id; }

void WwisePluginInfo::set_dll_name(const String& p_name)
{
	dll_name = p_name;
	emit_changed();
}

String WwisePluginInfo::get_dll_name() const { return dll_name; }

void WwisePluginInfo::set_static_lib_name(const String& p_name)
{
	static_lib_name = p_name;
	emit_changed();
}

String WwisePluginInfo::get_static_lib_name() const { return static_lib_name; }
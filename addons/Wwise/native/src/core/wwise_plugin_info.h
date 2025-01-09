#pragma once

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class WwisePluginInfo : public Resource
{
	GDCLASS(WwisePluginInfo, Resource);

protected:
	static void _bind_methods();

private:
	String plugin_name;
	uint32_t plugin_id{};
	String dll_name;
	String static_lib_name;

public:
	void set_plugin_name(const String& p_name);
	String get_plugin_name() const;

	void set_plugin_id(uint32_t p_id);
	uint32_t get_plugin_id() const;

	void set_dll_name(const String& p_name);
	String get_dll_name() const;

	void set_static_lib_name(const String& p_name);
	String get_static_lib_name() const;
};
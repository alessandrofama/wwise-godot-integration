#pragma once

#include "core/wwise_plugin_info.h"
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class WwisePlatformInfo : public Resource
{
	GDCLASS(WwisePlatformInfo, Resource);

protected:
	static void _bind_methods();

private:
	String platform_name;
	String platform_path;
	TypedArray<WwisePluginInfo> plugin_info;

public:
	void set_platform_name(const String& p_name);
	String get_platform_name() const;

	void set_platform_path(const String& p_path);
	String get_platform_path() const;

	void set_plugin_info(const TypedArray<WwisePluginInfo>& p_plugin_info);
	TypedArray<WwisePluginInfo> get_plugin_info() const;
};
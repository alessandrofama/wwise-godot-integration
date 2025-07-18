#pragma once
// clang-format off
#include "editor/wwise_project_database_includes.h"
// clang-format on
#include "core/wwise_platform_info.h"
#include "core/wwise_settings.h"
#include "editor/ak_editor_settings.h"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <map>

using namespace godot;

class WwiseProjectDatabase : public Object
{
	GDCLASS(WwiseProjectDatabase, Object);

protected:
	static void _bind_methods();
	static WwiseProjectDatabase* singleton;

public:
	static WwiseProjectDatabase* get_singleton();
	WwiseProjectDatabase();
	~WwiseProjectDatabase();
	static bool project_info_exists;
	static void init_check(const String& p_directory);
	void post_init_callback();

	bool init(const String& p_directory_path, const String& p_directory_platform_name);
	void set_current_language(const String& p_language_name);
};
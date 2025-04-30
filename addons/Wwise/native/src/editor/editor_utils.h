#pragma once

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_settings.hpp>
#include <godot_cpp/variant/string_name.hpp>

using namespace godot;

#define EDITOR_GET(m_var) _EDITOR_GET(m_var)
static Variant _EDITOR_GET(const String& p_setting)
{
	Ref<EditorSettings> settings = EditorInterface::get_singleton()->get_editor_settings();
	ERR_FAIL_COND_V(!settings.is_valid() || !settings->has_setting(p_setting), Variant());
	return settings->get(p_setting);
}

#define SNAME(m_arg)                                                                                                   \
	(                                                                                                                  \
			[]() -> const StringName&                                                                                  \
			{                                                                                                          \
				static StringName sname = StringName(m_arg, true);                                                     \
				return sname;                                                                                          \
			})()
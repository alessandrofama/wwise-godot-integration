#pragma once

#include "core/wwise_object_type.h"
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

class AkEditorUtils : public Object
{
	GDCLASS(AkEditorUtils, Object);

protected:
	static void _bind_methods() {};

private:
	static Ref<Texture2D> *get_cache()
	{
		static Ref<Texture2D> *cache =
				memnew_arr(
						Ref<Texture2D>,
						2 * (int)WwiseObjectType::Max);

		return cache;
	}
static int get_cache_index(bool dark_mode, WwiseObjectType type)
	{
		return (dark_mode ? 1 : 0) * (int)WwiseObjectType::Max + (int)type;
	}

public:
	static Ref<Texture2D> get_editor_icon(const WwiseObjectType p_type);
	static String get_icon_name(const WwiseObjectType p_type);
	static String get_theme_folder(bool dark_mode);
};
#pragma once

#include "core/wwise_platform_info.h"
#include "core/wwise_settings.h"
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/editor_export_plugin.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <map>

using namespace godot;

class AkEditorExportPlugin : public EditorExportPlugin
{
	GDCLASS(AkEditorExportPlugin, EditorExportPlugin);

protected:
	static void _bind_methods() {}

public:
	virtual String _get_name() const override;
	virtual void _export_begin(
			const PackedStringArray& p_features, bool p_is_debug, const String& p_path, uint32_t p_flags) override;
	void add_files_recursive(Ref<DirAccess> dir, const String& base_path);
};
#pragma once

#include <godot_cpp/classes/editor_export_plugin.hpp>
#include <godot_cpp/classes/project_settings.hpp>

using namespace godot;

class AkEditorExportPlugin : public EditorExportPlugin
{
	GDCLASS(AkEditorExportPlugin, EditorExportPlugin);

protected:
	static void _bind_methods() {}

public:
	virtual String _get_name() const override;
	virtual void _export_file(const String& path, const String& type, const PackedStringArray& features) override;
};
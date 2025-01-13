#pragma once

#include "editor/plugins/ak_android_export_plugin.h"
#include "editor/plugins/ak_editor_export_plugin.h"
#include "editor/plugins/ak_event_3d_gizmo_plugin.h"
#include "editor/plugins/ak_wwise_editor_inspector_plugin.h"
#include <godot_cpp/classes/editor_plugin.hpp>

using namespace godot;

class AkEditorPlugin : public EditorPlugin
{
	GDCLASS(AkEditorPlugin, EditorPlugin);

protected:
	static void _bind_methods() {}

private:
	Ref<AkEvent3DGizmoPlugin> gizmo_plugin;
	Ref<AkEditorExportPlugin> export_plugin;
	Ref<AkAndroidExportPlugin> android_export_plugin;
	Ref<AkWwiseEditorInspectorPlugin> inspector_plugin;

public:
	virtual void _enter_tree() override;
	virtual void _exit_tree() override;
};
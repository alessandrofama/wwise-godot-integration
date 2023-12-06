#ifndef WWISE_EDITOR_MANAGER_H
#define WWISE_EDITOR_MANAGER_H

#include "editor/event_gizmo/ak_event_3d_gizmo_plugin.h"
#include "editor/export_plugin/ak_editor_export_plugin.h"
#include "editor/export_plugin/ak_android_export_plugin.h"
#include "editor/inspector_plugin/ak_inspector_plugin.h"
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/project_settings.hpp>

namespace godot
{
class WwiseEditorPlugin : public EditorPlugin
{
	GDCLASS(WwiseEditorPlugin, EditorPlugin);

protected:
	static void _bind_methods() {}

private:
	Ref<AkEvent3DGizmoPlugin> gizmo_plugin;
	Ref<AkEditorExportPlugin> export_plugin;
	Ref<AkAndroidExportPlugin> android_export_plugin;
	Ref<AkInspectorEditorInspectorPlugin> inspector_plugin;

public:
	virtual void _enter_tree() override
	{
		if (!ProjectSettings::get_singleton()->has_setting("autoload/WwiseRuntimeManager"))
		{
			add_autoload_singleton("WwiseRuntimeManager", "res://addons/Wwise/runtime/wwise_runtime_manager.gd");
		}

		gizmo_plugin.instantiate();
		add_node_3d_gizmo_plugin(gizmo_plugin);

		export_plugin.instantiate();
		add_export_plugin(export_plugin);

		android_export_plugin.instantiate();
		add_export_plugin(android_export_plugin);

		inspector_plugin.instantiate();
		add_inspector_plugin(inspector_plugin);
	}

	virtual void _exit_tree() override
	{
		remove_inspector_plugin(inspector_plugin);
		remove_export_plugin(export_plugin);
		remove_export_plugin(android_export_plugin);
		remove_node_3d_gizmo_plugin(gizmo_plugin);
		remove_autoload_singleton("WwiseRuntimeManager");
	}
};

} //namespace godot

#endif

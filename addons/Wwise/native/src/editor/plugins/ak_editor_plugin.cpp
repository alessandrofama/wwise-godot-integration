#include "ak_editor_plugin.h"

void AkEditorPlugin::_enter_tree()
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

void AkEditorPlugin::_exit_tree()
{
	remove_inspector_plugin(inspector_plugin);
	remove_export_plugin(export_plugin);
	remove_export_plugin(android_export_plugin);
	remove_node_3d_gizmo_plugin(gizmo_plugin);
	remove_autoload_singleton("WwiseRuntimeManager");
}

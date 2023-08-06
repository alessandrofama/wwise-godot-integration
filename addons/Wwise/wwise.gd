@tool
extends EditorPlugin

var editor_plugin: EditorPlugin
var initialized: bool = false


func _enter_tree():
	if !ProjectSettings.has_setting("autoload/WwiseRuntimeManager"):
		add_autoload_singleton(
			"WwiseRuntimeManager", "res://addons/Wwise/runtime/wwise_runtime_manager.gd"
		)

	if Engine.is_editor_hint() && !initialized:
		editor_plugin = load("res://addons/Wwise/editor/ak_editor_plugin.gd").new()
		get_editor_interface().get_base_control().add_child(editor_plugin)
		initialized = true


func _exit_tree():
	remove_autoload_singleton("WwiseRuntimeManager")

	if Engine.is_editor_hint():
			get_editor_interface().get_base_control().remove_child(editor_plugin)
			editor_plugin.queue_free()

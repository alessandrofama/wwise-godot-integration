@tool
extends EditorPlugin

var inspector_browser:EditorPlugin

func _enter_tree():
	if !ProjectSettings.has_setting("autoload/WwiseRuntimeManager"):
		add_autoload_singleton("WwiseRuntimeManager", "res://addons/Wwise/runtime/wwise_runtime_manager.gd")
		
	if Engine.is_editor_hint():
		inspector_browser = load("res://addons/wwise/editor/inspector_browser/inspector_browser_editor_plugin.gd").new()
		get_editor_interface().get_base_control().add_child(inspector_browser)

func _exit_tree():

	remove_autoload_singleton("WwiseRuntimeManager")

	if Engine.is_editor_hint():
		get_editor_interface().get_base_control().remove_child(inspector_browser)
		inspector_browser.queue_free()
@tool
extends EditorPlugin

var inspector_browser: EditorPlugin
var editor_plugin: EditorPlugin
var waapi_picker: EditorPlugin
var initialized: bool = false


func _enter_tree():
	if !ProjectSettings.has_setting("autoload/WwiseRuntimeManager"):
		add_autoload_singleton(
			"WwiseRuntimeManager", "res://addons/Wwise/runtime/wwise_runtime_manager.gd"
		)

	if Engine.is_editor_hint() && !initialized:
		inspector_browser = (
			load("res://addons/wwise/editor/inspector_browser/inspector_browser_editor_plugin.gd")
			. new()
		)
		get_editor_interface().get_base_control().add_child(inspector_browser)

		editor_plugin = load("res://addons/Wwise/editor/editor_plugin.gd").new()
		get_editor_interface().get_base_control().add_child(editor_plugin)

		waapi_picker = load("res://addons/Wwise/editor/waapi_picker/waapi_picker.gd").new()
		get_editor_interface().get_base_control().add_child(waapi_picker)
		initialized = true


func _exit_tree():
	remove_autoload_singleton("WwiseRuntimeManager")

	if Engine.is_editor_hint():
		get_editor_interface().get_base_control().remove_child(inspector_browser)
		inspector_browser.queue_free()
		get_editor_interface().get_base_control().remove_child(editor_plugin)
		editor_plugin.queue_free()
		get_editor_interface().get_base_control().remove_child(waapi_picker)
		editor_plugin.queue_free()

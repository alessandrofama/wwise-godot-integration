extends EditorPlugin

var inspector_browser:EditorInspectorPlugin


func _enter_tree():
	inspector_browser = load("res://addons/wwise/editor/inspector_browser/inspector_browser_plugin.gd").new()
	add_inspector_plugin(inspector_browser)

func _exit_tree():
	remove_inspector_plugin(inspector_browser)


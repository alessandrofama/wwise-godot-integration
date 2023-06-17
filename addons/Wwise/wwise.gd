@tool
extends EditorPlugin

func _enter_tree():
	if !ProjectSettings.has_setting("autoload/WwiseRuntimeManager"):
		add_autoload_singleton("WwiseRuntimeManager", "res://addons/Wwise/runtime/wwise_runtime_manager.gd")

func _exit_tree():

	remove_autoload_singleton("WwiseRuntimeManager")

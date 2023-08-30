@tool
extends EditorPlugin

var build_export_plugin: EditorExportPlugin

func _enter_tree():
	build_export_plugin = load("res://addons/Wwise/editor/ak_build_export.gd").AkBuildExport.new()
	add_export_plugin(build_export_plugin)


func _exit_tree():
	remove_export_plugin(build_export_plugin)

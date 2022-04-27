tool
extends EditorPlugin

var custom_nodes_plugin:EditorPlugin
var build_export_plugin:EditorExportPlugin 
var waapi_picker:EditorPlugin
var inspector_browser:EditorPlugin

func _enter_tree():
	add_autoload_singleton("WwiseSettings", "res://addons/wwise/editor/wwise_settings.gd")
	add_autoload_singleton("Wwise", "res://addons/wwise/bin/wwise-gdnative.gdns")

	custom_nodes_plugin = load("res://addons/wwise/editor/wwise_custom_nodes.gd").new()
	build_export_plugin = load("res://addons/wwise/editor/wwise_build_export.gd").WwiseBuildExport.new()
	
	while get_editor_interface() == null:
		yield(get_tree(), "idle_frame")
	while get_editor_interface().get_base_control() == null:
		yield(get_tree(), "idle_frame")
		
	get_editor_interface().get_base_control().add_child(custom_nodes_plugin)

	if (OS.has_feature("Windows") || OS.has_feature("OSX")):
		add_autoload_singleton("Waapi", "res://addons/wwise/bin/waapi-gdnative.gdns")
		if Engine.is_editor_hint():
			waapi_picker = load("res://addons/wwise/editor/waapi_picker/waapi_picker.gd").new()
			get_editor_interface().get_base_control().add_child(waapi_picker)
	
	if Engine.is_editor_hint():
		inspector_browser = load("res://addons/wwise/editor/inspector_browser/inspector_browser_editor_plugin.gd").new()
		get_editor_interface().get_base_control().add_child(inspector_browser)
	
	add_export_plugin(build_export_plugin)
	
func _exit_tree():
	custom_nodes_plugin.queue_free()
	
	remove_export_plugin(build_export_plugin)
	
	remove_autoload_singleton("WwiseSettings")
	remove_autoload_singleton("Wwise")
	
	if (OS.has_feature("Windows") || OS.has_feature("OSX")):
		if Engine.is_editor_hint():
			waapi_picker.connect("tree_exited", self, "_on_waapi_exit")
			get_editor_interface().get_base_control().remove_child(waapi_picker)
			waapi_picker.queue_free()
		else:
			_on_waapi_exit()
			
	if Engine.is_editor_hint():
		get_editor_interface().get_base_control().remove_child(inspector_browser)
		inspector_browser.queue_free()

func _on_waapi_exit():
	remove_autoload_singleton("Waapi")
	
func handles(object):
	return object.get_class() == "Spatial"
	
func forward_spatial_gui_input(camera, event):
	if (OS.has_feature("Windows") || OS.has_feature("OSX")):
		if Engine.is_editor_hint() && waapi_picker:
			waapi_picker.forward_spatial_gui_input(camera, event)

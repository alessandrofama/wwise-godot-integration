@tool
extends EditorPlugin

var wwise_node_icon: Texture2D = load("res://addons/Wwise/editor/images/wwise_node.svg")
var ak_event_gizmo = load("res://addons/Wwise/editor/ak_event_gizmo.gd").new()
var inspector_browser: EditorPlugin
var waapi_picker: EditorPlugin
var build_export_plugin: EditorExportPlugin


func _enter_tree():
	setup_custom_nodes_icons()
	add_node_3d_gizmo_plugin(ak_event_gizmo)

	inspector_browser = (
		load("res://addons/Wwise/editor/inspector_browser/inspector_browser_editor_plugin.gd").new()
	)
	get_editor_interface().get_base_control().add_child(inspector_browser)
	
	if OS.has_feature("windows") || OS.has_feature("macos"):
		waapi_picker = load("res://addons/Wwise/editor/waapi_picker/waapi_picker.gd").new()
		get_editor_interface().get_base_control().add_child(waapi_picker)

	build_export_plugin = load("res://addons/Wwise/editor/ak_build_export.gd").AkBuildExport.new()
	add_export_plugin(build_export_plugin)


func _exit_tree():
	remove_node_3d_gizmo_plugin(ak_event_gizmo)

	get_editor_interface().get_base_control().remove_child(inspector_browser)
	inspector_browser.queue_free()
	
	if OS.has_feature("windows") || OS.has_feature("macos"):
		get_editor_interface().get_base_control().remove_child(waapi_picker)
		waapi_picker.queue_free()

	remove_export_plugin(build_export_plugin)
	build_export_plugin.free()


func setup_custom_nodes_icons():
	# note(alex): Directly setting the icons on the theme slows down startup time
	# considerably. We duplicate the theme to avoid this here.
	var theme := get_editor_interface().get_base_control().get_theme().duplicate()
	if theme:
		theme.set_icon(&"Wwise", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"Waapi", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"WwiseSettings", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkListener2D", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkListener3D", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkEvent2D", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkEvent3D", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkState", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkSwitch", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkBank", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkEnvironment", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkEnvironmentData", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkGeometry", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkPortal", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkRoom", &"EditorIcons", wwise_node_icon)
		theme.set_icon(&"AkEarlyReflections", &"EditorIcons", wwise_node_icon)
		get_editor_interface().get_base_control().set_theme(theme)

@tool
extends EditorPlugin

var wwise_node_icon: Texture2D = load("res://addons/Wwise/editor/images/wwise_node.svg")


func _enter_tree():
	setup_custom_nodes_icons()


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

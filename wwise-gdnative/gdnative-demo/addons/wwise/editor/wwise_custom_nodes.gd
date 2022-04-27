tool
extends EditorPlugin

var AkEventGizmo = load("res://addons/wwise/editor/ak_event_gizmo.gd")

var ak_event_gizmo = AkEventGizmo.new()

func _enter_tree():
	var node_image = Image.new()
	var err = node_image.load("res://addons/wwise/editor/images/wwise_node.png")
	if err != OK:
		push_error("Failed load custom nodes icon")
		return
	var node_icon = ImageTexture.new()
	node_icon.create_from_image(node_image, 0)

	# 3D Nodes
	add_custom_type("AkEvent", "Spatial", load("res://addons/wwise/runtime/nodes/ak_event.gd"), node_icon)
	add_custom_type("AkBank", "Node", load("res://addons/wwise/runtime/nodes/ak_bank.gd"), node_icon)
	add_custom_type("AkListener", "Spatial", load("res://addons/wwise/runtime/nodes/ak_listener.gd"), node_icon)
	add_custom_type("AkState", "Node", load("res://addons/wwise/runtime/nodes/ak_state.gd"), node_icon)
	add_custom_type("AkSwitch", "Node", load("res://addons/wwise/runtime/nodes/ak_switch.gd"), node_icon)
	add_custom_type("AkEnvironment", "Area", load("res://addons/wwise/runtime/nodes/ak_environment.gd"), node_icon)
	add_spatial_gizmo_plugin(ak_event_gizmo)
	
	# Spatial Audio Nodes
	add_custom_type("AkGeometry", "Spatial", load("res://addons/wwise/runtime/nodes/ak_geometry.gd"), node_icon)
	add_custom_type("AkRoom", "Area", load("res://addons/wwise/runtime/nodes/ak_room.gd"), node_icon)
	add_custom_type("AkPortal", "Area", load("res://addons/wwise/runtime/nodes/ak_portal.gd"), node_icon)
	add_custom_type("AkEarlyReflections", "Node", load("res://addons/wwise/runtime/nodes/ak_early_reflections.gd"), node_icon)
	
	# 2D Nodes
	add_custom_type("AkEvent2D", "Node2D", load("res://addons/wwise/runtime/nodes/ak_event_2d.gd"), node_icon)
	add_custom_type("AkListener2D", "Node2D", load("res://addons/wwise/runtime/nodes/ak_listener_2d.gd"), node_icon)
	
	# Visual Script Nodes
	VisualScriptEditor.add_custom_node("AkLoadBank", "Wwise", load("res://addons/wwise/runtime/visual nodes/ak_load_bank_vs.gd"))
	VisualScriptEditor.add_custom_node("AkUnloadBank", "Wwise", load("res://addons/wwise/runtime/visual nodes/ak_unload_bank_vs.gd"))
	VisualScriptEditor.add_custom_node("AkPostEvent", "Wwise", load("res://addons/wwise/runtime/visual nodes/ak_post_event_vs.gd"))
	VisualScriptEditor.add_custom_node("AkPostEvent2D", "Wwise", load("res://addons/wwise/runtime/visual nodes/ak_post_event_2d_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSet3DPosition", "Wwise", load("res://addons/wwise/runtime/visual nodes/ak_set_3d_position_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSet2DPosition", "Wwise", load("res://addons/wwise/runtime/visual nodes/ak_set_2d_position_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSetState", "Wwise", load("res://addons/wwise/runtime/visual nodes/ak_set_state_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSetSwitch", "Wwise", load("res://addons/wwise/runtime/visual nodes/ak_set_switch_vs.gd"))
	VisualScriptEditor.add_custom_node("AkGetRTPCValue", "Wwise", load("res://addons/wwise/runtime/visual nodes/ak_get_rtpc_value_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSetRTPCValue", "Wwise", load("res://addons/wwise/runtime/visual nodes/ak_set_rtpc_value_vs.gd"))

	# Resources
	add_custom_type("AkAcousticTexture", "Resource", load("res://addons/wwise/runtime/nodes/ak_acoustic_texture.gd"), node_icon)
			
func _exit_tree():
	# 3D Nodes
	remove_custom_type("AkEvent")
	remove_custom_type("AkBank")
	remove_custom_type("AkListener")
	remove_custom_type("AkState")
	remove_custom_type("AkSwitch")
	remove_custom_type("AkEnvironment")
	remove_spatial_gizmo_plugin(ak_event_gizmo)
	
	# Spatial Audio Nodes
	remove_custom_type("AkGeometry")
	remove_custom_type("AkRoom")
	remove_custom_type("AkPortal")
	remove_custom_type("AkEarlyReflections")
	
	# 2D Nodes
	remove_custom_type("AkEvent2D")
	remove_custom_type("AkListener2D")
	
	# Visual Script nodes
	VisualScriptEditor.remove_custom_node("AkLoadBank", "Wwise")
	VisualScriptEditor.remove_custom_node("AkUnloadBank", "Wwise")
	VisualScriptEditor.remove_custom_node("AkPostEvent", "Wwise")
	VisualScriptEditor.remove_custom_node("AkPostEvent2D", "Wwise")
	VisualScriptEditor.remove_custom_node("AkSet3DPosition", "Wwise")
	VisualScriptEditor.remove_custom_node("AkSet2DPosition", "Wwise")
	VisualScriptEditor.remove_custom_node("AkSetState", "Wwise")
	VisualScriptEditor.remove_custom_node("AkSetSwitch", "Wwise")
	VisualScriptEditor.remove_custom_node("AkGetRTPCValue", "Wwise")
	VisualScriptEditor.remove_custom_node("AkSetRTPCValue", "Wwise")

	#Resources
	remove_custom_type("AkAcousticTexture")

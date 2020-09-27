tool
extends EditorPlugin

const AkEventGizmo = preload("res://wwise/editor/ak_event_gizmo.gd")

var ak_event_gizmo = AkEventGizmo.new()

func _enter_tree():
	# 3D Nodes
	add_custom_type("AkEvent", "Spatial", preload("res://wwise/runtime/nodes/ak_event.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkBank", "Spatial", preload("res://wwise/runtime/nodes/ak_bank.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkListener", "Spatial", preload("res://wwise/runtime/nodes/ak_listener.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkState", "Node", preload("res://wwise/runtime/nodes/ak_state.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkSwitch", "Node", preload("res://wwise/runtime/nodes/ak_switch.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkEnvironment", "Area", preload("res://wwise/runtime/nodes/ak_environment.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_spatial_gizmo_plugin(ak_event_gizmo)
	
	# Spatial Audio Nodes
	add_custom_type("AkGeometry", "Spatial", preload("res://wwise/runtime/nodes/ak_geometry.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkRoom", "Area", preload("res://wwise/runtime/nodes/ak_room.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkPortal", "Area", preload("res://wwise/runtime/nodes/ak_portal.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkEarlyReflections", "Node", preload("res://wwise/runtime/nodes/ak_early_reflections.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	
	# 2D Nodes
	add_custom_type("AkEvent2D", "Node2D", preload("res://wwise/runtime/nodes/ak_event_2d.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkBank2D", "Node2D", preload("res://wwise/runtime/nodes/ak_bank_2d.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkListener2D", "Node2D", preload("res://wwise/runtime/nodes/ak_listener_2d.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	
	# Visual Script Nodes
	VisualScriptEditor.add_custom_node("AkLoadBank", "Wwise", preload("res://wwise/runtime/visual nodes/ak_load_bank_vs.gd"))
	VisualScriptEditor.add_custom_node("AkUnloadBank", "Wwise", preload("res://wwise/runtime/visual nodes/ak_unload_bank_vs.gd"))
	VisualScriptEditor.add_custom_node("AkPostEvent", "Wwise", preload("res://wwise/runtime/visual nodes/ak_post_event_vs.gd"))
	VisualScriptEditor.add_custom_node("AkPostEvent2D", "Wwise", preload("res://wwise/runtime/visual nodes/ak_post_event_2d_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSet3DPosition", "Wwise", preload("res://wwise/runtime/visual nodes/ak_set_3d_position_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSet2DPosition", "Wwise", preload("res://wwise/runtime/visual nodes/ak_set_2d_position_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSetState", "Wwise", preload("res://wwise/runtime/visual nodes/ak_set_state_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSetSwitch", "Wwise", preload("res://wwise/runtime/visual nodes/ak_set_switch_vs.gd"))
	VisualScriptEditor.add_custom_node("AkGetRTPCValue", "Wwise", preload("res://wwise/runtime/visual nodes/ak_get_rtpc_value_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSetRTPCValue", "Wwise", preload("res://wwise/runtime/visual nodes/ak_set_rtpc_value_vs.gd"))

	# Resources
	add_custom_type("AkAcousticTexture", "Resource", preload("res://wwise/runtime/nodes/ak_acoustic_texture.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
			
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
	remove_custom_type("AkBank2D")
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

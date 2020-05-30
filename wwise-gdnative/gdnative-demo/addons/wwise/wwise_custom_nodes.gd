tool
extends EditorPlugin

const AkEventGizmo = preload("res://wwise/editor/ak_event_gizmo.gd")

var ak_event_gizmo = AkEventGizmo.new()

func _enter_tree():
	# 3D Nodes
	add_custom_type("AkEvent", "Spatial", preload("res://wwise/nodes/ak_event.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkBank", "Spatial", preload("res://wwise/nodes/ak_bank.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkListener", "Spatial", preload("res://wwise/nodes/ak_listener.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkState", "Spatial", preload("res://wwise/nodes/ak_state.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkSwitch", "Spatial", preload("res://wwise/nodes/ak_switch.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkEnvironment", "Area", preload("res://wwise/nodes/ak_environment.gd"), preload("res://icon_small_node.png"))
	add_spatial_gizmo_plugin(ak_event_gizmo)
	
	# 2D Nodes
	add_custom_type("AkEvent2D", "Node2D", preload("res://wwise/nodes/ak_event_2d.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkBank2D", "Node2D", preload("res://wwise/nodes/ak_bank_2d.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkListener2D", "Node2D", preload("res://wwise/nodes/ak_listener_2d.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkState2D", "Node2D", preload("res://wwise/nodes/ak_state_2d.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkSwitch2D", "Node2D", preload("res://wwise/nodes/ak_switch_2d.gd"), preload("res://icon_small_node.png"))
	
	# Visual Script Nodes
	VisualScriptEditor.add_custom_node("AkLoadBank", "Wwise", preload("res://wwise/visual nodes/ak_load_bank_vs.gd"))
	VisualScriptEditor.add_custom_node("AkUnloadBank", "Wwise", preload("res://wwise/visual nodes/ak_unload_bank_vs.gd"))
	VisualScriptEditor.add_custom_node("AkPostEvent", "Wwise", preload("res://wwise/visual nodes/ak_post_event_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSet3DPosition", "Wwise", preload("res://wwise/visual nodes/ak_set_3d_position_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSetState", "Wwise", preload("res://wwise/visual nodes/ak_set_state_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSetSwitch", "Wwise", preload("res://wwise/visual nodes/ak_set_switch_vs.gd"))
	VisualScriptEditor.add_custom_node("AkGetRTPCValue", "Wwise", preload("res://wwise/visual nodes/ak_get_rtpc_value_vs.gd"))
	VisualScriptEditor.add_custom_node("AkSetRTPCValue", "Wwise", preload("res://wwise/visual nodes/ak_set_rtpc_value_vs.gd"))
			
func _exit_tree():
	# 3D Nodes
	remove_custom_type("AkEvent")
	remove_custom_type("AkBank")
	remove_custom_type("AkListener")
	remove_custom_type("AkState")
	remove_custom_type("AkSwitch")
	remove_custom_type("AkEnvironment")
	remove_spatial_gizmo_plugin(ak_event_gizmo)
	
	# 2D Nodes
	remove_custom_type("AkEvent2D")
	remove_custom_type("AkBank2D")
	remove_custom_type("AkListener2D")
	remove_custom_type("AkState2D")
	remove_custom_type("AkSwitch2D")
	
	# Visual Script nodes
	VisualScriptEditor.remove_custom_node("AkLoadBank", "Wwise")
	VisualScriptEditor.remove_custom_node("AkUnloadBank", "Wwise")
	VisualScriptEditor.remove_custom_node("AkPostEvent", "Wwise")
	VisualScriptEditor.remove_custom_node("AkSet3DPosition", "Wwise")
	VisualScriptEditor.remove_custom_node("AkSetState", "Wwise")
	VisualScriptEditor.remove_custom_node("AkSetSwitch", "Wwise")
	VisualScriptEditor.remove_custom_node("AkGetRTPCValue", "Wwise")
	VisualScriptEditor.remove_custom_node("AkSetRTPCValue", "Wwise")

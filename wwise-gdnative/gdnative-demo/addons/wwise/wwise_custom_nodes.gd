tool
extends EditorPlugin

const AkEventGizmo = preload("res://wwise/editor/ak_event_gizmo.gd")

var ak_event_gizmo = AkEventGizmo.new()

func _enter_tree():
	add_custom_type("AkEvent", "Spatial", preload("res://wwise/nodes/ak_event.gd"), preload("res://addons/wwise/wwise_node.png"))
	add_custom_type("AkBank", "Spatial", preload("res://wwise/nodes/ak_bank.gd"), preload("res://addons/wwise/wwise_node.png"))
	add_custom_type("AkListener", "Spatial", preload("res://wwise/nodes/ak_listener.gd"), preload("res://addons/wwise/wwise_node.png"))
	add_custom_type("AkState", "Spatial", preload("res://wwise/nodes/ak_state.gd"), preload("res://addons/wwise/wwise_node.png"))
	add_custom_type("AkSwitch", "Spatial", preload("res://wwise/nodes/ak_switch.gd"), preload("res://addons/wwise/wwise_node.png"))
	add_custom_type("AkEnvironment", "Area", preload("res://wwise/nodes/ak_environment.gd"), preload("res://addons/wwise/wwise_node.png"))
	add_spatial_gizmo_plugin(ak_event_gizmo)
	
	add_custom_type("AkEvent2D", "Node2D", preload("res://wwise/nodes/ak_event_2d.gd"), preload("res://addons/wwise/wwise_node.png"))
	add_custom_type("AkBank2D", "Node2D", preload("res://wwise/nodes/ak_bank_2d.gd"), preload("res://addons/wwise/wwise_node.png"))
	add_custom_type("AkListener2D", "Node2D", preload("res://wwise/nodes/ak_listener_2d.gd"), preload("res://addons/wwise/wwise_node.png"))
	add_custom_type("AkState2D", "Node2D", preload("res://wwise/nodes/ak_state_2d.gd"), preload("res://addons/wwise/wwise_node.png"))
	add_custom_type("AkSwitch2D", "Node2D", preload("res://wwise/nodes/ak_switch_2d.gd"), preload("res://addons/wwise/wwise_node.png"))
	
func _exit_tree():
	remove_custom_type("AkEvent")
	remove_custom_type("AkBank")
	remove_custom_type("AkListener")
	remove_custom_type("AkState")
	remove_custom_type("AkSwitch")
	remove_custom_type("AkEnvironment")
	remove_spatial_gizmo_plugin(ak_event_gizmo)
	
	remove_custom_type("AkEvent2D")
	remove_custom_type("AkBank2D")
	remove_custom_type("AkListener2D")
	remove_custom_type("AkState2D")
	remove_custom_type("AkSwitch2D")

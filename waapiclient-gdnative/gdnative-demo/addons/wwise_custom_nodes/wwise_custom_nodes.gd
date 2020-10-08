tool
extends EditorPlugin

const AkEventGizmo = preload("res://wwise/editor/ak_event_gizmo.gd")

var ak_event_gizmo = AkEventGizmo.new()

func _enter_tree():
	add_custom_type("AkEvent", "Spatial", preload("res://wwise/runtime/nodes/ak_event.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkBank", "Node", preload("res://wwise/runtime/nodes/ak_bank.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkState", "Node", preload("res://wwise/runtime/nodes/ak_state.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
	add_custom_type("AkSwitch", "Node", preload("res://wwise/runtime/nodes/ak_switch.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))

	add_spatial_gizmo_plugin(ak_event_gizmo)
			
func _exit_tree():
	remove_custom_type("AkEvent")
	remove_custom_type("AkBank")
	remove_custom_type("AkState")
	remove_custom_type("AkSwitch")
	remove_spatial_gizmo_plugin(ak_event_gizmo)

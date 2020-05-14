tool
extends EditorPlugin

const AkEventGizmo = preload("res://wwise/editor/ak_event_gizmo.gd")

var ak_event_gizmo = AkEventGizmo.new()

func _enter_tree():
	add_custom_type("AkEvent", "Spatial", preload("res://wwise/nodes/ak_event.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkBank", "Spatial", preload("res://wwise/nodes/ak_bank.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkListener", "Spatial", preload("res://wwise/nodes/ak_listener.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkState", "Spatial", preload("res://wwise/nodes/ak_state.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkSwitch", "Spatial", preload("res://wwise/nodes/ak_switch.gd"), preload("res://icon_small_node.png"))
	add_spatial_gizmo_plugin(ak_event_gizmo)
	

func _exit_tree():
	remove_custom_type("AkEvent")
	remove_custom_type("AkBank")
	remove_custom_type("AkListener")
	remove_custom_type("AkState")
	remove_custom_type("AkSwitch")
	remove_spatial_gizmo_plugin(ak_event_gizmo)

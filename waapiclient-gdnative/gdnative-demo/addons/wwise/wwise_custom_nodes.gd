tool
extends EditorPlugin

const AkEventGizmo = preload("res://wwise/editor/ak_event_gizmo.gd")

var ak_event_gizmo = AkEventGizmo.new()

func _enter_tree():
	# 3D Nodes
	add_custom_type("AkEvent", "Spatial", preload("res://wwise/nodes/ak_event.gd"), preload("res://addons/wwise/wwise_node.png"))
	add_spatial_gizmo_plugin(ak_event_gizmo)
	
func _exit_tree():
	# 3D Nodes
	remove_custom_type("AkEvent")
	remove_spatial_gizmo_plugin(ak_event_gizmo)
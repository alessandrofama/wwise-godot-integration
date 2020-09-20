tool
extends EditorPlugin

const AkEventGizmo = preload("res://wwise/editor/ak_event_gizmo.gd")

var ak_event_gizmo = AkEventGizmo.new()

func _enter_tree():
	add_custom_type("AkEvent", "Spatial", preload("res://wwise/runtime/nodes/ak_event.gd"), ResourceLoader.load("res://addons/wwise_custom_nodes/wwise_node.png"))
			
func _exit_tree():
	remove_custom_type("AkEvent")
	remove_spatial_gizmo_plugin(ak_event_gizmo)
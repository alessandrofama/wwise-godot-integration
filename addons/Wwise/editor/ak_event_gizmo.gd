extends EditorNode3DGizmoPlugin

var gizmo_image = load("res://addons/Wwise/editor/images/wwise_audio_speaker.svg")


func _init():
	create_icon_material("ak_event_icon", gizmo_image)


func _has_gizmo(for_node_3d: Node3D) -> bool:
	return for_node_3d is AkEvent3D


func _get_gizmo_name():
	return "AkEvent Gizmo"


func _redraw(gizmo: EditorNode3DGizmo):
	gizmo.clear()
	var icon = get_material("ak_event_icon", gizmo)
	gizmo.add_unscaled_billboard(icon, 0.05)

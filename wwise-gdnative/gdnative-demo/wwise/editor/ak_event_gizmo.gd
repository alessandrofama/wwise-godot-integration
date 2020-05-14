extends EditorSpatialGizmoPlugin

const AkEventNode = preload("res://wwise/nodes/ak_event.gd")

func _init():
	create_icon_material("ak_event_icon", preload("res://wwise/editor/WwiseAudioSpeaker.png") as Texture, true)

func has_gizmo(spatial):
	return spatial is AkEventNode
	
func get_name():
	return "AkEvent Gizmo"

func redraw(gizmo):
	gizmo.clear()
	var spatial = gizmo.get_spatial_node()
	var icon = get_material("ak_event_icon", gizmo)
	gizmo.add_unscaled_billboard(icon, 0.05)

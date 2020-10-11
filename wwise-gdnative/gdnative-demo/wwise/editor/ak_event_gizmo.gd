extends EditorSpatialGizmoPlugin

const AkEventNode = preload("res://wwise/runtime/nodes/ak_event.gd")

func _init():
	var gizmo_image = Image.new()
	var err = gizmo_image.load("res://wwise/editor/icon/WwiseAudioSpeaker.png")
	if err != OK:
		push_error("Failed load gizmo icon")
		return
	var gizmo_icon = ImageTexture.new()
	gizmo_icon.create_from_image(gizmo_image, 0)
	
	create_icon_material("ak_event_icon", gizmo_icon, true)

func has_gizmo(spatial):
	return spatial is AkEventNode
	
func get_name():
	return "AkEvent Gizmo"

func redraw(gizmo):
	gizmo.clear()
	var spatial = gizmo.get_spatial_node()
	var icon = get_material("ak_event_icon", gizmo)
	gizmo.add_unscaled_billboard(icon, 0.05)

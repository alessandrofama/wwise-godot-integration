tool
extends EditorPlugin

const ResonanceAudioRoomGizmo = preload("res://addons/resonance_audio/resonance_audio_room_gizmo.gd")

var resonance_audio_room_gizmo = ResonanceAudioRoomGizmo.new()

func _enter_tree():
	var node_image = Image.new()
	var err = node_image.load("res://addons/wwise_custom_nodes/icon/wwise_node.png")
	if err != OK:
		push_error("Failed load custom nodes icon")
		return
	var node_icon = ImageTexture.new()
	node_icon.create_from_image(node_image, 0)

	# 3D Nodes
	add_custom_type("ResonanceAudioRoom", "Spatial", preload("res://addons/resonance_audio/resonance_audio_room.gd"), node_icon)
	add_spatial_gizmo_plugin(resonance_audio_room_gizmo)

func _exit_tree():
	remove_custom_type("ResonanceAudioRoom")
	remove_spatial_gizmo_plugin(resonance_audio_room_gizmo)
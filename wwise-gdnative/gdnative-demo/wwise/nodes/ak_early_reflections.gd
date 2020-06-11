extends Node

export(NodePath) var game_object:NodePath
export(AK.AUX_BUSSES._enum) var reflections_aux_bus
export(float) var reflections_volume:float = 1.0

func _ready() -> void:
	var event = get_node(game_object)
	if not event:
		print_debug("Failed to retrieve AkEvent node")
		return
	Wwise.set_early_reflections_aux_send(event, reflections_aux_bus)
	set_early_reflections_volume(event, reflections_volume)

func set_early_reflections_volume(event:Object, volume:float) -> void:
	Wwise.set_early_reflections_volume(event, volume)

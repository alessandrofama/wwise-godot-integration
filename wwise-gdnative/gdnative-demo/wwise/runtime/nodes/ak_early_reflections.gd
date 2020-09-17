extends Node

export(NodePath) var game_object_path:NodePath
export(AK.AUX_BUSSES._enum) var aux_bus
export(float) var volume:float = 1.0

func _ready() -> void:
	if game_object_path.is_empty():
		push_error("GameObject NodePath in AkEarlyReflections is empty, id: " + String(self.get_instance_id()))
		return
	var game_object = get_node(game_object_path)
	Wwise.set_early_reflections_aux_send(game_object, aux_bus)
	set_early_reflections_volume(game_object, volume)

func set_early_reflections_volume(game_object:Object, reflections_volume:float) -> void:
	Wwise.set_early_reflections_volume(game_object, reflections_volume)

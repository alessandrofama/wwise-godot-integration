extends Node

var aux_array:Array = []

func _init():
	aux_array = []

func set_values(event:Object):
	Wwise.set_game_obj_aux_send_values(event.get_instance_id(), aux_array, aux_array.size())

func reset():
	aux_array.clear()

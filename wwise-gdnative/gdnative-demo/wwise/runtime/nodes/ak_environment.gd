extends Area

export(AK.AUX_BUSSES._enum) var aux_bus:int

export var env_priority = 0

func _enter_tree():
	#warning-ignore:return_value_discarded
	connect("body_entered", self, "_on_body_enter")
	#warning-ignore:return_value_discarded
	connect("body_exited", self, "_on_body_exit")
	
func _on_body_enter(body):
	var children = body.get_children()
	for child in children:
		if child is preload("res://wwise/runtime/nodes/ak_event.gd"):
			if child.is_environment_aware:
				child.ak_environment_data.add_environment(self)

func _on_body_exit(body):
	var children = body.get_children()
	for child in children:
		if child is preload("res://wwise/runtime/nodes/ak_event.gd"):
			if child.is_environment_aware:
				child.ak_environment_data.remove_environment(self)

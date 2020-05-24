extends Area

export(AK.AUX_BUSSES._enum) var aux_bus_id:int
export(NodePath) var listener_node

var ak_aux_send_value:Dictionary

func _enter_tree():
	ak_aux_send_value["aux_bus_id"] = aux_bus_id
	ak_aux_send_value["control_value"] = 0
	
	connect("body_entered", self, "_on_body_enter")
	connect("body_exited", self, "_on_body_exit")

func _on_body_enter(body):
	ak_aux_send_value["control_value"] = 1.0
	var children = body.get_children()
	for child in children:
		if child is AkEvent:
			if child.is_environment_aware:
				child.set_environment(ak_aux_send_value)


func _on_body_exit(body):
	ak_aux_send_value["control_value"] = 0
	var children = body.get_children()
	for child in children:
		if child is AkEvent:
			if child.is_environment_aware:
				child.set_environment(ak_aux_send_value)

extends Area

export(Dictionary) var aux_bus:Dictionary = {"Name": "", "Id": 0}
export(float) var reverb_level:float = 1.0
export(float) var transmission_loss:float = 1.0
export(NodePath) var associated_geometry:NodePath
var geometry_id:int

var ak_event = load("res://addons/wwise/runtime/nodes/ak_event.gd")
var ak_listener = load("res://addons/wwise/runtime/nodes/ak_listener.gd")

func _enter_tree() -> void:
	if not associated_geometry.is_empty():
		var geometry_node = get_node(associated_geometry)
		geometry_id = geometry_node.get_instance_id()
	else:
		geometry_id = 0
		
	var normalized_transform = global_transform.orthonormalized()
	
	Wwise.set_room(self, aux_bus.get("Id"), reverb_level, transmission_loss, normalized_transform.basis.z, normalized_transform.basis.y, geometry_id, self.get_name())
	#warning-ignore:return_value_discarded
	connect("body_entered", self, "_on_body_enter")
	#warning-ignore:return_value_discarded
	connect("body_exited", self, "_on_body_exit")

func _exit_tree() -> void:
	Wwise.remove_room(self)

# If any Event or Listener enters or exites this Area, it will added or removed
# to/from this Room.	
func _on_body_enter(body:Node) -> void:
	var child_nodes:Array = body.get_children()
	# Getting all the children nodes of the nodes above and appending them
	# to the child_nodes array, since the Event or Listener could be nested 
	# somewhere in the tree...
	for child in child_nodes:
		var sub_children = child.get_children()
		for i in sub_children:
			child_nodes.append(i)
	for child in child_nodes:
		if child is ak_event:
			if child.is_spatial:
				add_game_obj_to_room(child, self)
		if child is ak_listener:
			add_game_obj_to_room(child, self)

func _on_body_exit(body:Node) -> void:
	var child_nodes:Array = body.get_children()
	for child in child_nodes:
		var sub_children = child.get_children()
		for i in sub_children:
			child_nodes.append(i)
	for child in child_nodes:
		if child is ak_event:
			if child.is_spatial:
				remove_game_obj_from_room(child)
		if child is ak_listener:
			remove_game_obj_from_room(child)

func add_game_obj_to_room(game_obj:Object, room:Object) -> void:
	Wwise.set_game_obj_in_room(game_obj, room)
	
func remove_game_obj_from_room(game_obj:Object) -> void:
	Wwise.remove_game_obj_from_room(game_obj)

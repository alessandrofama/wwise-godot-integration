extends Area

export(AK.AUX_BUSSES._enum) var aux_bus_id:int

func _ready():
	Wwise.set_room(self, aux_bus_id, self.get_name())

func _enter_tree():
	#warning-ignore:return_value_discarded
	connect("body_entered", self, "_on_body_enter")
	#warning-ignore:return_value_discarded
	connect("body_exited", self, "_on_body_exit")

# If any Event or Listener enters or exites this Area, it will added or removed
# to/from this Room.	
func _on_body_enter(body):
	var child_nodes = body.get_children()
	# Getting all the childs nodes of the nodes above and appending them
	# to the child_nodes array, since the Event or Listener could be nested 
	# somewhere in the tree...
	for child in child_nodes:
		var sub_children = child.get_children()
		for i in sub_children:
			child_nodes.append(i)
	for child in child_nodes:
		if child is AkEvent:
			if child.is_spatial:
				add_game_obj_to_room(child, self)
		if child is AkListener:
			add_game_obj_to_room(child, self)

func _on_body_exit(body):
	var child_nodes = body.get_children()
	for child in child_nodes:
		var sub_children = child.get_children()
		for i in sub_children:
			child_nodes.append(i)
	for child in child_nodes:
		if child is AkEvent:
			if child.is_spatial:
				remove_game_obj_from_room(child)
		if child is AkListener:
			remove_game_obj_from_room(child)

func add_game_obj_to_room(game_obj, room):
	Wwise.set_game_obj_in_room(game_obj, room)
	
func remove_game_obj_from_room(game_obj):
	Wwise.remove_game_obj_from_room(game_obj)
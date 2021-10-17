extends Area

var extent:Vector3
export(NodePath) var front_room:NodePath
var front_room_node:Node
export(NodePath) var back_room:NodePath
var back_room_node:Node
export(bool) var enabled:bool = true setget set_enabled

func _ready() -> void:
	if not front_room.is_empty():		
		front_room_node = get_node(front_room)
	if not back_room.is_empty():
		back_room_node = get_node(back_room)
	extent = self.get_global_transform().basis.get_scale()
	set_portal()

func set_enabled(value: bool) -> void:
	if enabled != value:
		enabled = value
		set_portal()

func set_portal() -> void:
	Wwise.set_portal(self, self.get_global_transform(), extent, front_room_node, 
	back_room_node, enabled, self.get_name())

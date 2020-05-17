extends Node2D

func _ready() -> void:
	Wwise.register_listener(self)
	
func _process(_delta) -> void:
	Wwise.set_2d_position(self, get_global_transform(), self.z_index)

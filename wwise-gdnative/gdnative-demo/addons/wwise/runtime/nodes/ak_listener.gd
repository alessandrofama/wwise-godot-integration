extends Spatial

export(bool) var is_spatial:bool = false

func _ready() -> void:
	Wwise.register_listener(self)
	if is_spatial:
		Wwise.register_spatial_listener(self)
	Wwise.set_3d_position(self, get_global_transform())

func _process(_delta) -> void:
	Wwise.set_3d_position(self, self.get_global_transform())

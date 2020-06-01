extends Spatial
class_name AkListener

export(bool) var is_spatial = false
var last_position:Vector3

func _ready() -> void:
	Wwise.register_listener(self)
	if is_spatial:
		Wwise.register_spatial_listener(self)
	Wwise.set_3d_position(self, get_global_transform())

func _process(_delta) -> void:
	if not get_global_transform().origin == last_position:
		Wwise.set_3d_position(self, self.get_global_transform())

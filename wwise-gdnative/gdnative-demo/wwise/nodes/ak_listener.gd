extends Spatial
class_name AkListener

func _ready() -> void:
	Wwise.register_listener(self)
	Wwise.set_3d_position(self, get_global_transform())

func _process(_delta) -> void:
	Wwise.set_3d_position(self, get_global_transform())

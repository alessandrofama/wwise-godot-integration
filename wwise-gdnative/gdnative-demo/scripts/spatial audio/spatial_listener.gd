extends Spatial

var last_position:Vector3

func _ready():
	Wwise.register_listener(self)
	Wwise.register_spatial_listener(self)

func _process(_delta):
	if not get_global_transform().origin == last_position:
		Wwise.set_3d_position(self, self.get_global_transform())
		last_position = get_global_transform().origin

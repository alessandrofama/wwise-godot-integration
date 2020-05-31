extends Spatial

func _ready():
	Wwise.register_listener(self)
	Wwise.register_spatial_listener(self)

func _process(_delta):
	Wwise.set_3d_position(self, self.get_global_transform())

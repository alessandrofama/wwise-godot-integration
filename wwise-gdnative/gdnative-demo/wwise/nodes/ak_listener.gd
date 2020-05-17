extends Spatial

func _ready() -> void:
	Wwise.register_listener(self)
	
	var listenerResult = Wwise.set_3d_position(self, get_global_transform())
	print("Setting listener position: ", listenerResult)

func _process(_delta) -> void:
	Wwise.set_3d_position(self, get_global_transform())

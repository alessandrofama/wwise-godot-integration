extends Camera

func _ready():
	Wwise.register_listener(self)
	
	var listenerResult = Wwise.set_3d_position(self, get_transform())
	print("Setting listener position: ", listenerResult)

func _process(_delta):
	Wwise.set_3d_position(self, get_transform())
	

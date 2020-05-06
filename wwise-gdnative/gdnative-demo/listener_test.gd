extends Camera

func _ready():
	Wwise.register_listener()
	
	var listenerResult = Wwise.set_listener_position(self)
	print("Setting listener position: ", listenerResult)

func _process(_delta):
	Wwise.set_listener_position(self)
	

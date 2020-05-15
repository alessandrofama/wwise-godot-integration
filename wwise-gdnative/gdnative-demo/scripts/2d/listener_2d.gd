extends Node2D

export var zDepth = 0

func _ready():
	Wwise.register_listener(self)
	
	var listenerResult = Wwise.set_2d_position(self, get_transform(), zDepth)
	print("Setting listener 2d position: ", listenerResult)

func _process(_delta):
	Wwise.set_2d_position(self, get_transform(), zDepth)

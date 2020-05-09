extends Node2D

var playingID

# Called when the node enters the scene tree for the first time.
func _ready():
	var goResult = Wwise.register_game_obj(self, "TestGO")
	print("Registering GameObject: ", goResult)

	playingID = Wwise.post_event_id(AK.EVENTS.MUSIC_2D, self)


func _process(delta):
	Wwise.set_2d_position(self, get_transform(), 0)
	
	var mouse_position = get_viewport().get_mouse_position()
	set_position(mouse_position)

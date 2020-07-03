extends CSGBox

export(float) var rtpcValue = 0.0
var musicVolume:float = -96.0
var playingID:int = 0

export(AK.STATES.MUSICSTATE.STATE._enum) var state = AK.STATES.MUSICSTATE.STATE.CALM

func _ready():
	var goResult = Wwise.register_game_obj(self, "TestGO")
	print("Registering GameObject: ", goResult)

	playingID = Wwise.post_event_id(AK.EVENTS.MUSIC, self)

func _process(_delta):
	musicVolume = Wwise.get_rtpc_id(AK.GAME_PARAMETERS.MUSICVOLUME, self)
	set_scale(Vector3 (stepify(musicVolume * -0.1, 0.001), 
					   stepify(musicVolume * -0.1, 0.001), 
					   stepify(musicVolume * -0.1, 0.001)))
					
	Wwise.set_3d_position(self, get_transform())

	Wwise.set_rtpc_id(AK.GAME_PARAMETERS.ENEMIES, rtpcValue, self)
	Wwise.set_state_id(AK.STATES.MUSICSTATE.GROUP, state)

func _input(_ev):
	if Input.is_key_pressed(KEY_P):
		var stopEventResult = Wwise.stop_event(playingID, 3000, AkUtils.AkCurveInterpolation.LINEAR)
		print("Stop Event: ", stopEventResult)

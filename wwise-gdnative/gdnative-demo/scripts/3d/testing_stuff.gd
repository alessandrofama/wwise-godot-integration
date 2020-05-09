extends CSGBox

export(float) var rtpcValue = 0.0
var musicVolume:float = -96.0
var playingID:int = 0

enum stateValues {
	CALM ,
	INTENSE
}

export(stateValues) var state = stateValues.CALM
var stateValue = AK.STATES.MUSICSTATE.STATE.NONE

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
	
	if (state == stateValues.CALM):
		stateValue = AK.STATES.MUSICSTATE.STATE.CALM

	if (state == stateValues.INTENSE):
		stateValue = AK.STATES.MUSICSTATE.STATE.INTENSE

	Wwise.set_rtpc_id(AK.GAME_PARAMETERS.ENEMIES, rtpcValue, self)
	Wwise.set_state_id(AK.STATES.MUSICSTATE.GROUP, stateValue)

func _input(_ev):
	if Input.is_key_pressed(KEY_P):
		var stopEventResult = Wwise.stop_event(playingID, 3000, AkUtils.AkInterpolationMode.LINEAR)
		print("Stop Event: ", stopEventResult)

extends CSGBox

var musicVolume = -96.0
var playingID = 0

var rtpcSlider = null
var stateButton = null

func _ready():
	var goResult = Wwise.register_game_obj(self, "TestGO")
	print("Registering GameObject: ", goResult)

	playingID = Wwise.post_event_id(AK.EVENTS.MUSIC, self)
	
	rtpcSlider = get_parent().find_node("RTPCSlider")
	var signalResult = rtpcSlider.connect("value_changed", self, "_onRtpcSliderChanged")
	assert(signalResult == OK)
	
	stateButton = get_parent().find_node("StateButton")
	signalResult = stateButton.connect("toggled", self, "_onStateButtonToggled")
	assert(signalResult == OK)
	
func _onRtpcSliderChanged(value):
	Wwise.set_rtpc_id(AK.GAME_PARAMETERS.ENEMIES, value, self)
	
func _onStateButtonToggled(button_pressed):
	if button_pressed:
		Wwise.set_state_id(AK.STATES.MUSICSTATE.GROUP, AK.STATES.MUSICSTATE.STATE.INTENSE)
	else:
		Wwise.set_state_id(AK.STATES.MUSICSTATE.GROUP, AK.STATES.MUSICSTATE.STATE.CALM)

func _process(_delta):
	musicVolume = Wwise.get_rtpc_id(AK.GAME_PARAMETERS.MUSICVOLUME, self)
	set_scale(Vector3 (stepify(musicVolume * -0.1, 0.001), 
					   stepify(musicVolume * -0.1, 0.001), 
					   stepify(musicVolume * -0.1, 0.001)))
					
	Wwise.set_3d_position(self, get_transform())

func _input(_ev):
	if Input.is_key_pressed(KEY_P):
		var stopEventResult = Wwise.stop_event(playingID, 3000, AkUtils.AkCurveInterpolation.LINEAR)
		print("Stop Event: ", stopEventResult)

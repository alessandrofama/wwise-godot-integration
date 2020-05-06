extends CSGBox

export(float) var rtpcValue = 0.0
var musicVolume:float = -96.0
var playingID:int = 0

enum stateValues {
	CALM ,
	INTENSE
}

export(stateValues) var state = stateValues.CALM
var stateValue = "Calm"

func _ready():
	var goResult = Wwise.register_game_obj(self, "TestGO")
	print("Registering GameObject: ", goResult)

	playingID = Wwise.post_event("Music", self)

func _process(_delta):
	musicVolume = Wwise.get_rtpc("MusicVolume", self)
	set_scale(Vector3 (stepify(musicVolume * -0.1, 0.001), 
					   stepify(musicVolume * -0.1, 0.001), 
					   stepify(musicVolume * -0.1, 0.001)))
					
	Wwise.set_3d_position(self)
	
	if (state == stateValues.CALM):
		stateValue = "Calm"

	if (state == stateValues.INTENSE):
		stateValue = "Intense"

	Wwise.set_rtpc("Enemies", rtpcValue, self)
	Wwise.set_state("MusicState", stateValue)

func _input(_ev):
	if Input.is_key_pressed(KEY_P):
		var stopEventResult = Wwise.stop_event(playingID, 3000, AkUtils.AkInterpolationMode.LINEAR)
		print("Stop Event: ", stopEventResult)

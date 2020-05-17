extends AkEventHandler2D

export(AK.EVENTS._enum) var event:int
export(AkUtils.GameEvent) var trigger_on:int = AkUtils.GameEvent.NONE
export(AkUtils.GameEvent) var stop_on:int = AkUtils.GameEvent.NONE
export(int) var stop_fade_time = 0
export (AkUtils.AkInterpolationMode) var interpolation_mode = AkUtils.AkInterpolationMode.LINEAR

var playingID:int

func _init() -> void:
	register_game_object(self, String(event))

func handle_game_event(gameEvent:int) -> void:
	if trigger_on == gameEvent:
		post_event()
	if stop_on == gameEvent:
		stop_event()
		
func post_event() -> void:
	playingID = Wwise.post_event_id(event, self)
	
func stop_event() -> void:
	Wwise.stop_event(playingID, stop_fade_time, interpolation_mode)
	
func _process(_delta):
	Wwise.set_2d_position(self, get_global_transform(), self.z_index)
	

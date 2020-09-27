extends "res://wwise/runtime/helpers/ak_event_handler_2d.gd"

export(AK.EVENTS._enum) var event:int
export(AkUtils.GameEvent) var trigger_on:int = AkUtils.GameEvent.NONE
export(AkUtils.GameEvent) var stop_on:int = AkUtils.GameEvent.NONE
export(int) var stop_fade_time:int = 0
export (AkUtils.AkCurveInterpolation) var interpolation_mode:int = AkUtils.AkCurveInterpolation.LINEAR

var playingID:int

export(bool) var use_callback:bool = false
export(AkUtils.AkCallbackType) var callback_type = AkUtils.AkCallbackType.AK_EndOfEvent
export(NodePath) var callback_receiver:NodePath

func _enter_tree() -> void:
	register_game_object(self, self.get_name())
	
func _ready() -> void:
	if use_callback:
		connect_signals(callback_receiver, callback_type)
				
func handle_game_event(game_event:int) -> void:
	if trigger_on == game_event:
		post_event()
	if stop_on == game_event:
		stop_event()
		
func post_event() -> void:
	if not use_callback:
		playingID = Wwise.post_event_id(event, self)
	else:
		playingID = Wwise.post_event_id_callback(event, callback_type, self)
	
func stop_event() -> void:
	Wwise.stop_event(playingID, stop_fade_time, interpolation_mode)
	
func _process(_delta) -> void:
	Wwise.set_2d_position(self, get_global_transform(), self.z_index)
	

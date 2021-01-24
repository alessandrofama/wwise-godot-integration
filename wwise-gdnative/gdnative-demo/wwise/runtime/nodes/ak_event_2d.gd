extends "res://wwise/runtime/helpers/ak_event_handler_2d.gd"

var playing_id:int

func _enter_tree() -> void:
	register_game_object(self, self.get_name())
	
func _ready() -> void:
	if use_callback:
			for flag in AkUtils.AkCallbackType.values().size():
				if (callback_flag & AkUtils.AkCallbackType.values()[flag] > 0):
					connect_signals(AkUtils.AkCallbackType.values()[flag])
				
func handle_game_event(game_event:int) -> void:
	if trigger_on == game_event:
		post_event()
	if stop_on == game_event:
		stop_event()
		
func post_event() -> void:
	if not use_callback:
		playing_id = Wwise.post_event_id(event, self)
	else:
		playing_id = Wwise.post_event_id_callback(event, callback_flag, self)
	
func stop_event() -> void:
	Wwise.stop_event(playing_id, stop_fade_time, interpolation_mode)
	
func _process(_delta) -> void:
	Wwise.set_2d_position(self, get_global_transform(), self.z_index)

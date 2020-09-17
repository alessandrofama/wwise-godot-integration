extends "res://wwise/runtime/helpers/ak_event_handler_2d.gd"

export(String) var state_group:String
export(String) var state_value:String
export(AkUtils.GameEvent) var trigger_on:int = AkUtils.GameEvent.NONE

func handle_game_event(game_event:int) -> void:
	if trigger_on == game_event:
		set_state()
		
func set_state() -> void:
	Wwise.set_state(state_group, state_value)

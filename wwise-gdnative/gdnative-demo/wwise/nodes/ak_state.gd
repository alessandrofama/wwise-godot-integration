extends AkEventHandler

export(int) var state_group:int
export(int) var state_value:int
export(AkUtils.GameEvent) var trigger_on = AkUtils.GameEvent.NONE

func handle_game_event(gameEvent:int) -> void:
	if trigger_on == gameEvent:
		set_state()
		
func set_state() -> void:
	Wwise.set_state_id(state_group, state_value)

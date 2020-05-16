extends AkEventHandler2D

export(int) var state_group
export(int) var state_value
export(AkUtils.GameEvent) var trigger_on

func handle_game_event(event:int):
	if trigger_on == event:
		set_state()
		
func set_state():
	Wwise.set_state_id(state_group, state_value)

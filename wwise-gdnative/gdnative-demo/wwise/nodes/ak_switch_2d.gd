extends AkEventHandler2D

export(NodePath) var ak_event_2d
export(int) var switch_group
export(int) var switch_value
export(AkUtils.GameEvent) var trigger_on

func handle_game_event(gameEvent:int):
	if trigger_on == gameEvent:
		set_switch()
		
func set_switch():
	var event:Object = get_node(ak_event_2d)
	Wwise.set_switch_id(switch_group, switch_value, event)

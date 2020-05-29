extends AkEventHandler

export(NodePath) var ak_event:NodePath
export(int) var switch_group:int
export(int) var switch_value:int
export(AkUtils.GameEvent) var trigger_on = AkUtils.GameEvent.NONE

func handle_game_event(gameEvent:int) -> void:
	if trigger_on == gameEvent:
		set_switch()
		
func set_switch() -> void:
	var event:Object = get_node(ak_event)
	Wwise.set_switch_id(switch_group, switch_value, event)

extends AkEventHandler2D

export(NodePath) var ak_event_2d:NodePath
export(String) var switch_group:String
export(String) var switch_value:String
export(AkUtils.GameEvent) var trigger_on = AkUtils.GameEvent.NONE

func handle_game_event(game_event:int) -> void:
	if trigger_on == game_event:
		set_switch()
		
func set_switch() -> void:
	if ak_event_2d.is_empty():
		push_error("AkEvent2D NodePath in AkSwitch2D is empty, id: " + String(self.get_instance_id()))
		return
	var event:Object = get_node(ak_event_2d)
	Wwise.set_switch(switch_group, switch_value, event)

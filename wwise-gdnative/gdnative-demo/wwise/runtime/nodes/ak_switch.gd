extends Node

export(String) var group_tag
export(NodePath) var ak_event:NodePath
export(String) var switch_group:String
export(String) var switch_value:String
export(AkUtils.GameEvent) var trigger_on:int = AkUtils.GameEvent.NONE

func handle_game_event(game_event:int) -> void:
	if trigger_on == game_event:
		set_switch()
		
func set_switch() -> void:
	if ak_event.is_empty():
		push_error("AkEvent NodePath in AkSwitch is empty, id: " + String(self.get_instance_id()))
		return
	var event:Object = get_node(ak_event)
	Wwise.set_switch(switch_group, switch_value, event)

func _enter_tree() -> void:
	handle_game_event(AkUtils.GameEvent.TREE_ENTER);
	
func _exit_tree() -> void:
	handle_game_event(AkUtils.GameEvent.TREE_EXIT);

func _ready() -> void:
	handle_game_event(AkUtils.GameEvent.READY);
	
func _on_area_entered(area:Object) -> void:
	if group_tag.empty() or area.is_in_group(group_tag):
		handle_game_event(AkUtils.GameEvent.AREA_ENTER);
	
func _on_area_exited(area:Object) -> void:
	if group_tag.empty() or area.is_in_group(group_tag):
		handle_game_event(AkUtils.GameEvent.AREA_EXIT);
	
func _on_body_entered(body:Object) -> void:
	if group_tag.empty() or body.is_in_group(group_tag):
		handle_game_event(AkUtils.GameEvent.BODY_ENTER);
	
func _on_body_exited(body:Object) -> void:
	if group_tag.empty() or body.is_in_group(group_tag):
		handle_game_event(AkUtils.GameEvent.BODY_EXIT);

extends Node

export(Dictionary) var state_group:Dictionary = {"Name": "", "Id": 0}
export(Dictionary) var state_value:Dictionary = {"Name": "", "Id": 0}
export(AkUtils.GameEvent) var trigger_on:int = AkUtils.GameEvent.NONE
export(String) var group_tag
signal state_set(stage_group, state_value)

func handle_game_event(game_event:int) -> void:
	if trigger_on == game_event:
		set_state()

func set_state() -> void:
	if Wwise.set_state_id(state_group.get("Id"), state_value.get("Id")):
		emit_signal("state_set", state_group, state_value)

func _enter_tree() -> void:
	handle_game_event(AkUtils.GameEvent.TREE_ENTER);
	
func _exit_tree() -> void:
	handle_game_event(AkUtils.GameEvent.TREE_EXIT);

func _ready() -> void:
	handle_game_event(AkUtils.GameEvent.READY);
	
func _on_area_entered(_area:Object) -> void:
	if group_tag.empty() or _area.is_in_group(group_tag):
		handle_game_event(AkUtils.GameEvent.AREA_ENTER);
	
func _on_area_exited(_area:Object) -> void:
	if group_tag.empty() or _area.is_in_group(group_tag):
		handle_game_event(AkUtils.GameEvent.AREA_EXIT);
	
func _on_body_entered(_body:Object) -> void:
	if group_tag.empty() or _body.is_in_group(group_tag):
		handle_game_event(AkUtils.GameEvent.BODY_ENTER);
	
func _on_body_exited(_body:Object) -> void:
	if group_tag.empty() or _body.is_in_group(group_tag):
		handle_game_event(AkUtils.GameEvent.BODY_EXIT);

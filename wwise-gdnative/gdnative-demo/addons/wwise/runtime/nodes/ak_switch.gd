extends Node

export(NodePath) var ak_event:NodePath
export(Dictionary) var switch_group:Dictionary = {"Name": "", "Id": 0}
export(Dictionary) var switch_value:Dictionary = {"Name": "", "Id": 0}
export(AkUtils.GameEvent) var trigger_on:int = AkUtils.GameEvent.NONE
export(String) var group_tag
var event:Object = null
signal set_switch(event, switch_group, switch_value)

func handle_game_event(game_event:int) -> void:
	if trigger_on == game_event:
		set_switch()
		
func set_switch() -> void:
	if ak_event.is_empty():
		push_error("AkEvent NodePath in AkSwitch is empty, id: " + String(self.get_instance_id()))
		return
	event = get_node(ak_event)
	if Wwise.set_switch_id(switch_group.get("Id"), switch_value.get("Id"), event):
		emit_signal("set_switch", event, switch_group.get("Id"), switch_value.get("Id"))

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

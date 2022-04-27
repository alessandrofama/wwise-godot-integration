extends Node

export(String) var group_tag
export(Dictionary) var bank:Dictionary = {"Name": "", "Id": 0}
export(AkUtils.GameEvent) var load_on:int = AkUtils.GameEvent.NONE
export(AkUtils.GameEvent) var unload_on:int = AkUtils.GameEvent.NONE

func handle_game_event(game_event:int) -> void:
	if load_on == game_event:
		load_bank()
	if unload_on == game_event:
		unload_bank()

func load_bank() -> void:
	Wwise.load_bank_id(bank.get("Id"))
	
func unload_bank() -> void:
	Wwise.unload_bank_id(bank.get("Id"))

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

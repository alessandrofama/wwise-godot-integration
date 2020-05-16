extends AkGameObj2D
class_name AkEventHandler2D

export(String) var group_tag

func _enter_tree():
	handle_game_event(AkUtils.GameEvent.TREE_ENTER);
	
func _exit_tree():
	handle_game_event(AkUtils.GameEvent.TREE_EXIT);

func _ready():
	handle_game_event(AkUtils.GameEvent.READY);
	
func _on_area_entered(area:Object):
	if group_tag.empty() or area.is_in_group(group_tag):
		handle_game_event(AkUtils.GameEvent.AREA_ENTER);
	
func _on_area_exited(area:Object):
	if group_tag.empty() or area.is_in_group(group_tag):
		handle_game_event(AkUtils.GameEvent.AREA_EXIT);
	
func _on_body_entered(body:Object):
	if group_tag.empty() or body.is_in_group(group_tag):
		handle_game_event(AkUtils.GameEvent.BODY_ENTER);
	
func _on_body_exited(body:Object):
	if group_tag.empty() or body.is_in_group(group_tag):
		handle_game_event(AkUtils.GameEvent.BODY_EXIT);
	
func handle_game_event(_gameEvent:int):
	pass

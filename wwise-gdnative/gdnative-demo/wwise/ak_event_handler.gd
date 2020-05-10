extends AkGameObj
class_name AkEventHandler

func _enter_tree():
	handle_game_event(AkUtils.GameEvent.TREE_ENTER);
	
func _exit_tree():
	handle_game_event(AkUtils.GameEvent.TREE_EXIT);

func _ready():
	handle_game_event(AkUtils.GameEvent.READY);
	
func _on_area_entered(_area:Object):
	handle_game_event(AkUtils.GameEvent.AREA_ENTER);
	
func _on_area_exited(_area:Object):
	handle_game_event(AkUtils.GameEvent.AREA_EXIT);
	
func _on_body_entered(_body:Object):
	handle_game_event(AkUtils.GameEvent.BODY_ENTER);
	
func _on_body_exited(_body:Object):
	handle_game_event(AkUtils.GameEvent.BODY_EXIT);
	
func handle_game_event(_gameEvent:int):
	pass

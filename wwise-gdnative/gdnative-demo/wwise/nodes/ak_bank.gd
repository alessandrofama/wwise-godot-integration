extends AkGameObj
class_name AkBank

enum banks {initBank = 1355168291, testBank = 3291379323}
export(banks) var bank_id

export(AkUtils.GameEvent) var load_on
export(AkUtils.GameEvent) var unload_on

func _enter_tree():
	Wwise.set_base_path("Banks/Windows/")
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
	
func handle_game_event(gameEvent:int):
	if load_on == gameEvent:
		load_bank()
	if unload_on == gameEvent:
		unload_bank()

func load_bank():
	Wwise.load_bank_id(bank_id)
	
func unload_bank():
	Wwise.unload_bank_id(bank_id)

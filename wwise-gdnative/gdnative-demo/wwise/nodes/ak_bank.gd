extends AkEventHandler
class_name AkBank

enum banks {initBank = 1355168291, testBank = 3291379323}
export(banks) var bank_id

export(AkUtils.GameEvent) var load_on
export(AkUtils.GameEvent) var unload_on

func _init():
	Wwise.set_base_path("Banks/Windows/")
	
func handle_game_event(gameEvent:int):
	if load_on == gameEvent:
		load_bank()
	if unload_on == gameEvent:
		unload_bank()

func load_bank():
	Wwise.load_bank_id(bank_id)
	
func unload_bank():
	Wwise.unload_bank_id(bank_id)

extends AkEventHandler

export(AK.BANKS._enum) var bank:int

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
	Wwise.load_bank_id(bank)
	
func unload_bank():
	Wwise.unload_bank_id(bank)

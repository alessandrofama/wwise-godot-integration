extends AkEventHandler2D

export(AK.BANKS._enum) var bank = AK.BANKS.INIT

export(AkUtils.GameEvent) var load_on:int
export(AkUtils.GameEvent) var unload_on:int

func handle_game_event(gameEvent:int) -> void:
	if load_on == gameEvent:
		load_bank()
	if unload_on == gameEvent:
		unload_bank()

func load_bank() -> void:
	Wwise.load_bank_id(bank)
	print("AkBank2D: " + String(bank) + " loaded")
	
func unload_bank() -> void:
	Wwise.unload_bank_id(bank)

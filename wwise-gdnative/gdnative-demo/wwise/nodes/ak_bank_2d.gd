extends AkEventHandler2D

export(AK.BANKS._enum) var bank:int

export(AkUtils.GameEvent) var load_on:int = AkUtils.GameEvent.NONE
export(AkUtils.GameEvent) var unload_on:int = AkUtils.GameEvent.NONE

func handle_game_event(gameEvent:int) -> void:
	if load_on == gameEvent:
		load_bank()
	if unload_on == gameEvent:
		unload_bank()

func load_bank() -> void:
	Wwise.load_bank_id(bank)
	
func unload_bank() -> void:
	Wwise.unload_bank_id(bank)

extends "res://wwise/runtime/helpers/ak_event_handler.gd"

export(AK.BANKS._enum) var bank:int

export(AkUtils.GameEvent) var load_on:int = AkUtils.GameEvent.NONE
export(AkUtils.GameEvent) var unload_on:int = AkUtils.GameEvent.NONE

func handle_game_event(game_event:int) -> void:
	if load_on == game_event:
		load_bank()
	if unload_on == game_event:
		unload_bank()

func load_bank() -> void:
	Wwise.load_bank_id(bank)
	
func unload_bank() -> void:
	Wwise.unload_bank_id(bank)

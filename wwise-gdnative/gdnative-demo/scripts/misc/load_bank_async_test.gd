extends Node

func _ready():
	Wwise.connect(AkUtils.Signals.BANK_CALLBACK, self, "on_bank_callback")
	Wwise.load_bank_async_id(AK.BANKS.INIT)
	Wwise.load_bank_async_id(AK.BANKS.TESTBANK)

func on_bank_callback(data):
	print(data)


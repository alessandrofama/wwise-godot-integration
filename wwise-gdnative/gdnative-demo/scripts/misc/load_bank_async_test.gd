extends Spatial

var cookie:FuncRef 

func _ready():
	cookie = FuncRef.new()
	cookie.set_instance(self)
	cookie.set_function("on_bank_callback")
	Wwise.load_bank_async_id(AK.BANKS.INIT, cookie)
	Wwise.load_bank_async_id(AK.BANKS.TESTBANK, cookie)

func on_bank_callback(data):
	print(data)
	


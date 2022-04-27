extends Node

export(AkUtils.AkCallbackType) var callback_type = AkUtils.AkCallbackType.AK_MusicSyncBeat

var cookie:FuncRef

func _ready():
	var registerResult = Wwise.register_game_obj(self, "Beat Callback Test")
	print("Registering GameObject: ", registerResult)

	cookie = FuncRef.new() # needs to be an instance variable
	cookie.set_instance(self) # instance in which the function should be called
	cookie.set_function("beat_callback") # name of the function
	
	Wwise.post_event_id_callback(AK.EVENTS.MUSIC, callback_type, self, cookie)

func beat_callback(data):
	print(data)

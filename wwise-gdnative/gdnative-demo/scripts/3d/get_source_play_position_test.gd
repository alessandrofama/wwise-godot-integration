extends Node

export(AK.EVENTS._enum) var event = AK.EVENTS.MUSIC
export(AkUtils.AkCallbackType) var callback_type = AkUtils.AkCallbackType.AK_EnableGetSourcePlayPosition

var playing_id

func _ready():
	var registerResult = Wwise.register_game_obj(self, "Registering Event Test")
	print("Registering Event test: ", registerResult)
	
	playing_id = Wwise.post_event_id_callback(event, callback_type, self)

func _process(_delta):
	var position = Wwise.get_source_play_position(playing_id, true)
	print(position)

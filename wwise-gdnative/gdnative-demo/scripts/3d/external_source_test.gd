extends Node

export(String) var event = "External_Source_Event"
export(String) var external_source = "External_Source"
export(String) var file_name = "ExternalSources/External_Source_Demo.wem"
export(AkUtils.AkCodecID) var codec = AkUtils.AkCodecID.AKCODECID_PCM

func _ready():
	var registerResult = Wwise.register_game_obj(self, "External Source Test")
	print("Registering Event test: ", registerResult)
	
func _input(_ev):
	if Input.is_key_pressed(KEY_SPACE):
		var externalSourcePlayingID = Wwise.post_external_source(event, self, external_source, file_name, codec)
		print("External Source PlayingID: ", externalSourcePlayingID)

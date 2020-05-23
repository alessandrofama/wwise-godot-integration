extends Node

export(AK.EVENTS._enum) var event = AK.EVENTS.EXTERNAL_SOURCE_EVENT
export(AK.EXTERNAL_SOURCES._enum) var external_source = AK.EXTERNAL_SOURCES.EXTERNAL_SOURCE
export(String) var file_name = "ExternalSources/External_Source_Demo.wem"
export(AkUtils.AkCodecID) var codec = AkUtils.AkCodecID.AKCODECID_PCM

func _ready():
	var registerResult = Wwise.register_game_obj(self, "External Source Test")
	print("Registering Event test: ", registerResult)
	
func _input(_ev):
	if Input.is_key_pressed(KEY_SPACE):
		var externalSourcePlayingID = Wwise.post_external_source_id(event, self, external_source, file_name, codec)
		print("External Source PlayingID: ", externalSourcePlayingID)

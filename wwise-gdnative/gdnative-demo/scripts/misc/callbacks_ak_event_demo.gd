extends Node

func _on_AkEvent_music_sync_beat(data):
	print(data)

func _on_AkEvent_music_sync_bar(data):
	pass
	print(data)

func _on_AkEvent_duration(data):
	print(data)

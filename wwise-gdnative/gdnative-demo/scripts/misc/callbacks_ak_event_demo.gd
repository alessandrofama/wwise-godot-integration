extends Node

func _on_AkEvent_music_sync_beat(data):
	print(data)

func _on_AkEvent_end_of_event(data):
	print(data)

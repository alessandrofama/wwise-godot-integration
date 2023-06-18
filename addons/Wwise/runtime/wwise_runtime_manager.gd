extends Node

func _init():
	Wwise.init()
	
func _process(delta):
	Wwise.process(delta)

func _notification(what: int) -> void:
	if what == NOTIFICATION_WM_CLOSE_REQUEST || what == NOTIFICATION_CRASH:
		Wwise.shutdown()

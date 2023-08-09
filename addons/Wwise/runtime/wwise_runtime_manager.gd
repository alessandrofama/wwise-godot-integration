extends Node


func _init():
	Wwise.init()


func _enter_tree():
	get_tree().connect("process_frame", Callable(Wwise, "render_audio"))


func _notification(what: int) -> void:
	if what == NOTIFICATION_WM_CLOSE_REQUEST || what == NOTIFICATION_CRASH:
		Wwise.shutdown()

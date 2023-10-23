extends Node

var render_callable = Callable(Wwise, &"render_audio")
var render_signal = &"process_frame"

func _init():
	Wwise.init()


func _enter_tree():
	get_tree().connect(render_signal, render_callable)


func _notification(what: int) -> void:
	if what == NOTIFICATION_EXIT_TREE || what == NOTIFICATION_CRASH:
		if Wwise.get_soundengine_state() == AkUtils.AK_STATE_SHUTDOWN:
			return
			
		if get_tree().is_connected(render_signal, render_callable):
			get_tree().disconnect(render_signal, render_callable)

		Wwise.shutdown()

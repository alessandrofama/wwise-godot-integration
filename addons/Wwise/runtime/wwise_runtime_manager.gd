extends Node

var suspend_on_focus_loss:bool = false;

func _init() -> void:
	suspend_on_focus_loss = ProjectSettings.get_setting("wwise/common_user_settings/suspend_on_focus_loss", false)
	process_mode = Node.PROCESS_MODE_ALWAYS
	Wwise.init()

	
func _process(_delta: float) -> void:
	if Wwise.is_initialized():
		Wwise.render_audio()


func _notification(what: int) -> void:
	if suspend_on_focus_loss:
		# Handling pause state on desktop platforms
		if what == NOTIFICATION_APPLICATION_FOCUS_OUT:
			pause()
		elif what == NOTIFICATION_APPLICATION_FOCUS_IN:
			resume()
		
	# Handling pause state on Android
	if what == NOTIFICATION_APPLICATION_PAUSED:
		pause()
	elif what == NOTIFICATION_APPLICATION_RESUMED:
		resume()
	
	# Handling shutdown
	if what == NOTIFICATION_EXIT_TREE || what == NOTIFICATION_CRASH:
		if !Wwise.is_initialized():
			return
		Wwise.shutdown()


func pause():
	if Wwise.is_initialized():
		Wwise.suspend(false)


func resume():
	if Wwise.is_initialized():
		Wwise.wakeup_from_suspend()

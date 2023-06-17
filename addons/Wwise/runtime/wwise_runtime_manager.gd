extends Node


# Called when the node enters the scene tree for the first time.
func _ready():
	Wwise.init()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	Wwise.process(delta)

func _notification(what: int) -> void:
	if what == NOTIFICATION_WM_CLOSE_REQUEST || what == NOTIFICATION_CRASH:
		Wwise.shutdown()

extends KinematicBody


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var velocity = Vector3(-250, 0, 0)
var _timer = null
# Called when the node enters the scene tree for the first time.
func _ready():
	_timer = Timer.new()
	add_child(_timer)

	_timer.connect("timeout", self, "_on_Timer_timeout")
	_timer.set_wait_time(5)
	_timer.start()


func _physics_process(delta):
	#warning-ignore:return_value_discarded
	move_and_slide(velocity * delta)

func _on_Timer_timeout():
	velocity = Vector3(velocity.x * -1, 0, 0)
	_timer.start()

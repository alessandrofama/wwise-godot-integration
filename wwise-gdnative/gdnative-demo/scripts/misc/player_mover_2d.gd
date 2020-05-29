extends RigidBody2D

export var speed = 5
export(NodePath) var ak_event

var _timer = null
var timerStarted = false

func _ready():
	_timer = Timer.new()
	add_child(_timer)

	_timer.connect("timeout", self, "_on_Timer_timeout")
	_timer.set_wait_time(0.5)
#	_timer.start()


func _physics_process(delta):
	# Get player input
	var direction: Vector2
	direction.x = Input.get_action_strength("ui_right") - Input.get_action_strength("ui_left")
	direction.y = Input.get_action_strength("ui_down") - Input.get_action_strength("ui_up")
	
	# If input is digital, normalize it for diagonal movement
	if abs(direction.x) == 1 and abs(direction.y) == 1:
		direction = direction.normalized()
	
	# Apply movement
	var movementX = speed * direction.x * delta
	var movementY = speed * direction.y * delta
	var movement = Vector2(movementX, movementY)
	self.translate(movement)
	
	if (abs(direction.x) > 0 or abs(direction.y) > 0) and timerStarted == false:
		_timer.start()
		timerStarted = true
	elif (abs(direction.x) == 0 and abs(direction.y) == 0) and timerStarted == true:
		_timer.stop()
		timerStarted = false

func _on_Timer_timeout():
	var event = get_node(ak_event)
	event.post_event()

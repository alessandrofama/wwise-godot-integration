extends Button

@onready var ak_event: AkEvent2D = $"../AkEvent2D"


func _ready() -> void:
	pressed.connect(_on_pressed)


func _on_pressed() -> void:
	ak_event.post_event()

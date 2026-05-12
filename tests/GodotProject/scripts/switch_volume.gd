extends HSlider


func _ready() -> void:
	value = 1.0
	value_changed.connect(_on_value_changed)


func _on_value_changed(new_value: float) -> void:
	Wwise.set_rtpc_value_id(AK.GAME_PARAMETERS.SWITCHVOLUME, new_value, null)

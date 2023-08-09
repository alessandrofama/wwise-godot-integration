@tool
extends Tree


func _get_drag_data(at_position: Vector2):
	var preview = Label.new()
	if get_selected():
		preview.text = get_selected().get_text(0)
		set_drag_preview(preview)
		return get_selected()  # TreeItem

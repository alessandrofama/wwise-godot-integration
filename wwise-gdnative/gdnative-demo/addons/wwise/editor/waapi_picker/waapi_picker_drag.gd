tool
extends Tree

func get_drag_data(pos):
	var preview = Label.new()
	if get_selected():
		preview.text = get_selected().get_text(0)
		set_drag_preview(preview)
		return get_selected() # TreeItem
	

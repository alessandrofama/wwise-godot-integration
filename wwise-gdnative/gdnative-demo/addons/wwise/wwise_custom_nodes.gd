tool
extends EditorPlugin

func _enter_tree():
	add_custom_type("AkEvent", "Spatial", preload("res://wwise/nodes/ak_event.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkBank", "Spatial", preload("res://wwise/nodes/ak_bank.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkListener", "Spatial", preload("res://wwise/nodes/ak_listener.gd"), preload("res://icon_small_node.png"))
	add_custom_type("AkState", "Spatial", preload("res://wwise/nodes/ak_state.gd"), preload("res://icon_small_node.png"))

func _exit_tree():
	remove_custom_type("AkEvent")
	remove_custom_type("AkBank")
	remove_custom_type("AkListener")
	remove_custom_type("AkState")

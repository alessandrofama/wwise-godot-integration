tool
extends EditorPlugin

var waapiPickerScene = preload("res://wwise/waapi_picker.tscn")
var waapiPickerControl := Control.new()
var editorViewport = null
var parentWaapiContainer = null

func _ready():
	waapiPickerControl = waapiPickerScene.instance()
	add_control_to_bottom_panel(waapiPickerControl,"Waapi Picker")
	yield(get_tree(), 'idle_frame')
	editorViewport = get_editor_interface().get_editor_viewport()

	waapiPickerControl.rect_min_size.y = 200
	parentWaapiContainer = waapiPickerControl.find_node("ParentVBoxContainer")
	var error = waapiPickerControl.connect("draw", self, "_on_draw_waapiPickerControl")
	assert(error == OK)

func _on_draw_waapiPickerControl():
	var width = editorViewport.rect_size.x - 6
	var height = get_editor_interface().get_base_control().get_size().y - editorViewport.rect_size.y - 150
	parentWaapiContainer.set_size(Vector2(width, height))

func _exit_tree():
	remove_control_from_bottom_panel (waapiPickerControl)
	waapiPickerControl.queue_free()

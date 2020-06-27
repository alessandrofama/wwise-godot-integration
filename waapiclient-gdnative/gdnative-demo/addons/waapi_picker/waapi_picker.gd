tool
extends EditorPlugin

var waapiPickerControl := Control.new()
var editorViewport = null
var parentWaapiContainer = null
var refreshProjectButton = null
var projectObjectsTree = null

func _enter_tree():
	waapiPickerControl = preload("res://addons/waapi_picker/waapi_picker.tscn").instance()
	var buttonResult = add_control_to_bottom_panel(waapiPickerControl,"Waapi Picker")
	assert(buttonResult)
	yield(get_tree(), 'idle_frame')
	editorViewport = get_editor_interface().get_editor_viewport()

	waapiPickerControl.rect_min_size.y = 200
	parentWaapiContainer = waapiPickerControl.find_node("ParentVBoxContainer")
	var error = waapiPickerControl.connect("draw", self, "_on_draw_waapiPickerControl")
	assert(error == OK)
	
	refreshProjectButton = waapiPickerControl.find_node("RefreshProjectButton")
	refreshProjectButton.connect("button_up", self, "_on_refreshProjectButtonClick")
	
	projectObjectsTree = waapiPickerControl.find_node("ProjectObjectsTree")

func _on_draw_waapiPickerControl():
	var width = editorViewport.rect_size.x - 6
	var height = get_editor_interface().get_base_control().get_size().y - editorViewport.rect_size.y - 150
	parentWaapiContainer.set_size(Vector2(width, height))

func _on_refreshProjectButtonClick():
	var connectResult = Waapi.connect_client("127.0.0.1", 8080)

	if connectResult:	
		var args = {"from": {"ofType": ["Event"]}}
		var options = {"return": ["id", "name"]}
	
		var dict = Waapi.client_call("ak.wwise.core.object.get", JSON.print(args), JSON.print(options))
		var jsonDocument = JSON.parse(dict["resultString"])
	
		if jsonDocument.error == OK and jsonDocument.result.has("return"):
			print(jsonDocument.result["return"])
			
			projectObjectsTree.clear()
			var eventsTree = projectObjectsTree.create_item()
			eventsTree.set_text(0, "Events")
			for event in jsonDocument.result["return"]:
				var item = projectObjectsTree.create_item(eventsTree)
				item.set_text(0, event.name)
		
	if Waapi.is_client_connected():
		Waapi.disconnect_client()

func _exit_tree():
	remove_control_from_bottom_panel(waapiPickerControl)
	waapiPickerControl.queue_free()

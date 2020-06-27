tool
extends EditorPlugin

var waapiPickerControl := Control.new()
var editorViewport = null
var parentWaapiContainer = null
var refreshProjectButton = null
var exportSoundbanksButton = null
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
	error = refreshProjectButton.connect("button_up", self, "_on_refreshProjectButtonClick")
	assert(error == OK)
	
	exportSoundbanksButton = waapiPickerControl.find_node("ExportSoundbanksButton")
	error = exportSoundbanksButton.connect("button_up", self, "_on_exportSoundbanksButtonClick")
	assert(error == OK)
	
	projectObjectsTree = waapiPickerControl.find_node("ProjectObjectsTree")

func _on_draw_waapiPickerControl():
	var width = editorViewport.rect_size.x - 6
	var height = get_editor_interface().get_base_control().get_size().y - editorViewport.rect_size.y - 150
	parentWaapiContainer.set_size(Vector2(width, height))

func _on_refreshProjectButtonClick():
	var connectResult = Waapi.connect_client("127.0.0.1", 8080)

	if connectResult:	
		var args = {"from": {"ofType": ["Project", "Event", "Switch", "State", "SoundBank", "AuxBus", "AcousticTexture"]}}
		var options = {"return": ["id", "name", "type"]}
	
		var dict = Waapi.client_call("ak.wwise.core.object.get", JSON.print(args), JSON.print(options))
		var jsonDocument = JSON.parse(dict["resultString"])
	
		if jsonDocument.error == OK and jsonDocument.result.has("return"):
			print(jsonDocument.result["return"])
			
			projectObjectsTree.clear()
			
			var wwiseProjectTree = projectObjectsTree.create_item()
			wwiseProjectTree.set_text(0, "WwiseProject")
			wwiseProjectTree.set_icon(0, preload("res://addons/waapi_picker/icons/wwise_project.png"))
			var eventsTree = projectObjectsTree.create_item()
			eventsTree.set_text(0, "Events")
			eventsTree.set_icon(0, preload("res://addons/waapi_picker/icons/folder.png"))
			var switchesTree = projectObjectsTree.create_item()
			switchesTree.set_text(0, "Switches")
			switchesTree.set_icon(0, preload("res://addons/waapi_picker/icons/folder.png"))
			var statesTree = projectObjectsTree.create_item()
			statesTree.set_text(0, "States")
			statesTree.set_icon(0, preload("res://addons/waapi_picker/icons/folder.png"))
			var soundbanksTree = projectObjectsTree.create_item()
			soundbanksTree.set_text(0, "SoundBanks")
			soundbanksTree.set_icon(0, preload("res://addons/waapi_picker/icons/folder.png"))
			var auxiliaryBusesTree = projectObjectsTree.create_item()
			auxiliaryBusesTree.set_text(0, "Auxiliary Buses")
			auxiliaryBusesTree.set_icon(0, preload("res://addons/waapi_picker/icons/folder.png"))
			var virtualAcousticsTree = projectObjectsTree.create_item()
			virtualAcousticsTree.set_text(0, "Virtual Acoustics")
			virtualAcousticsTree.set_icon(0, preload("res://addons/waapi_picker/icons/folder.png"))
			
			for object in jsonDocument.result["return"]:
				var item = null
				
				if object.type == "Project":
					wwiseProjectTree.set_text(0, object.name)
				elif object.type == "Event":
					item = projectObjectsTree.create_item(eventsTree)
					item.set_icon(0, preload("res://addons/waapi_picker/icons/event.png"))
				elif object.type == "Switch":
					item = projectObjectsTree.create_item(switchesTree)
					item.set_icon(0, preload("res://addons/waapi_picker/icons/switch.png"))
				elif object.type == "State":
					item = projectObjectsTree.create_item(statesTree)
					item.set_icon(0, preload("res://addons/waapi_picker/icons/state.png"))
				elif object.type == "SoundBank":
					item = projectObjectsTree.create_item(soundbanksTree)
					item.set_icon(0, preload("res://addons/waapi_picker/icons/soundbank.png"))
				elif object.type == "AuxBus":
					item = projectObjectsTree.create_item(auxiliaryBusesTree)
					item.set_icon(0, preload("res://addons/waapi_picker/icons/auxbus.png"))
				elif object.type == "AcousticTexture":
					item = projectObjectsTree.create_item(virtualAcousticsTree)
					item.set_icon(0, preload("res://addons/waapi_picker/icons/acoustictexture.png"))
				else:
					assert(false) # Not supported type
				
				if item:
					item.set_text(0, object.name)
		
	if Waapi.is_client_connected():
		Waapi.disconnect_client()
		
func _on_exportSoundbanksButtonClick():
	var connectResult = Waapi.connect_client("127.0.0.1", 8080)

	if connectResult:	
		var args = {"command": "GenerateAllSoundbanksAllPlatforms"}
		var options = {}
	
		var dict = Waapi.client_call("ak.wwise.ui.commands.execute", JSON.print(args), JSON.print(options))
		var jsonDocument = JSON.parse(dict["resultString"])
		var callResult = dict["callResult"]
	
		if jsonDocument.error == OK and callResult:
			print("Generated soundbanks OK")
		else:
			printerr("Error when generated soundbanks with result: " + String(jsonDocument.result))
		
	if Waapi.is_client_connected():
		Waapi.disconnect_client()

func _exit_tree():
	remove_control_from_bottom_panel(waapiPickerControl)
	waapiPickerControl.queue_free()

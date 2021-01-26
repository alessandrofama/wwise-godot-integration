tool
extends EditorPlugin

var waapiPickerControl := Control.new()
var editorViewport = null
var parentWaapiContainer = null
var refreshProjectButton = null
var exportSoundbanksButton = null

var jsonProjectDocument = null
var projectObjectsTree = null
var searchText = null
var isShowingViewport = true

var projectIcon			:ImageTexture
var folderIcon			:ImageTexture
var eventIcon			:ImageTexture
var switchGroupIcon 	:ImageTexture
var switchIcon 			:ImageTexture
var stateGroupIcon 		:ImageTexture
var stateIcon 			:ImageTexture
var soundBankIcon 		:ImageTexture
var busIcon				:ImageTexture
var auxBusIcon 			:ImageTexture
var acousticTextureIcon	:ImageTexture
var workUnitIcon 		:ImageTexture
var searchIcon 			:ImageTexture

var selectedItem = null
var worldPosition:Vector3

var menu:PopupMenu
var target_pos:Vector2
enum PopupItems {
	PlayStop,
	StopAll
}
var playingItems = []

var connectResult
signal connectionChanged(result)

func create_icon(path: String) -> ImageTexture:
	var waapi_image = Image.new()
	var err = waapi_image.load(path)
	if err != OK:
		push_error("Failed load custom nodes icon")
	var waapi_icon = ImageTexture.new()
	waapi_icon.create_from_image(waapi_image, 0)
	return waapi_icon

func _init():
	projectIcon			= create_icon("res://addons/waapi_picker/icons/wwise_project.png")
	folderIcon			= create_icon("res://addons/waapi_picker/icons/folder.png")
	eventIcon			= create_icon("res://addons/waapi_picker/icons/event.png")
	switchGroupIcon 	= create_icon("res://addons/waapi_picker/icons/switchgroup.png")
	switchIcon 			= create_icon("res://addons/waapi_picker/icons/switch.png")
	stateGroupIcon 		= create_icon("res://addons/waapi_picker/icons/stategroup.png")
	stateIcon 			= create_icon("res://addons/waapi_picker/icons/state.png")
	soundBankIcon 		= create_icon("res://addons/waapi_picker/icons/soundbank.png")
	busIcon				= create_icon("res://addons/waapi_picker/icons/bus.png")
	auxBusIcon 			= create_icon("res://addons/waapi_picker/icons/auxbus.png")
	acousticTextureIcon	= create_icon("res://addons/waapi_picker/icons/acoustictexture.png")
	workUnitIcon 		= create_icon("res://addons/waapi_picker/icons/workunit.png")	
	searchIcon 			= create_icon("res://addons/waapi_picker/icons/search.png")	
	
func _enter_tree():
	waapiPickerControl = preload("res://addons/waapi_picker/waapi_picker.tscn").instance()
	var buttonResult = add_control_to_bottom_panel(waapiPickerControl,"Waapi Picker")
	assert(buttonResult)
	
	yield(get_tree(), 'idle_frame')
	editorViewport = get_editor_interface().get_editor_viewport()

	waapiPickerControl.rect_min_size.y = 200
	parentWaapiContainer = waapiPickerControl.find_node("ParentVBoxContainer")
	var error = editorViewport.connect("resized", self, "_on_resized_editorViewport")
	assert(error == OK)
	
	error = editorViewport.connect("visibility_changed", self, "_on_visibility_changed_editorViewport")
	assert(error == OK)
	
	refreshProjectButton = waapiPickerControl.find_node("RefreshProjectButton")
	error = refreshProjectButton.connect("button_up", self, "_on_refreshProjectButtonClick")
	assert(error == OK)
	
	exportSoundbanksButton = waapiPickerControl.find_node("ExportSoundbanksButton")
	error = exportSoundbanksButton.connect("button_up", self, "_on_exportSoundbanksButtonClick")
	assert(error == OK)
	
	projectObjectsTree = waapiPickerControl.find_node("ProjectObjectsTree")
	error = projectObjectsTree.connect("cell_selected", self, "_on_cellSelected")
	assert(error == OK)
	
	searchText = waapiPickerControl.find_node("SearchText")
	error = searchText.connect("text_changed", self, "_on_searchTextChanged")
	assert(error == OK)
	searchText.right_icon = searchIcon
	
	self.connect("connectionChanged", self, "_on_connectionChanged")
	
	connectResult = Waapi.connect_client("127.0.0.1", 8080)
	emit_signal("connectionChanged", connectResult)
	
	setup_menu()
	
	_on_refreshProjectButtonClick()
	
func _on_resized_editorViewport():
	if editorViewport:
		var width = editorViewport.rect_size.x - 6
		var height = get_editor_interface().get_base_control().get_size().y - editorViewport.rect_size.y - 150
	
		parentWaapiContainer.set_size(Vector2(width, height))
	
func _on_visibility_changed_editorViewport():
	var width = editorViewport.rect_size.x - 6
	var height = 0
	
	if isShowingViewport:
		height = get_editor_interface().get_base_control().get_size().y - 110
		isShowingViewport = false
	else:
		height = get_editor_interface().get_base_control().get_size().y - editorViewport.rect_size.y - 150
		isShowingViewport = true	

	parentWaapiContainer.set_size(Vector2(width, height))
	
func _on_refreshProjectButtonClick():
	if !Waapi.is_client_connected():
		connectResult = Waapi.connect_client("127.0.0.1", 8080)
		emit_signal("connectionChanged", connectResult)
		
	if connectResult:	
		var args = {"from": {"ofType": ["Project", "SwitchGroup", "StateGroup", "Bus", "Switch", "State",
										"AuxBus", "Event", "SoundBank", "AcousticTexture", "WorkUnit"]}}
		var options = {"return": ["id", "name", "type", "workunit", "path", "shortId"]}
	
		var clientCallDict = Waapi.client_call("ak.wwise.core.object.get", JSON.print(args), JSON.print(options))
		jsonProjectDocument = JSON.parse(clientCallDict["resultString"])

		if jsonProjectDocument.error == OK and jsonProjectDocument.result.has("return"):
			#print(jsonProjectDocument.result["return"])
			_create_projectObjectsTree("")

func _on_exportSoundbanksButtonClick():
	if !Waapi.is_client_connected():
		var connectResult = Waapi.connect_client("127.0.0.1", 8080)
		emit_signal("connectionChanged", connectResult)

	if connectResult:	
		var args = {"command": "GenerateAllSoundbanksAllPlatformsAutoClose"}
		var options = {}
	
		var dict = Waapi.client_call("ak.wwise.ui.commands.execute", JSON.print(args), JSON.print(options))
		var jsonDocument = JSON.parse(dict["resultString"])
		var callResult = dict["callResult"]
	
		if jsonDocument.error == OK and callResult:
			print("Generated soundbanks OK")
		else:
			printerr("Error when generated soundbanks with result: " + String(jsonDocument.result))
		
func _create_projectObjectsTree(textFilter):
	# Initialise tree
	projectObjectsTree.clear()
	
	# Create root node
	var wwiseProjectRoot = projectObjectsTree.create_item()
	wwiseProjectRoot.set_text(0, "WwiseProject")
	wwiseProjectRoot.set_icon(0, projectIcon)

	# Create folders for each of the parent types (events, switches etc)
	var eventsTree = projectObjectsTree.create_item(wwiseProjectRoot)
	eventsTree.set_text(0, "Events")
	eventsTree.set_icon(0, folderIcon)
	var switchesTree = projectObjectsTree.create_item(wwiseProjectRoot)
	switchesTree.set_text(0, "Switches")
	switchesTree.set_icon(0, folderIcon)
	var statesTree = projectObjectsTree.create_item(wwiseProjectRoot)
	statesTree.set_text(0, "States")
	statesTree.set_icon(0, folderIcon)
	var soundbanksTree = projectObjectsTree.create_item(wwiseProjectRoot)
	soundbanksTree.set_text(0, "SoundBanks")
	soundbanksTree.set_icon(0, folderIcon)
	var auxiliaryBusesTree = projectObjectsTree.create_item(wwiseProjectRoot)
	auxiliaryBusesTree.set_text(0, "Auxiliary Buses")
	auxiliaryBusesTree.set_icon(0, folderIcon)
	var virtualAcousticsTree = projectObjectsTree.create_item(wwiseProjectRoot)
	virtualAcousticsTree.set_text(0, "Virtual Acoustics")
	virtualAcousticsTree.set_icon(0, folderIcon)
	
	# Set project root name
	if jsonProjectDocument == null:
		return
	for object in jsonProjectDocument.result["return"]:
		if object.type == "Project":
			wwiseProjectRoot.set_text(0, object.name)
			break

	# Create work units hierarchy	
	for object in jsonProjectDocument.result["return"]:
		if object.type == "WorkUnit":
			var item = null
			
			if "\\Events\\" in object.path:
				item = projectObjectsTree.create_item(eventsTree)
			elif "\\Switches\\" in object.path:
				item = projectObjectsTree.create_item(switchesTree)
			elif "\\States\\" in object.path:
				item = projectObjectsTree.create_item(statesTree)
			elif "\\SoundBanks\\" in object.path:
				item = projectObjectsTree.create_item(soundbanksTree)
			elif "\\Master-Mixer Hierarchy\\" in object.path:
				item = projectObjectsTree.create_item(auxiliaryBusesTree)
			elif "\\Virtual Acoustics\\" in object.path:
				item = projectObjectsTree.create_item(virtualAcousticsTree)

			if item:			
				item.set_text(0, object.name)
				item.set_icon(0, workUnitIcon)

	# Create switch groups, state groups and buses
	var numSwitchGroups = 0
	var numStateGroups = 0
	var numBusGroups = 0
	
	for object in jsonProjectDocument.result["return"]:
		var item = null
		
		if object.type == "SwitchGroup":
			var workUnit = switchesTree.get_children()
			while workUnit:
				if workUnit.get_text(0) in object.path:
					if textFilter.empty() or textFilter in object.name:
						item = projectObjectsTree.create_item(workUnit)
						item.set_icon(0, switchGroupIcon)
						numSwitchGroups += 1
					break	
				workUnit = workUnit.get_next()
		elif object.type == "StateGroup":
			var workUnit = statesTree.get_children()
			while workUnit:
				if workUnit.get_text(0) in object.path:
					if textFilter.empty() or textFilter in object.name:
						item = projectObjectsTree.create_item(workUnit)
						item.set_icon(0, stateGroupIcon)
						numStateGroups += 1
					break
				workUnit = workUnit.get_next()
		elif object.type == "Bus":
			var workUnit = auxiliaryBusesTree.get_children()
			while workUnit:
				if workUnit.get_text(0) in object.path:
					if textFilter.empty() or textFilter in object.name:
						item = projectObjectsTree.create_item(workUnit)
						item.set_icon(0, busIcon)
						numBusGroups += 1
					break
				workUnit = workUnit.get_next()		
		if item:
			item.set_text(0, object.name)

	# Create child switches, states and aux busses
	# Create events, soundbanks and acoustic textures
	var numSwitches = 0
	var numStates = 0
	var numAuxBux = 0
	var numEvents = 0
	var numSoundBanks = 0
	var numAcousticTextures = 0
	
	for object in jsonProjectDocument.result["return"]:
		var item = null
		
		if object.type == "Switch":
			var workUnit = switchesTree.get_children()
			while workUnit:
				if workUnit.get_text(0) in object.path:
					var switchGroup = workUnit.get_children()
					while switchGroup:
						if switchGroup.get_text(0) in object.path:
							if textFilter.empty() or textFilter in object.name:
								item = projectObjectsTree.create_item(switchGroup)
								item.set_icon(0, switchIcon)
								item.set_meta("Type", "Switch")
								item.set_meta("SwitchGroup", switchGroup.get_text(0))
								item.set_meta("SwitchValue", object.name)
								numSwitches += 1
							break
						switchGroup = switchGroup.get_next()
					break	
				workUnit = workUnit.get_next()
		elif object.type == "State":
			var workUnit = statesTree.get_children()
			while workUnit:
				if workUnit.get_text(0) in object.path:
					var stateGroup = workUnit.get_children()
					while stateGroup:
						if stateGroup.get_text(0) in object.path:
							if textFilter.empty() or textFilter in object.name:
								item = projectObjectsTree.create_item(stateGroup)
								item.set_icon(0, stateIcon)
								item.set_meta("Type", "State")
								item.set_meta("StateGroup", stateGroup.get_text(0))
								item.set_meta("StateValue", object.name)
								numStates += 1
							break
						stateGroup = stateGroup.get_next()
					break	
				workUnit = workUnit.get_next()
		elif object.type == "AuxBus":
			var workUnit = auxiliaryBusesTree.get_children()
			while workUnit:
				if workUnit.get_text(0) in object.path:
					var bus = workUnit.get_children()
					while bus:
						if bus.get_text(0) in object.path:
							if textFilter.empty() or textFilter in object.name:
								item = projectObjectsTree.create_item(bus)
								item.set_icon(0, auxBusIcon)
								numAuxBux += 1
							break
						bus = bus.get_next()
					break	
				workUnit = workUnit.get_next()
		elif object.type == "Event":
			var workUnit = eventsTree.get_children()
			while workUnit:
				if workUnit.get_text(0) in object.path:
					if textFilter.empty() or textFilter in object.name:
						item = projectObjectsTree.create_item(workUnit)
						item.set_icon(0, eventIcon)
						item.set_meta("Type", object.type)
						item.set_meta("ShortId", object.shortId)
						item.set_meta("Id", object.id)
						numEvents += 1
					break	
				workUnit = workUnit.get_next()
		elif object.type == "SoundBank":
			var workUnit = soundbanksTree.get_children()
			while workUnit:
				if workUnit.get_text(0) in object.path:
					if textFilter.empty() or textFilter in object.name:
						item = projectObjectsTree.create_item(workUnit)
						item.set_icon(0, soundBankIcon)
						item.set_meta("Type", object.type)
						item.set_meta("ShortId", object.shortId)
						numSoundBanks += 1
					break	
				workUnit = workUnit.get_next()
		elif object.type == "AcousticTexture":
			var workUnit = virtualAcousticsTree.get_children()
			while workUnit:
				if workUnit.get_text(0) in object.path:
					if textFilter.empty() or textFilter in object.name:
						item = projectObjectsTree.create_item(workUnit)
						item.set_icon(0, acousticTextureIcon)
						numAcousticTextures += 1
					break	
				workUnit = workUnit.get_next()	
		if item:
			item.set_text(0, object.name)
			
	# Delete trees that don't have more than 1 fitered item
	if numSwitches == 0 and numSwitchGroups == 0:
		switchesTree.free()
	if numEvents == 0:
		eventsTree.free()
	if numStates == 0 and numStateGroups == 0:
		statesTree.free()
	if numAuxBux == 0 and numBusGroups == 0:
		auxiliaryBusesTree.free()
	if numAcousticTextures == 0:
		virtualAcousticsTree.free()
	if numSoundBanks == 0:
		soundbanksTree.free()

func _on_searchTextChanged(textFilter):
	_create_projectObjectsTree(textFilter)

func _exit_tree():
	if Waapi.is_client_connected():
		Waapi.disconnect_client()
	remove_control_from_bottom_panel(waapiPickerControl)
	waapiPickerControl.queue_free()

func handles(object):
	return true

func forward_spatial_gui_input(camera, event):
	if event is InputEventMouseMotion:
		var from = camera.project_ray_origin(event.position)
		var end = from + camera.project_ray_normal(event.position) * 3000
		var spaceState = get_editor_interface().get_editor_viewport().get_child(1).get_viewport().world.direct_space_state
		var intersection = spaceState.intersect_ray(from, end)

		if not intersection.empty():
			worldPosition = intersection.position
		else:
			if get_editor_interface().get_edited_scene_root():
				if get_editor_interface().get_edited_scene_root().has_method("get_global_transform"):
					var root = get_editor_interface().get_edited_scene_root().get_global_transform().origin
					from = camera.project_ray_origin(event.position)
					end = from + camera.project_ray_normal(event.position) * root.distance_to(camera.global_transform.origin)
					worldPosition = end
		return false

func _notification(notification):
	if notification == NOTIFICATION_DRAG_END:
		if selectedItem:
			if selectedItem.get_meta_list().size() > 1:
				if selectedItem.get_meta("Type") == "Event":
					var akEvent = load("res://wwise/runtime/nodes/ak_event.gd").new()
					akEvent.name = selectedItem.get_text(0)
					akEvent.event = selectedItem.get_meta("ShortId")
					akEvent.trigger_on = AkUtils.GameEvent.NONE
					if get_editor_interface().get_edited_scene_root() == null:
						print("No root node found. Please add one before trying to add an Event to the tree")
						return
					var root = get_editor_interface().get_edited_scene_root()
					root.add_child(akEvent)
					akEvent.owner = root
					akEvent.global_transform.origin = worldPosition
					selectedItem = null
				elif selectedItem.get_meta("Type") == "Switch":
					var akSwitch = load("res://wwise/runtime/nodes/ak_switch.gd").new()
					akSwitch.name = selectedItem.get_text(0)
					akSwitch.switch_group = selectedItem.get_meta("SwitchGroup")
					akSwitch.switch_value = selectedItem.get_meta("SwitchValue")
					if get_editor_interface().get_edited_scene_root() == null:
						print("No root node found. Please add one before trying to add a Switch node to the tree")
						return
					var root = get_editor_interface().get_edited_scene_root()
					root.add_child(akSwitch)
					akSwitch.owner = root
					selectedItem = null
				elif selectedItem.get_meta("Type") == "State":
					var akState = load("res://wwise/runtime/nodes/ak_state.gd").new()
					akState.name = selectedItem.get_text(0)
					akState.state_group = selectedItem.get_meta("StateGroup")
					akState.state_value = selectedItem.get_meta("StateValue")
					if get_editor_interface().get_edited_scene_root() == null:
						print("No root node found. Please add one before trying to add a State node to the tree")
						return
					var root = get_editor_interface().get_edited_scene_root()
					root.add_child(akState)
					akState.owner = root
					selectedItem = null
				elif selectedItem.get_meta("Type") == "SoundBank":
					var akBank = load("res://wwise/runtime/nodes/ak_bank.gd").new()
					akBank.name = selectedItem.get_text(0)
					akBank.bank = selectedItem.get_meta("ShortId")
					if get_editor_interface().get_edited_scene_root() == null:
						print("No root node found. Please add one before trying to add a Bank node to the tree")
						return
					var root = get_editor_interface().get_edited_scene_root()
					root.add_child(akBank)
					akBank.owner = root
					selectedItem = null
					
func setup_menu():
	menu = PopupMenu.new()
	menu.add_item("Play/Stop                    SPACE", PopupItems.PlayStop)
	menu.add_item("Stop All", PopupItems.StopAll)
	menu.connect("id_pressed", self, "menu_id_pressed")
	waapiPickerControl.add_child(menu)
	
func popup_menu(position):
	target_pos = position
	menu.popup(Rect2(projectObjectsTree.get_global_mouse_position(),Vector2(1,1)))

func _input(event):
	if waapiPickerControl.visible:
		if !is_instance_valid(selectedItem):
			return
		if !Waapi.is_client_connected():
			connectResult = Waapi.connect_client("127.0.0.1", 8080)
			emit_signal("connectionChanged", connectResult)
		if event is InputEventMouseButton:
			if event.button_index == BUTTON_RIGHT && event.pressed:
				if selectedItem.has_meta("Type"):
					if selectedItem.get_meta("Type") == "Event":
						if Rect2(projectObjectsTree.rect_global_position, projectObjectsTree.rect_size).has_point(projectObjectsTree.get_global_mouse_position()):
							popup_menu(waapiPickerControl.get_global_mouse_position())
							return true
		elif event is InputEventKey:
			if event.pressed and event.scancode == KEY_SPACE:
				if selectedItem.has_meta("Type"):
					if selectedItem.get_meta("Type") == "Event":
						if Rect2(projectObjectsTree.rect_global_position, projectObjectsTree.rect_size).has_point(projectObjectsTree.get_global_mouse_position()):
							menu_id_pressed(PopupItems.PlayStop)
			
func menu_id_pressed(id):
	if !is_instance_valid(selectedItem):
		return
	match(id):
		# When the User presses the Play/Stop Popup Button or the Space Key:
		PopupItems.PlayStop:
			if selectedItem.has_meta("Id"):
				var guid = selectedItem.get_meta("Id")
				if Waapi.is_client_connected():
					if !selectedItem.has_meta("Transport"):
						var args = {"object": guid}
						var dict = Waapi.client_call("ak.wwise.core.transport.create", JSON.print(args), JSON.print({}))
						var jsonDocument = JSON.parse(dict["resultString"])
						var callResult = dict["callResult"]
						if callResult:
							selectedItem.set_meta("Transport", jsonDocument.result["transport"])
							
							args = {"transport": jsonDocument.result["transport"]}
							dict = Waapi.client_call("ak.wwise.core.transport.getState", JSON.print(args), JSON.print({}))
							var result = JSON.parse(dict["resultString"])
							if result.result["state"] != "playing":
								args = {"action": "playStop", "transport": jsonDocument.result["transport"]}
								Waapi.client_call("ak.wwise.core.transport.executeAction", JSON.print(args), JSON.print({}))
								playingItems.append(selectedItem)
								
								var transportListDict = Waapi.client_call("ak.wwise.core.transport.getList", JSON.print({}), JSON.print({}))
								var transportListResult = JSON.parse(transportListDict["resultString"])
								
								while transportListResult.result.list.size() > 0:
									transportListDict = Waapi.client_call("ak.wwise.core.transport.getList", JSON.print({}), JSON.print({}))
									transportListResult = JSON.parse(transportListDict["resultString"])
									args = {"transport": jsonDocument.result["transport"]}
									dict = Waapi.client_call("ak.wwise.core.transport.getState", JSON.print(args), JSON.print({}))
									result = JSON.parse(dict["resultString"])
									if !result.result.has("state"):
										break
									if result.result["state"] != "playing":
										break
									if !Waapi.is_client_connected():
										break	
									yield(get_tree(), "idle_frame")
								# Handling One-Shots
								if playingItems.size() > 0:
									args = {"transport": jsonDocument.result["transport"]}
									Waapi.client_call("ak.wwise.core.transport.destroy", JSON.print(args), JSON.print({}))
									selectedItem.set_meta("Transport", null)
									playingItems.remove(playingItems.size() - 1)
						else:
							printerr("Failed to create Transport for selected item.")
					# If the selected item already has a Transport in its meta, it means
					# that it is already playing. Stopping here, destroying the Transport
					# and setting the Transport meta to null for the selected item
					else:
						var args = {"action": "playStop", "transport": selectedItem.get_meta("Transport")}
						Waapi.client_call("ak.wwise.core.transport.executeAction", JSON.print(args), JSON.print({}))
						
						args = {"transport": selectedItem.get_meta("Transport")}
						Waapi.client_call("ak.wwise.core.transport.destroy", JSON.print(args), JSON.print({}))
						
						selectedItem.set_meta("Transport", null)
						if playingItems.size() > 0:
							playingItems.remove(playingItems.size() - 1)
		
		# When the user presses the StopAll button:
		# We first stop all the Transports, then loop and destroy every transport
		# Then we loop through the playingItems array and remove every "Transport" meta
		# Finally clear the array
		PopupItems.StopAll:
			if Waapi.is_client_connected():
				var args = {"action": "stop"}
				Waapi.client_call("ak.wwise.core.transport.executeAction", JSON.print(args), JSON.print({}))
				
				var dict = Waapi.client_call("ak.wwise.core.transport.getList", JSON.print({}), JSON.print({}))
				var result = JSON.parse(dict["resultString"])
				for item in result.result.list:
					args = {"transport": item["transport"]}
					Waapi.client_call("ak.wwise.core.transport.destroy", JSON.print(args), JSON.print({}))
				for playingItem in playingItems:
					playingItem.set_meta("Transport", null)
				playingItems.clear()

func _on_cellSelected():
	selectedItem = projectObjectsTree.get_selected()
	
func _on_connectionChanged(result:bool):
	var connectionLabel:Label = waapiPickerControl.find_node("ConnectionText")
	connectionLabel.text = "•"
	var color = Color.limegreen if result else Color.darkred
	connectionLabel.add_color_override("font_color", color)

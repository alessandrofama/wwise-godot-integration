@tool
extends EditorPlugin

var waapi_picker_control := Control.new()
var editor_viewport = null
var parent_waapi_container = null
var refresh_project_button = null
var export_soundbanks_button = null
var generate_ids_button = null

var json_project_document = null
var project_objects_tree = null
var search_text = null
var is_showing_viewport = true

var icon_project: Texture2D
var icon_folder: Texture2D
var icon_event: Texture2D
var icon_switch_group: Texture2D
var icon_switch: Texture2D
var icon_state_group: Texture2D
var icon_state: Texture2D
var icon_soundbank: Texture2D
var icon_bus: Texture2D
var icon_aux_bus: Texture2D
var icon_acoustic_texture: Texture2D
var icon_work_unit: Texture2D
var icon_search: Texture2D

var selected_item = null
var worldPosition: Vector3

var menu: PopupMenu
var target_pos: Vector2

var connect_result
signal connection_changed(result)

var fileDialog: FileDialog
const WWISE_IDS_GD_FILE = "wwise_ids.gd"
var final_text: String

var json: JSON = JSON.new()

var waapi_client = null


func _init():
	icon_project = load("res://addons/Wwise/editor/images/wwise_project.svg")
	icon_folder = load("res://addons/Wwise/editor/images/folder.svg")
	icon_event = load("res://addons/Wwise/editor/images/event.svg")
	icon_switch_group = load("res://addons/Wwise/editor/images/switchgroup.svg")
	icon_switch = load("res://addons/Wwise/editor/images/switch.svg")
	icon_state_group = load("res://addons/Wwise/editor/images/stategroup.svg")
	icon_state = load("res://addons/Wwise/editor/images/state.svg")
	icon_soundbank = load("res://addons/Wwise/editor/images/soundbank.svg")
	icon_bus = load("res://addons/Wwise/editor/images/bus.svg")
	icon_aux_bus = load("res://addons/Wwise/editor/images/auxbus.svg")
	icon_acoustic_texture = load("res://addons/Wwise/editor/images/acoustictexture.svg")
	icon_work_unit = load("res://addons/Wwise/editor/images/workunit.svg")
	icon_search = load("res://addons/Wwise/editor/images/search.svg")


func _enter_tree():
	waapi_picker_control = (
		load("res://addons/Wwise/editor/waapi_picker/waapi_picker.tscn").instantiate()
	)
	var button = add_control_to_bottom_panel(waapi_picker_control, "WAAPI Picker")
	assert(button)

	await get_tree().process_frame

	editor_viewport = get_editor_interface().get_viewport()
	waapi_picker_control.custom_minimum_size.y = (
		get_editor_interface().get_viewport().get_visible_rect().size.y * 0.3
	)

	parent_waapi_container = waapi_picker_control.get_node("ParentVBoxContainer")
	var error = editor_viewport.connect(
		"size_changed", Callable(self, "_on_resized_editor_viewport")
	)
	assert(error == OK)

	error = editor_viewport.connect(
		"visibility_changed", Callable(self, "_on_visibility_changed_editor_viewport")
	)
	assert(error == OK)

	refresh_project_button = waapi_picker_control.get_node(
		"ParentVBoxContainer/TopVBoxContainer/ButtonsHContainer/RefreshProjectButton"
	)
	error = refresh_project_button.connect(
		"button_up", Callable(self, "_on_refresh_project_button_up")
	)
	assert(error == OK)

	export_soundbanks_button = waapi_picker_control.get_node(
		"ParentVBoxContainer/TopVBoxContainer/ButtonsHContainer/ExportSoundbanksButton"
	)
	error = export_soundbanks_button.connect(
		"button_up", Callable(self, "_on_export_soundbanks_button_up")
	)
	assert(error == OK)

	generate_ids_button = waapi_picker_control.get_node(
		"ParentVBoxContainer/TopVBoxContainer/ButtonsHContainer/GenerateIdsButton"
	)
	error = generate_ids_button.connect("button_up", Callable(self, "_on_generate_ids_button_up"))
	assert(error == OK)

	project_objects_tree = waapi_picker_control.get_node("ParentVBoxContainer/ProjectObjectsTree")
	error = project_objects_tree.connect("cell_selected", Callable(self, "_on_cell_selected"))
	assert(error == OK)

	search_text = waapi_picker_control.get_node("ParentVBoxContainer/TopVBoxContainer/SearchText")
	error = search_text.connect("text_changed", Callable(self, "_on_search_text_changed"))
	assert(error == OK)
	search_text.right_icon = icon_search

	self.connect("connection_changed", Callable(self, "_on_connection_changed"))
	
	waapi_client = Engine.get_singleton(&"Waapi")
	
	if waapi_client:
		connect_result = waapi_client.connect_client("127.0.0.1", 8080)
		emit_signal("connection_changed", connect_result)

	_on_refresh_project_button_up()


func _process(_delta):
	if waapi_client:
		if waapi_client.is_client_connected():
			waapi_client.process_callbacks()


func _exit_tree():
	if waapi_client:
		if waapi_client.is_client_connected():
			waapi_client.disconnect_client()
		if is_instance_valid(waapi_picker_control):
			remove_control_from_bottom_panel(waapi_picker_control)
			waapi_picker_control.queue_free()


func _notification(notification):
	if notification == NOTIFICATION_DRAG_END:
		if selected_item:
			if selected_item.get_meta_list().size() > 1:
				if selected_item.get_meta("type") == "event":
					var ak_event = AkEvent3D.new()
					ak_event.name = selected_item.get_text(0)
					ak_event.event.name = selected_item.get_text(0)
					ak_event.event.id = selected_item.get_meta("short_id")
					ak_event.trigger_on = AkUtils.GAMEEVENT_NONE
					if get_editor_interface().get_edited_scene_root() == null:
						print(
							"No root node found. Please add one before trying to add an Event to the tree"
						)
						return
					var root = get_editor_interface().get_edited_scene_root()
					root.add_child(ak_event)
					ak_event.owner = root
					ak_event.global_transform.origin = worldPosition
					selected_item = null
				elif selected_item.get_meta("type") == "switch":
					var ak_switch = AkSwitch.new()
					ak_switch.name = selected_item.get_text(0)
					ak_switch.switch_group.name = selected_item.get_meta("switch_group_name")
					ak_switch.switch_group.id = selected_item.get_meta("switch_group_id")
					ak_switch.switch_value.name = selected_item.get_text(0)
					ak_switch.switch_value.id = selected_item.get_meta("switch_value_id")
					if get_editor_interface().get_edited_scene_root() == null:
						print(
							"No root node found. Please add one before trying to add a Switch node to the tree"
						)
						return
					var root = get_editor_interface().get_edited_scene_root()
					root.add_child(ak_switch)
					ak_switch.owner = root
					selected_item = null
				elif selected_item.get_meta("type") == "state":
					var ak_state = AkState.new()
					ak_state.name = selected_item.get_text(0)
					ak_state.state_group.name = selected_item.get_meta("state_group_name")
					ak_state.state_group.id = selected_item.get_meta("state_group_id")
					ak_state.state_value.name = selected_item.get_text(0)
					ak_state.state_value.id = selected_item.get_meta("state_value_id")
					if get_editor_interface().get_edited_scene_root() == null:
						print(
							"No root node found. Please add one before trying to add a State node to the tree"
						)
						return
					var root = get_editor_interface().get_edited_scene_root()
					root.add_child(ak_state)
					ak_state.owner = root
					selected_item = null
				elif selected_item.get_meta("type") == "soundbank":
					var ak_bank = AkBank.new()
					ak_bank.name = selected_item.get_text(0)
					ak_bank.bank.name = selected_item.get_text(0)
					ak_bank.bank.id = selected_item.get_meta("short_id")
					if get_editor_interface().get_edited_scene_root() == null:
						print(
							"No root node found. Please add a Node3D node before trying to add a Bank node to the tree"
						)
						return
					var root = get_editor_interface().get_edited_scene_root()
					root.add_child(ak_bank)
					ak_bank.owner = root
					selected_item = null


func _forward_3d_gui_input(viewport_camera: Camera3D, event: InputEvent):
	if event is InputEventMouseMotion:
		var from = viewport_camera.project_ray_origin(event.position)
		var end = from + viewport_camera.project_ray_normal(event.position) * 1000
		var spaceState = (
			get_editor_interface()
			. get_editor_viewport()
			. get_child(1)
			. get_viewport()
			. world
			. direct_space_state
		)
		var intersection = spaceState.intersect_ray(from, end, [self])

		if not intersection.is_empty():
			worldPosition = intersection.position
		else:
			if get_editor_interface().get_edited_scene_root():
				if get_editor_interface().get_edited_scene_root().get_class() == "Node3D":
					if get_editor_interface().get_edited_scene_root().has_method(
						"get_global_transform"
					):
						var root = (
							get_editor_interface()
							. get_edited_scene_root()
							. get_global_transform()
							. origin
						)
						from = viewport_camera.project_ray_origin(event.position)
						end = (
							from
							+ (
								viewport_camera.project_ray_normal(event.position)
								* root.distance_to(viewport_camera.global_transform.origin)
							)
						)
						worldPosition = end
			return false


func _create_project_objects_tree(text_filter):
	# Initialize tree
	project_objects_tree.clear()

	# Create root node
	var wwise_project_root = project_objects_tree.create_item()
	wwise_project_root.set_text(0, "WwiseProject")
	wwise_project_root.set_icon(0, icon_project)

	# Create folders for each of the parent types (events, switches etc)
	var tree_events = project_objects_tree.create_item(wwise_project_root)
	tree_events.set_text(0, "Events")
	tree_events.set_icon(0, icon_folder)
	var tree_switches = project_objects_tree.create_item(wwise_project_root)
	tree_switches.set_text(0, "Switches")
	tree_switches.set_icon(0, icon_folder)
	var tree_states = project_objects_tree.create_item(wwise_project_root)
	tree_states.set_text(0, "States")
	tree_states.set_icon(0, icon_folder)
	var tree_soundbanks = project_objects_tree.create_item(wwise_project_root)
	tree_soundbanks.set_text(0, "SoundBanks")
	tree_soundbanks.set_icon(0, icon_folder)
	var tree_aux_busses = project_objects_tree.create_item(wwise_project_root)
	tree_aux_busses.set_text(0, "Auxiliary Buses")
	tree_aux_busses.set_icon(0, icon_folder)
	var tree_virtual_acoustics = project_objects_tree.create_item(wwise_project_root)
	tree_virtual_acoustics.set_text(0, "Virtual Acoustics")
	tree_virtual_acoustics.set_icon(0, icon_folder)

	# Set project root name
	if json_project_document == null:
		return
	for object in json.data["return"]:
		if object.type == "Project":
			wwise_project_root.set_text(0, object.name)
			break

	# Create work units hierarchy
	for object in json.data["return"]:
		if object.type == "WorkUnit":
			var item = null

			if "\\Events\\" in object.path:
				item = project_objects_tree.create_item(tree_events)
			elif "\\Switches\\" in object.path:
				item = project_objects_tree.create_item(tree_switches)
			elif "\\States\\" in object.path:
				item = project_objects_tree.create_item(tree_states)
			elif "\\SoundBanks\\" in object.path:
				item = project_objects_tree.create_item(tree_soundbanks)
			elif "\\Master-Mixer Hierarchy\\" in object.path:
				item = project_objects_tree.create_item(tree_aux_busses)
			elif "\\Virtual Acoustics\\" in object.path:
				item = project_objects_tree.create_item(tree_virtual_acoustics)

			if item:
				item.set_text(0, object.name)
				item.set_icon(0, icon_work_unit)

	# Create switch groups, state groups and buses
	var num_switch_groups = 0
	var num_states_groups = 0
	var num_bus_groups = 0

	for object in json.data["return"]:
		var item = null

		if object.type == "SwitchGroup":
			var work_unit = tree_switches.get_first_child()
			while work_unit:
				if work_unit.get_text(0) in object.path:
					if text_filter.is_empty() or text_filter in object.name:
						item = project_objects_tree.create_item(work_unit)
						item.set_meta("short_id", int(object.shortId))
						item.set_icon(0, icon_switch_group)
						num_switch_groups += 1
					break
				work_unit = work_unit.get_next()
		elif object.type == "StateGroup":
			var work_unit = tree_states.get_first_child()
			while work_unit:
				if work_unit.get_text(0) in object.path:
					if text_filter.is_empty() or text_filter in object.name:
						item = project_objects_tree.create_item(work_unit)
						item.set_meta("short_id", int(object.shortId))
						item.set_icon(0, icon_state_group)
						num_states_groups += 1
					break
				work_unit = work_unit.get_next()
		elif object.type == "Bus":
			var work_unit = tree_aux_busses.get_first_child()
			while work_unit:
				if work_unit.get_text(0) in object.path:
					if text_filter.is_empty() or text_filter in object.name:
						item = project_objects_tree.create_item(work_unit)
						item.set_icon(0, icon_bus)
						num_bus_groups += 1
					break
				work_unit = work_unit.get_next()
		if item:
			item.set_text(0, object.name)

	# Create child switches, states and aux busses
	# Create events, soundbanks and acoustic textures
	var num_switches = 0
	var num_states = 0
	var num_aux_busses = 0
	var num_events = 0
	var num_soundbanks = 0
	var num_acoustic_textures = 0

	for object in json.data["return"]:
		var item = null

		if object.type == "Switch":
			var work_unit = tree_switches.get_first_child()
			while work_unit:
				if work_unit.get_text(0) in object.path:
					var switchGroup = work_unit.get_first_child()
					while switchGroup:
						if switchGroup.get_text(0) in object.path:
							if text_filter.is_empty() or text_filter in object.name:
								item = project_objects_tree.create_item(switchGroup)
								item.set_icon(0, icon_switch)
								item.set_meta("type", "switch")
								item.set_meta("switch_group_id", switchGroup.get_meta("short_id"))
								item.set_meta("switch_group_name", switchGroup.get_text(0))
								item.set_meta("switch_value_id", int(object.shortId))
								num_switches += 1
							break
						switchGroup = switchGroup.get_next()
					break
				work_unit = work_unit.get_next()
		elif object.type == "State":
			var work_unit = tree_states.get_first_child()
			while work_unit:
				if work_unit.get_text(0) in object.path:
					var stateGroup = work_unit.get_first_child()
					while stateGroup:
						if stateGroup.get_text(0) in object.path:
							if text_filter.is_empty() or text_filter in object.name:
								item = project_objects_tree.create_item(stateGroup)
								item.set_icon(0, icon_state)
								item.set_meta("type", "state")
								item.set_meta("state_group_id", stateGroup.get_meta("short_id"))
								item.set_meta("state_group_name", stateGroup.get_text(0))
								item.set_meta("state_value_id", int(object.shortId))
								num_states += 1
							break
						stateGroup = stateGroup.get_next()
					break
				work_unit = work_unit.get_next()
		elif object.type == "AuxBus":
			var work_unit = tree_aux_busses.get_first_child()
			while work_unit:
				if work_unit.get_text(0) in object.path:
					var bus = work_unit.get_first_child()
					while bus:
						if bus.get_text(0) in object.path:
							if text_filter.is_empty() or text_filter in object.name:
								item = project_objects_tree.create_item(bus)
								item.set_icon(0, icon_aux_bus)
								num_aux_busses += 1
							break
						bus = bus.get_next()
					break
				work_unit = work_unit.get_next()
		elif object.type == "Event":
			var work_unit = tree_events.get_first_child()
			while work_unit:
				if work_unit.get_text(0) in object.path:
					if text_filter.is_empty() or text_filter in object.name:
						item = project_objects_tree.create_item(work_unit)
						item.set_icon(0, icon_event)
						item.set_meta("type", "event")
						item.set_meta("short_id", int(object.shortId))
						item.set_meta("id", object.id)
						num_events += 1
					break
				work_unit = work_unit.get_next()
		elif object.type == "SoundBank":
			var work_unit = tree_soundbanks.get_first_child()
			while work_unit:
				if work_unit.get_text(0) in object.path:
					if text_filter.is_empty() or text_filter in object.name:
						item = project_objects_tree.create_item(work_unit)
						item.set_icon(0, icon_soundbank)
						item.set_meta("type", "soundbank")
						item.set_meta("short_id", int(object.shortId))
						num_soundbanks += 1
					break
				work_unit = work_unit.get_next()
		elif object.type == "AcousticTexture":
			var work_unit = tree_virtual_acoustics.get_first_child()
			while work_unit:
				if work_unit.get_text(0) in object.path:
					if text_filter.is_empty() or text_filter in object.name:
						item = project_objects_tree.create_item(work_unit)
						item.set_icon(0, icon_acoustic_texture)
						num_acoustic_textures += 1
					break
				work_unit = work_unit.get_next()
		if item:
			item.set_text(0, object.name)

	# Delete trees that don't have more than 1 fitered item
	if num_switches == 0 and num_switch_groups == 0:
		tree_switches.free()
	if num_events == 0:
		tree_events.free()
	if num_states == 0 and num_states_groups == 0:
		tree_states.free()
	if num_aux_busses == 0 and num_bus_groups == 0:
		tree_aux_busses.free()
	if num_acoustic_textures == 0:
		tree_virtual_acoustics.free()
	if num_soundbanks == 0:
		tree_soundbanks.free()


func _on_resized_editor_viewport():
	if editor_viewport:
		if waapi_picker_control:
			if waapi_picker_control.get_parent():
				var width = waapi_picker_control.get_parent().size.x - 10
				var height = waapi_picker_control.get_parent().size.y - 80
				parent_waapi_container.size.x = width
				parent_waapi_container.size.y = height


func _on_visibility_changed_editor_viewport():
	var width = editor_viewport.rect_size.x - 6
	var height = 0

	if is_showing_viewport:
		height = get_editor_interface().get_base_control().get_size().y - 110
		is_showing_viewport = false
	else:
		height = (
			get_editor_interface().get_base_control().get_size().y
			- editor_viewport.rect_size.y
			- 150
		)
		is_showing_viewport = true

	parent_waapi_container.set_size(Vector2(width, height))


func _on_refresh_project_button_up():
	if !waapi_client:
		return
		
	if !waapi_client.is_client_connected():
		connect_result = waapi_client.connect_client("127.0.0.1", 8080)
		emit_signal("connection_changed", connect_result)

	if connect_result:
		var args = {
			"from":
			{
				"ofType":
				[
					"Project",
					"SwitchGroup",
					"StateGroup",
					"Bus",
					"Switch",
					"State",
					"AuxBus",
					"Event",
					"SoundBank",
					"AcousticTexture",
					"WorkUnit"
				]
			}
		}
		var options = {"return": ["id", "name", "type", "workunit", "path", "shortId"]}

		var clientCallDict = waapi_client.client_call(
			"ak.wwise.core.object.get", JSON.stringify(args), JSON.stringify(options)
		)
		json_project_document = json.parse(clientCallDict["result_string"])

		if json_project_document == OK:
			if json.data.has("return"):
				_create_project_objects_tree("")


func _on_export_soundbanks_button_up():
	if !waapi_client.is_client_connected():
		var connect_result = waapi_client.connect_client("127.0.0.1", 8080)
		emit_signal("connection_changed", connect_result)

	if connect_result:
		var args = {"command": "GenerateAllSoundbanksAllPlatformsAutoClose"}
		var options = {}

		var dict = waapi_client.client_call(
			"ak.wwise.ui.commands.execute", JSON.stringify(args), JSON.stringify(options)
		)
		var json_document = json.parse(dict["result_string"])
		var call_result = dict["call_result"]

		if json_document == OK and call_result:
			print("Generated soundbanks OK")
		else:
			printerr("Error when generated soundbanks with result: " + String(json_document.result))


func _on_search_text_changed(text_filter):
	_create_project_objects_tree(text_filter)


func _on_cell_selected():
	selected_item = project_objects_tree.get_selected()


func _on_connection_changed(result: bool):
	var connectionLabel: Label = waapi_picker_control.get_node(
		"ParentVBoxContainer/TopVBoxContainer/ButtonsHContainer/ConnectionText"
	)
	connectionLabel.text = "•"
	var color = Color.LIME_GREEN if result else Color.DARK_RED
	connectionLabel.add_theme_color_override("font_color", color)


func _on_generate_ids_button_up():
	fileDialog = FileDialog.new()
	fileDialog.file_mode = FileDialog.FILE_MODE_SAVE_FILE
	fileDialog.access = FileDialog.ACCESS_RESOURCES
	fileDialog.filters = PackedStringArray(["*.gd ; GDScript Files"])

	fileDialog.connect("file_selected", Callable(self, "_on_FileDialog_file_selected"))

	var editor_interface = get_editor_interface()
	var base_control = editor_interface.get_base_control()
	base_control.add_child(fileDialog)
	fileDialog.popup_centered_ratio()


func _on_FileDialog_file_selected(path):
	var connect_result = false
	if !waapi_client.is_client_connected():
		connect_result = waapi_client.connect_client("127.0.0.1", 8080)
	else:
		connect_result = true

	if connect_result:
		var args = {
			"from":
			{
				"ofType":
				[
					"Event",
					"StateGroup",
					"State",
					"SwitchGroup",
					"Switch",
					"GameParameter",
					"Trigger",
					"SoundBank",
					"Bus",
					"AuxBus",
					"AudioDevice",
					"ExternalSource"
				]
			}
		}
		var options = {"return": ["name", "type", "shortId", "parent.name"]}
		var dict = waapi_client.client_call(
			"ak.wwise.core.object.get", JSON.stringify(args), JSON.stringify(options)
		)
		var json_document = json.parse(dict["result_string"])

		if json_document == OK:
			var arr = json.data["return"]
			generate_ids(arr)
	else:
		print("Failed to generate Wwise IDs. Wwise authoring isn't launched!")
		if waapi_client.is_client_connected():
			waapi_client.disconnect_client()
		return

	var wwiseIDsGDFile: FileAccess = FileAccess.open(path, FileAccess.WRITE)
	wwiseIDsGDFile.store_string(final_text)
	wwiseIDsGDFile.close()
	print("Generated IDs at {path}".format({"path": path}))
	fileDialog.queue_free()
	final_text = ""

	var filesystem = get_editor_interface().get_resource_filesystem()
	filesystem.update_file(path)

	if waapi_client.is_client_connected():
		waapi_client.disconnect_client()


func generate_ids(arr):
	var event_arr: Array = []
	var state_group_arr: Array = []
	var state_arr: Array = []
	var switch_group_arr: Array = []
	var switch_arr: Array = []
	var game_param_arr: Array = []
	var trigger_arr: Array = []
	var soundbank_arr: Array = []
	var bus_arr: Array = []
	var aux_bus_arr: Array = []
	var audio_device_arr: Array = []
	var external_src_arr: Array = []

	for i in arr.size():
		match arr[i].type:
			"Event":
				event_arr.push_back(arr[i])
			"StateGroup":
				state_group_arr.push_back(arr[i])
			"State":
				state_arr.push_back(arr[i])
			"SwitchGroup":
				switch_group_arr.push_back(arr[i])
			"Switch":
				switch_arr.push_back(arr[i])
			"GameParameter":
				game_param_arr.push_back(arr[i])
			"Trigger":
				trigger_arr.push_back(arr[i])
			"SoundBank":
				soundbank_arr.push_back(arr[i])
			"Bus":
				bus_arr.push_back(arr[i])
			"AuxBus":
				aux_bus_arr.push_back(arr[i])
			"AudioDevice":
				audio_device_arr.push_back(arr[i])
			"ExternalSource":
				external_src_arr.push_back(arr[i])

	soundbank_arr.push_front({"name": "Init", "shortId": 1355168291, "type": "SoundBank"})

	final_text = "class_name AK"
	final_text += "\n\n"

	create_class(event_arr, "EVENTS")
	create_state_switch_class(state_group_arr, state_arr, "STATES", "STATE")
	create_state_switch_class(switch_group_arr, switch_arr, "SWITCHES", "SWITCH")
	create_class(game_param_arr, "GAME_PARAMETERS")
	create_class(trigger_arr, "TRIGGERS")
	create_class(soundbank_arr, "BANKS")
	create_class(bus_arr, "BUSSES")
	create_class(aux_bus_arr, "AUX_BUSSES")
	create_class(audio_device_arr, "AUDIO_DEVICES")
	create_class(external_src_arr, "EXTERNAL_SOURCES")


func create_class(arr: Array, type: String):
	if arr.size() > 0:
		final_text += "class {type}:".format({"type": type})
		final_text += "\n\n"

		for i in arr.size():
			final_text += "\tconst {str} = {id} \n".format(
				{"str": arr[i].name.to_upper().replace(" ", "_"), "id": arr[i].shortId}
			)

		final_text += "\n\tconst _dict = { \n"

		for i in arr.size():
			if i < arr.size() - 1:
				final_text += '\t "{str}": {strId},\n'.format(
					{"str": arr[i].name, "strId": arr[i].name.to_upper().replace(" ", "_")}
				)
			else:
				final_text += '\t "{str}": {strId}\n'.format(
					{"str": arr[i].name, "strId": arr[i].name.to_upper().replace(" ", "_")}
				)
		final_text += "\t} \n\n"
	else:
		create_empty_class(type)


func create_state_switch_class(
	parent_arr: Array, child_arr: Array, parent_type: String, child_type: String
):
	if parent_arr.size() > 0:
		final_text += "class {type}:".format({"type": parent_type})
		final_text += "\n\n"
		for i in parent_arr.size():
			final_text += "\tclass {parent_name}:".format(
				{"parent_name": parent_arr[i].name.to_upper().replace(" ", "_")}
			)
			final_text += "\n"
			final_text += "\t\tconst {str} = {id} \n\n".format(
				{"str": "GROUP", "id": parent_arr[i].shortId}
			)
			final_text += "\t\tclass {type}:".format({"type": child_type})
			final_text += "\n"
			for j in child_arr.size():
				if child_arr[j].get("parent.name") == parent_arr[i].name:
					final_text += "\t\t\tconst {str} = {id} \n".format(
						{
							"str": child_arr[j].name.to_upper().replace(" ", "_"),
							"id": child_arr[j].shortId
						}
					)
			final_text += "\n"

		final_text += "\tconst _dict = { \n"

		for i in parent_arr.size():
			final_text += (
				'\t\t"{groupName}": {\n\t\t\t"GROUP": {groupID},\n\t\t\t"{type}": {\n'
				. format(
					{
						"groupName": parent_arr[i].name,
						"groupID": parent_arr[i].shortId,
						"type": child_type
					}
				)
			)
			for j in child_arr.size():
				if child_arr[j].get("parent.name") == parent_arr[i].name:
					if j < child_arr.size() - 1:
						final_text += '\t\t\t\t"{childName}": {childId},\n'.format(
							{"childName": child_arr[j].name, "childId": child_arr[j].shortId}
						)
					else:
						final_text += '\t\t\t\t"{childName}": {childId}\n'.format(
							{"childName": child_arr[j].name, "childId": child_arr[j].shortId}
						)
			final_text += "\t\t\t} \n"
			if i < parent_arr.size() - 1:
				final_text += "\t\t}, \n"
			else:
				final_text += "\t\t} \n"
		final_text += "\t} \n\n"
	else:
		create_empty_class(parent_type)


func create_empty_class(type: String):
	final_text += "class {type}:".format({"type": type})
	final_text += "\n\n"
	final_text += "\tconst _dict = {} \n\n"

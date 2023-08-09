extends EditorProperty

var property_control: Button = Button.new()
var icon: Texture2D
var current_value: Dictionary = {"name": "", "id": 0}
var updating = false
var inspector_browser: BoxContainer = (
	preload("res://addons/Wwise/editor/inspector_browser/inspector_browser.tscn")
	. instantiate(PackedScene.GEN_EDIT_STATE_INSTANCE)
)
var selected_item: TreeItem
var type: int = AkUtils.AKTYPE_EVENT
var user_data: Dictionary


func _init(wwise_type, _user_data: Dictionary = {}):
	type = wwise_type
	user_data = _user_data

	property_control.clip_text = true
	add_child(property_control)

	inspector_browser.get_node("ProjectObjectsTree").type = type
	inspector_browser.get_node("ProjectObjectsTree").user_data = user_data
	inspector_browser.get_node("ProjectObjectsTree").initialize()

	match type:
		AkUtils.AKTYPE_EVENT:
			property_control.text = "Select Event..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search Events..."
			icon = load("res://addons/Wwise/editor/images/event.svg")
		AkUtils.AKTYPE_BANK:
			property_control.text = "Select Bank..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search Banks..."
			icon = load("res://addons/Wwise/editor/images/soundbank.svg")
		AkUtils.AKTYPE_STATE_GROUP:
			property_control.text = "Select State Group..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search State Groups..."
			icon = load("res://addons/Wwise/editor/images/stategroup.svg")
		AkUtils.AKTYPE_STATE:
			property_control.text = "Select State..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search State..."
			icon = load("res://addons/Wwise/editor/images/state.svg")
		AkUtils.AKTYPE_SWITCH_GROUP:
			property_control.text = "Select Switch Group..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search Switch Groups..."
			icon = load("res://addons/Wwise/editor/images/switchgroup.svg")
		AkUtils.AKTYPE_SWITCH:
			property_control.text = "Select Switch..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search Switches..."
			icon = load("res://addons/Wwise/editor/images/switch.svg")
		AkUtils.AKTYPE_AUX_BUS:
			property_control.text = "Select Aux Bus..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search Aux Busses..."
			icon = load("res://addons/Wwise/editor/images/auxbus.svg")
		AkUtils.AKTYPE_RTPC:
			property_control.text = "Select Game Parameter..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search Game Parameters..."
			icon = load("res://addons/Wwise/editor/images/rtpc.svg")

	var tree = inspector_browser.get_node("ProjectObjectsTree")
	tree.connect("item_selected", Callable(self, "_on_item_selected"))
	property_control.connect("pressed", Callable(self, "_on_button_pressed"))


func _on_button_pressed():
	if updating:
		return

	remove_child(property_control)
	await get_tree().process_frame

	inspector_browser.size = Vector2(0.0, 0.0)
	inspector_browser.custom_minimum_size = Vector2(property_control.size.x, 600)

	add_child(inspector_browser)


func _on_item_selected():
	var tree: Tree = inspector_browser.get_node("ProjectObjectsTree")
	selected_item = tree.get_selected()
	if selected_item:
		current_value = {"name": selected_item.get_meta("name"), "id": selected_item.get_meta("id")}
		property_control.text = str(current_value.get("name"))
		property_control.icon = icon

		if get_edited_object()[get_edited_property()].get("id") != current_value.get("id"):
			if type == AkUtils.AKTYPE_STATE_GROUP:
				emit_changed("state_value", {"name": "", "id": 0})
			if type == AkUtils.AKTYPE_SWITCH_GROUP:
				emit_changed("switch_value", {"name": "", "id": 0})

		emit_changed(get_edited_property(), current_value)


func _update_property():
	var new_value = get_edited_object()[get_edited_property()]

	if new_value is String:
		return
	elif new_value == null:
		new_value = {"name": "", "id": 0}
	elif new_value.is_empty():
		new_value = {"name": "", "id": 0}

	updating = true
	current_value = new_value
	if new_value.get("name") == null:
		return

	if !new_value.get("name").is_empty():
		property_control.text = str(current_value.get("name"))
		property_control.icon = icon
	else:
		match type:
			AkUtils.AKTYPE_EVENT:
				property_control.text = "Select Event..."
			AkUtils.AKTYPE_BANK:
				property_control.text = "Select Bank..."
			AkUtils.AKTYPE_STATE_GROUP:
				property_control.text = "Select State Group..."
			AkUtils.AKTYPE_STATE:
				property_control.text = "Select State..."
			AkUtils.AKTYPE_SWITCH_GROUP:
				property_control.text = "Select Switch Group..."
			AkUtils.AKTYPE_SWITCH:
				property_control.text = "Select Switch..."
			AkUtils.AKTYPE_AUX_BUS:
				property_control.text = "Select Aux Bus..."
			AkUtils.AKTYPE_RTPC:
				property_control.text = "Select Game Parameter..."
		property_control.icon = null

	if self.get_child(0).get_class() == "VBoxContainer":
		remove_child(inspector_browser)
		add_child(property_control)

	match type:
		AkUtils.AKTYPE_STATE:
			inspector_browser.get_node("ProjectObjectsTree").user_data = (
				get_edited_object().state_group
			)
		AkUtils.AKTYPE_SWITCH:
			inspector_browser.get_node("ProjectObjectsTree").user_data = (
				get_edited_object().switch_group
			)

	inspector_browser.get_node("ProjectObjectsTree").populate_browser("")

	inspector_browser.get_node("TopVBoxContainer/SearchText").text = ""
	updating = false

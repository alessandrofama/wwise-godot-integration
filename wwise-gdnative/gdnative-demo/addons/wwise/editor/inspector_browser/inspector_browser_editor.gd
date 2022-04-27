extends EditorProperty

var property_control:Button = Button.new()
var icon:ImageTexture
var current_value:Dictionary = {"Name": "", "Id": 0}
var updating = false
var inspector_browser:BoxContainer = load("res://addons/wwise/editor/inspector_browser/inspector_browser.tscn").instance(PackedScene.GEN_EDIT_STATE_DISABLED)
var selected_item: TreeItem
var type:int = AkUtils.AkType.EVENT
var user_data:Dictionary

func _init(wwise_type, _user_data:Dictionary = {}):
	type = wwise_type
	user_data = _user_data
	
	property_control.clip_text = true
	add_child(property_control)

	inspector_browser.get_node("ProjectObjectsTree").type = type
	inspector_browser.get_node("ProjectObjectsTree").user_data = user_data
	inspector_browser.get_node("ProjectObjectsTree").initialize()

	match(type):
		AkUtils.AkType.EVENT:
			property_control.text = "Select Event..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search Events..."
			icon	 = AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/event.png")
		AkUtils.AkType.BANK:
			property_control.text = "Select Bank..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search Banks..."
			icon	 = AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/soundbank.png")
		AkUtils.AkType.STATE_GROUP:
			property_control.text = "Select State Group..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search State Groups..."
			icon	 = AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/stategroup.png")
		AkUtils.AkType.STATE:
			property_control.text = "Select State..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search State..."
			icon	 = AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/state.png")
		AkUtils.AkType.SWITCH_GROUP:
			property_control.text = "Select Switch Group..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search Switch Groups..."
			icon	 = AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/switchgroup.png")
		AkUtils.AkType.SWITCH:
			property_control.text = "Select Switch..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search Switches..."
			icon	 = AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/switch.png")
		AkUtils.AkType.AUX_BUS:
			property_control.text = "Select Aux Bus..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search Aux Busses..."
			icon	 = AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/auxbus.png")
		AkUtils.AkType.RTPC:
			property_control.text = "Select Game Parameter..."
			inspector_browser.get_node("TopVBoxContainer/SearchText").placeholder_text = "Search Game Parameters..."
			icon	 = AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/rtpc.png")	
			
	var tree = inspector_browser.get_node("ProjectObjectsTree")
	tree.connect("item_selected", self, "_on_item_selected")
	property_control.connect("pressed", self, "_on_button_pressed")
	
func _on_button_pressed():
	if (updating):
		return
	
	remove_child(property_control)
	yield(get_tree(),"idle_frame")

	inspector_browser.rect_size = Vector2(0.0, 0.0)
	inspector_browser.rect_min_size = Vector2(property_control.rect_size.x, 600)
	
	add_child(inspector_browser)

func _on_item_selected():
	var tree:Tree = inspector_browser.get_node("ProjectObjectsTree")
	selected_item = tree.get_selected()
	if selected_item:
		current_value = {"Name": selected_item.get_meta("Name"), "Id": selected_item.get_meta("Id")}
		property_control.text = str(current_value.get("Name"))
		property_control.icon = icon
		
		if get_edited_object()[get_edited_property()].get("Id") != current_value.get("Id"):
			if type == AkUtils.AkType.STATE_GROUP:
				emit_changed("state_value", {"Name": "", "Id": 0})
			if type == AkUtils.AkType.SWITCH_GROUP:
				emit_changed("switch_value", {"Name": "", "Id": 0})
		
		emit_changed(get_edited_property(), current_value)

func update_property():
	var new_value = get_edited_object()[get_edited_property()]
	
	if new_value is String:
		return
	elif new_value == null:
		new_value = {"Name": "", "Id": 0}
	elif new_value.empty():
		new_value = {"Name": "", "Id": 0}	
		
	updating = true
	current_value = new_value
	if new_value.get("Name") == null:
		return
		
	if !new_value.get("Name").empty():
		property_control.text = str(current_value.get("Name"))
		property_control.icon = icon
	else:
		match(type):
			AkUtils.AkType.EVENT:
				property_control.text = "Select Event..."
			AkUtils.AkType.BANK:
				property_control.text = "Select Bank..."
			AkUtils.AkType.STATE_GROUP:
				property_control.text = "Select State Group..."
			AkUtils.AkType.STATE:
				property_control.text = "Select State..."
			AkUtils.AkType.SWITCH_GROUP:
				property_control.text = "Select Switch Group..."
			AkUtils.AkType.SWITCH:
				property_control.text = "Select Switch..."
			AkUtils.AkType.AUX_BUS:
				property_control.text = "Select Aux Bus..."
			AkUtils.AkType.RTPC:
				property_control.text = "Select Game Parameter..."
		property_control.icon = null
	
	if self.get_child(0).get_class() == "VBoxContainer":
		remove_child(inspector_browser)
		add_child(property_control)
	
	match (type):	
		AkUtils.AkType.STATE:
			inspector_browser.get_node("ProjectObjectsTree").user_data = get_edited_object().state_group
		AkUtils.AkType.SWITCH:
			inspector_browser.get_node("ProjectObjectsTree").user_data = get_edited_object().switch_group
	
	inspector_browser.get_node("ProjectObjectsTree").populate_browser("")

	inspector_browser.get_node("TopVBoxContainer/SearchText").text = ""
	updating = false

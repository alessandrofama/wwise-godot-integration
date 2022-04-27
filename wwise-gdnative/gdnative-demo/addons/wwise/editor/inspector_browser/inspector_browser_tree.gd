tool
extends Tree

var selected_item:TreeItem = null
var root:TreeItem = null
var icon	:ImageTexture
var search_text:LineEdit
var type:int = AkUtils.AkType.EVENT
var user_data:Dictionary

func initialize():
	populate_browser("")
	search_text = get_parent().get_node("TopVBoxContainer/SearchText")
	search_text.connect("text_changed", self, "_on_text_changed")

func populate_browser(text_filter:String):
	if root:
		self.clear()
	root = self.create_item()
	self.hide_root = true
	root.set_text(0, "Root")
	
	var dict
	match(type):
		AkUtils.AkType.EVENT:
			dict = AK.EVENTS._dict
		AkUtils.AkType.BANK:
			dict = AK.BANKS._dict
		AkUtils.AkType.STATE_GROUP:
			dict = AK.STATES._dict
		AkUtils.AkType.STATE:
			if !user_data.empty():
				if AK.STATES._dict.get(user_data.get("Name")):
					dict = AK.STATES._dict.get(user_data.get("Name")).get("STATE")
				else:
					dict = {}	
		AkUtils.AkType.SWITCH_GROUP:
			dict = AK.SWITCHES._dict
		AkUtils.AkType.SWITCH:
			if !user_data.empty():
				if AK.SWITCHES._dict.get(user_data.get("Name")):
					dict = AK.SWITCHES._dict.get(user_data.get("Name")).get("SWITCH")
				else:
					dict = {}	
		AkUtils.AkType.AUX_BUS:
			dict = AK.AUX_BUSSES._dict
		AkUtils.AkType.RTPC:
			dict = AK.GAME_PARAMETERS._dict
					
	for i in dict.size():
		if !dict.keys()[i].findn(text_filter) != -1 && !text_filter.empty():
			continue
		
		var item = self.create_item(root)
		
		if type == AkUtils.AkType.EVENT:
			icon	= AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/event.png")
			item.set_text(0, dict.keys()[i])
			item.set_meta("Name", dict.keys()[i])
			item.set_meta("Id", dict.values()[i])	
		if type == AkUtils.AkType.BANK:
			icon	= AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/soundbank.png")
			item.set_text(0, dict.keys()[i])
			item.set_meta("Name", dict.keys()[i])
			item.set_meta("Id", dict.values()[i])	
		if type == AkUtils.AkType.STATE_GROUP:
			icon	= AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/stategroup.png")
			item.set_text(0, dict.keys()[i])
			item.set_meta("Name", dict.keys()[i])
			item.set_meta("Id", dict.values()[i].get("GROUP"))
		if type == AkUtils.AkType.STATE:
			if !user_data:
				return
			icon	= AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/state.png")
			item.set_text(0, dict.keys()[i])
			item.set_meta("Name", dict.keys()[i])
			item.set_meta("Id", dict.values()[i])		
		if type == AkUtils.AkType.SWITCH_GROUP:
			icon	= AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/switchgroup.png")
			item.set_text(0, dict.keys()[i])
			item.set_meta("Name", dict.keys()[i])
			item.set_meta("Id", dict.values()[i].get("GROUP"))
		if type == AkUtils.AkType.SWITCH:
			if !user_data:
				return
			icon	= AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/switch.png")
			item.set_text(0, dict.keys()[i])
			item.set_meta("Name", dict.keys()[i])
			item.set_meta("Id", dict.values()[i])	
		if type == AkUtils.AkType.AUX_BUS:
			icon	= AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/auxbus.png")
			item.set_text(0, dict.keys()[i])
			item.set_meta("Name", dict.keys()[i])
			item.set_meta("Id", dict.values()[i])	
		if type == AkUtils.AkType.RTPC:
			icon	= AkUtils.create_icon("res://addons/wwise/editor/waapi_picker/icons/rtpc.png")
			item.set_text(0, dict.keys()[i])
			item.set_meta("Name", dict.keys()[i])
			item.set_meta("Id", dict.values()[i])	
				
		item.set_icon(0, icon)
	
func _on_text_changed(text):
	populate_browser(text)

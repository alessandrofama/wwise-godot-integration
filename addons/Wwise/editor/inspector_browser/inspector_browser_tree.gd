@tool
extends Tree

var selected_item: TreeItem = null
var root: TreeItem = null
var icon: Texture2D
var search_text: LineEdit
var type: int = AkUtils.AKTYPE_EVENT
var user_data: Dictionary


func initialize():
	populate_browser("")
	search_text = get_parent().get_node("TopVBoxContainer/SearchText")
	search_text.connect("text_changed", Callable(self, "_on_text_changed"))


func populate_browser(text_filter: String):
	if root:
		self.clear()
	root = self.create_item()
	self.hide_root = true
	root.set_text(0, "Root")

	var dict
	match type:
		AkUtils.AKTYPE_EVENT:
			dict = AK.EVENTS._dict
		AkUtils.AKTYPE_BANK:
			dict = AK.BANKS._dict
		AkUtils.AKTYPE_STATE_GROUP:
			dict = AK.STATES._dict
		AkUtils.AKTYPE_STATE:
			if !user_data.is_empty():
				if AK.STATES._dict.get(user_data.get("name")):
					dict = AK.STATES._dict.get(user_data.get("name")).get("STATE")
				else:
					dict = {}
		AkUtils.AKTYPE_SWITCH_GROUP:
			dict = AK.SWITCHES._dict
		AkUtils.AKTYPE_SWITCH:
			if !user_data.is_empty():
				if AK.SWITCHES._dict.get(user_data.get("name")):
					dict = AK.SWITCHES._dict.get(user_data.get("name")).get("SWITCH")
				else:
					dict = {}
		AkUtils.AKTYPE_AUX_BUS:
			dict = AK.AUX_BUSSES._dict
		AkUtils.AKTYPE_RTPC:
			dict = AK.GAME_PARAMETERS._dict

	for i in dict.size():
		if !dict.keys()[i].findn(text_filter) != -1 && !text_filter.is_empty():
			continue

		var item = self.create_item(root)

		if type == AkUtils.AKTYPE_EVENT:
			icon = load("res://addons/Wwise/editor/images/event.svg")
			item.set_text(0, dict.keys()[i])
			item.set_meta("name", dict.keys()[i])
			item.set_meta("id", dict.values()[i])
		if type == AkUtils.AKTYPE_BANK:
			icon = load("res://addons/Wwise/editor/images/soundbank.svg")
			item.set_text(0, dict.keys()[i])
			item.set_meta("name", dict.keys()[i])
			item.set_meta("id", dict.values()[i])
		if type == AkUtils.AKTYPE_STATE_GROUP:
			icon = load("res://addons/Wwise/editor/images/stategroup.svg")
			item.set_text(0, dict.keys()[i])
			item.set_meta("name", dict.keys()[i])
			item.set_meta("id", dict.values()[i].get("GROUP"))
		if type == AkUtils.AKTYPE_STATE:
			if user_data.is_empty():
				return
			icon = load("res://addons/Wwise/editor/images//state.svg")
			item.set_text(0, dict.keys()[i])
			item.set_meta("name", dict.keys()[i])
			item.set_meta("id", dict.values()[i])
		if type == AkUtils.AKTYPE_SWITCH_GROUP:
			icon = load("res://addons/Wwise/editor/images/switchgroup.svg")
			item.set_text(0, dict.keys()[i])
			item.set_meta("name", dict.keys()[i])
			item.set_meta("id", dict.values()[i].get("GROUP"))
		if type == AkUtils.AKTYPE_SWITCH:
			if user_data.is_empty():
				return
			icon = load("res://addons/Wwise/editor/images/switch.svg")
			item.set_text(0, dict.keys()[i])
			item.set_meta("name", dict.keys()[i])
			item.set_meta("id", dict.values()[i])
		if type == AkUtils.AKTYPE_AUX_BUS:
			icon = load("res://addons/Wwise/editor/images/auxbus.svg")
			item.set_text(0, dict.keys()[i])
			item.set_meta("name", dict.keys()[i])
			item.set_meta("id", dict.values()[i])
		if type == AkUtils.AKTYPE_RTPC:
			icon = load("res://addons/Wwise/editor/images/rtpc.svg")
			item.set_text(0, dict.keys()[i])
			item.set_meta("name", dict.keys()[i])
			item.set_meta("id", dict.values()[i])

		item.set_icon(0, icon)


func _on_text_changed(text):
	populate_browser(text)

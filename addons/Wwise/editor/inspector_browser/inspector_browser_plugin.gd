extends EditorInspectorPlugin

var inspector_browser_editor = load("res://addons/wwise/editor/inspector_browser/inspector_browser_editor.gd")

func _can_handle(object: Object) -> bool:
	return (object is AkBank
	|| object is AkState
	|| object is AkSwitch)	 
	
	# todo(alex): replace these
#	return (object is ak_event 
#	|| object is ak_event_2d 
#	|| object is ak_bank 
#	|| object is ak_state 
#	|| object is ak_switch 
#	|| object is ak_room 
#	|| object is ak_environment )

func _parse_property(object: Object, type, name: String, hint_type: PropertyHint, hint_string: String, usage_flags: PropertyUsageFlags, wide: bool) -> bool:
	if type == TYPE_DICTIONARY && name == "event":
		add_property_editor(name, inspector_browser_editor.new(AkUtils.AKTYPE_EVENT))
		return true
	elif type == TYPE_DICTIONARY && name == "bank":
		add_property_editor(name, inspector_browser_editor.new(AkUtils.AKTYPE_BANK))
		return true
	elif type == TYPE_DICTIONARY && name == "state_group":
		add_property_editor(name, inspector_browser_editor.new(AkUtils.AKTYPE_STATE_GROUP, object.state_group))
		return true
	elif type == TYPE_DICTIONARY && name == "state_value":
		add_property_editor(name, inspector_browser_editor.new(AkUtils.AKTYPE_STATE, object.state_group))
		return true
	elif type == TYPE_DICTIONARY && name == "switch_group":
		add_property_editor(name, inspector_browser_editor.new(AkUtils.AKTYPE_SWITCH_GROUP, object.switch_group))
		return true
	elif type == TYPE_DICTIONARY && name == "switch_value":
		add_property_editor(name, inspector_browser_editor.new(AkUtils.AKTYPE_SWITCH, object.switch_value))
		return true
	elif type == TYPE_DICTIONARY && name == "aux_bus":
		add_property_editor(name, inspector_browser_editor.new(AkUtils.AKTYPE_AUX_BUS))
		return true
	elif type == TYPE_DICTIONARY && name == "game_parameter":
		add_property_editor(name, inspector_browser_editor.new(AkUtils.AKTYPE_RTPC))
		return true
	else:
		return false	

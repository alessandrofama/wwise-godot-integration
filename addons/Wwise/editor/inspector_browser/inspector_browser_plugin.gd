extends EditorInspectorPlugin

var inspector_browser_editor = load(
	"res://addons/Wwise/editor/inspector_browser/inspector_browser_editor.gd"
)


func _can_handle(object: Object) -> bool:
	return (
		object is AkBank
		|| object is AkState
		|| object is AkSwitch
		|| object is AkEvent2D
		|| object is AkEvent3D
		|| object is AkEnvironment
		|| object is AkRoom
		|| object is AkEarlyReflections
	)


func _parse_property(
	object,
	type,
	name,
	hint_type,
	hint_string,
	usage_flags,
	wide
) -> bool:
	if type == TYPE_DICTIONARY && name == "event":
		add_property_editor(name, inspector_browser_editor.new(AkUtils.AKTYPE_EVENT))
		return true
	elif type == TYPE_DICTIONARY && name == "bank":
		add_property_editor(name, inspector_browser_editor.new(AkUtils.AKTYPE_BANK))
		return true
	elif type == TYPE_DICTIONARY && name == "state_group":
		add_property_editor(
			name, inspector_browser_editor.new(AkUtils.AKTYPE_STATE_GROUP, object.state_group)
		)
		return true
	elif type == TYPE_DICTIONARY && name == "state_value":
		add_property_editor(
			name, inspector_browser_editor.new(AkUtils.AKTYPE_STATE, object.state_group)
		)
		return true
	elif type == TYPE_DICTIONARY && name == "switch_group":
		add_property_editor(
			name, inspector_browser_editor.new(AkUtils.AKTYPE_SWITCH_GROUP, object.switch_group)
		)
		return true
	elif type == TYPE_DICTIONARY && name == "switch_value":
		add_property_editor(
			name, inspector_browser_editor.new(AkUtils.AKTYPE_SWITCH, object.switch_value)
		)
		return true
	elif type == TYPE_DICTIONARY && name == "aux_bus":
		add_property_editor(name, inspector_browser_editor.new(AkUtils.AKTYPE_AUX_BUS))
		return true
	elif type == TYPE_DICTIONARY && name == "game_parameter":
		add_property_editor(name, inspector_browser_editor.new(AkUtils.AKTYPE_RTPC))
		return true
	else:
		return false

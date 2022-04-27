extends EditorInspectorPlugin

var inspector_browser_editor = load("res://addons/wwise/editor/inspector_browser/inspector_browser_editor.gd")

var ak_event = load("res://addons/wwise/runtime/nodes/ak_event.gd")
var ak_event_2d = load("res://addons/wwise/runtime/nodes/ak_event_2d.gd")
var ak_bank = load("res://addons/wwise/runtime/nodes/ak_bank.gd")
var ak_state = load("res://addons/wwise/runtime/nodes/ak_state.gd")
var ak_switch = load("res://addons/wwise/runtime/nodes/ak_switch.gd")
var ak_room = load("res://addons/wwise/runtime/nodes/ak_room.gd")
var ak_environment = load("res://addons/wwise/runtime/nodes/ak_environment.gd")

var ak_event_vs = load("res://addons/wwise/runtime/visual nodes/ak_post_event_vs.gd")
var ak_event_2d_vs = load("res://addons/wwise/runtime/visual nodes/ak_post_event_2d_vs.gd")
var ak_load_bank_vs = load("res://addons/wwise/runtime/visual nodes/ak_load_bank_vs.gd")
var ak_unload_bank_vs = load("res://addons/wwise/runtime/visual nodes/ak_unload_bank_vs.gd")
var ak_state_vs = load("res://addons/wwise/runtime/visual nodes/ak_set_state_vs.gd")
var ak_switch_vs = load("res://addons/wwise/runtime/visual nodes/ak_set_switch_vs.gd")
var ak_set_rtpc_vs = load("res://addons/wwise/runtime/visual nodes/ak_set_rtpc_value_vs.gd")
var ak_get_rtpc_vs = load("res://addons/wwise/runtime/visual nodes/ak_get_rtpc_value_vs.gd")

func can_handle(object):
	return (object is ak_event 
	|| object is ak_event_2d 
	|| object is ak_bank 
	|| object is ak_state 
	|| object is ak_switch 
	|| object is ak_room 
	|| object is ak_environment 
	|| object is ak_event_vs
	|| object is ak_event_2d_vs
	|| object is ak_load_bank_vs
	|| object is ak_unload_bank_vs
	|| object is ak_state_vs
	|| object is ak_switch_vs
	|| object is ak_set_rtpc_vs
	|| object is ak_get_rtpc_vs)

func parse_property(object: Object, type: int, path: String, hint: int, hint_text: String, usage: int):
	if type == TYPE_DICTIONARY && path == "event":
		add_property_editor(path, inspector_browser_editor.new(AkUtils.AkType.EVENT))
		return true
	elif type == TYPE_DICTIONARY && path == "bank":
		add_property_editor(path, inspector_browser_editor.new(AkUtils.AkType.BANK))
		return true
	elif type == TYPE_DICTIONARY && path == "state_group":
		add_property_editor(path, inspector_browser_editor.new(AkUtils.AkType.STATE_GROUP, object.state_group))
		return true
	elif type == TYPE_DICTIONARY && path == "state_value":
		add_property_editor(path, inspector_browser_editor.new(AkUtils.AkType.STATE, object.state_group))
		return true
	elif type == TYPE_DICTIONARY && path == "switch_group":
		add_property_editor(path, inspector_browser_editor.new(AkUtils.AkType.SWITCH_GROUP, object.switch_group))
		return true
	elif type == TYPE_DICTIONARY && path == "switch_value":
		add_property_editor(path, inspector_browser_editor.new(AkUtils.AkType.SWITCH, object.switch_value))
		return true
	elif type == TYPE_DICTIONARY && path == "aux_bus":
		add_property_editor(path, inspector_browser_editor.new(AkUtils.AkType.AUX_BUS))
		return true
	elif type == TYPE_DICTIONARY && path == "game_parameter":
		add_property_editor(path, inspector_browser_editor.new(AkUtils.AkType.RTPC))
		return true
	else:
		return false	

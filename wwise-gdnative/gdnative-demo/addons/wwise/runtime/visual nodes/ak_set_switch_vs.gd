tool
extends VisualScriptCustomNode

export(Dictionary) var switch_group:Dictionary = {"Name": "", "Id": 0}
export(Dictionary) var switch_value:Dictionary = {"Name": "", "Id": 0}

func _get_caption():
	return "Ak Set Switch"
	
func _get_category():
	return "Wwise"
	
func _get_text():
	return switch_group.get("Name") + " / " + switch_value.get("Name")

func _has_input_sequence_port():
	return true
	
func _get_output_sequence_port_count():
	return 1
	
func _get_input_value_port_count():
	return 1
	
func _get_input_value_port_name(idx):
	match idx:
		0: return "Game Object"

func _get_input_value_port_type(idx):
	match idx:
		0:	return TYPE_OBJECT

func _step(inputs, _outputs, start_mode, _working_mem):
	if start_mode == START_MODE_BEGIN_SEQUENCE:
		Wwise.set_switch_id(switch_group.get("Id"), switch_value.get("Id"), inputs[0])

	return 0

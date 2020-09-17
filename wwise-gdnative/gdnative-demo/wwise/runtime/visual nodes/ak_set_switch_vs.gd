tool
extends VisualScriptCustomNode

export(String) var switch_group:String
export(String) var switch_value:String

func _get_caption():
	return "Ak Set Switch"
	
func _get_category():
	return "Wwise"
	
func _get_text():
	return switch_group + " / " + switch_value

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
		Wwise.set_switch(switch_group, switch_value, inputs[0])

	return 0

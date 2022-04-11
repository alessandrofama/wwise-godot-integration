tool
extends VisualScriptCustomNode

export(String) var state_group:String
export(String) var state_value:String

func _get_caption():
	return "Ak Set State"
	
func _get_category():
	return "Wwise"
	
func _get_text():
	return state_group + " / " + state_value

func _has_input_sequence_port():
	return true
	
func _get_output_sequence_port_count():
	return 1

func _step(_inputs, _outputs, start_mode, _working_mem):
	if start_mode == START_MODE_BEGIN_SEQUENCE:
		Wwise.set_state(state_group, state_value)

	return 0

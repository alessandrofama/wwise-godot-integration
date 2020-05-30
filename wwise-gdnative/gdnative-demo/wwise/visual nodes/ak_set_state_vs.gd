tool
extends VisualScriptCustomNode

export(int) var state_group:int
export(int) var state_value:int

func _get_caption():
	return "Ak Set State"
	
func _get_category():
	return "Wwise"

func _has_input_sequence_port():
	return true
	
func _get_output_sequence_port_count():
	return 1

func _step(_inputs, _outputs, start_mode, _working_mem):
	if start_mode == START_MODE_BEGIN_SEQUENCE:
		Wwise.set_state_id(state_group, state_value)

	return 0

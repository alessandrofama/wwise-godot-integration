tool
extends VisualScriptCustomNode

export(Dictionary) var state_group:Dictionary = {"Name": "", "Id": 0}
export(Dictionary) var state_value:Dictionary = {"Name": "", "Id": 0}

func _get_caption():
	return "Ak Set State"
	
func _get_category():
	return "Wwise"
	
func _get_text():
	return state_group.get("Name") + " / " + state_value.get("Name")

func _has_input_sequence_port():
	return true
	
func _get_output_sequence_port_count():
	return 1

func _step(_inputs, _outputs, start_mode, _working_mem):
	if start_mode == START_MODE_BEGIN_SEQUENCE:
		Wwise.set_state_id(state_group.get("Id"), state_value.get("Id"))

	return 0

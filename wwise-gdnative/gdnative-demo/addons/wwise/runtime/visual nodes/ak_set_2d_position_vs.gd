tool
extends VisualScriptCustomNode

func _get_caption():
	return "Ak Set 2D Position"
	
func _get_category():
	return "Wwise"

func _get_text():
	return ""
	
func _has_input_sequence_port():
	return true
	
func _get_output_sequence_port_count():
	return 1
	
func _get_input_value_port_count():
	return 3
	
func _get_input_value_port_name(idx):
	match idx:
		0: return "Game Object"
		1: return "Transform 2D"
		2: return "zDepth"

func _get_input_value_port_type(idx):
	match idx:
		0: return TYPE_OBJECT
		1: return TYPE_TRANSFORM2D
		2: return TYPE_REAL

func _step(inputs, _outputs, start_mode,_working_mem):
	var zdepth = 0.0
	if start_mode == START_MODE_BEGIN_SEQUENCE:
		if not inputs[0] is Object:
			push_warning("Please connect a valid GameObject to the Ak Set 2D Position node")
		if not inputs[1] is Transform2D:
			push_warning("Please connect a valid Transform to the Ak Set 2D Position node")
		if not inputs[2] is float:
			zdepth = 0.0
		else:
			zdepth = inputs[2]
	
	Wwise.set_2d_position(inputs[0], inputs[1], zdepth)
	return 0

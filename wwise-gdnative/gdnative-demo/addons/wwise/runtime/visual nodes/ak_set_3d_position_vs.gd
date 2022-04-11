tool
extends VisualScriptCustomNode

func _get_caption():
	return "Ak Set 3D Position"
	
func _get_category():
	return "Wwise"

func _get_text():
	return ""
	
func _has_input_sequence_port():
	return true
	
func _get_output_sequence_port_count():
	return 1
	
func _get_input_value_port_count():
	return 2
	
func _get_input_value_port_name(idx):
	match idx:
		0: return "Game Object"
		1: return "Transform"

func _get_input_value_port_type(idx):
	match idx:
		0: return TYPE_OBJECT
		1: return TYPE_TRANSFORM

func _step(inputs, _outputs, start_mode,_working_mem):
	if start_mode == START_MODE_BEGIN_SEQUENCE:
		if not inputs[0] is Object:
			push_warning("Please connect a valid GameObject to the Ak Set 3D Position node")
		if not inputs[1] is Transform:
			push_warning("Please connect a valid Transform to the Ak Set 3D Position node")

	Wwise.set_3d_position(inputs[0], inputs[1])
	return 0

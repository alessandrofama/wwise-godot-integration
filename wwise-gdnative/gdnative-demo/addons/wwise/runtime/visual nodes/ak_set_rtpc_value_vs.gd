tool
extends VisualScriptCustomNode

export(Dictionary) var game_parameter:Dictionary = {"Name": "", "Id": 0}

func _get_caption():
	return "Ak Set RTPC Value"
	
func _get_category():
	return "Wwise"
	
func _get_text():
	return game_parameter.get("Name")

func _has_input_sequence_port():
	return true
	
func _get_output_sequence_port_count():
	return 1
	
func _get_input_value_port_count():
	return 2
	
func _get_input_value_port_name(idx):
	match idx:
		0: return "Value"
		1: return "Game Object"

func _get_input_value_port_type(idx):
	match idx:
		0:	return TYPE_REAL
		1: 	return TYPE_OBJECT

func _step(inputs, _outputs, _start_mode, _working_mem):
	Wwise.set_rtpc_id(game_parameter.get("Id"), inputs[0], inputs[1])

	return 0

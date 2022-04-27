tool
extends VisualScriptCustomNode

export(Dictionary) var bank:Dictionary = {"Name": "", "Id": 0}

func _get_caption():
	return "Ak Load Bank"
	
func _get_category():
	return "Wwise"
	
func _get_text():
	return bank.get("Name")
	
func _has_input_sequence_port():
	return true
	
func _get_output_sequence_port_count():
	return 1

func _step(_inputs, _outputs, start_mode, _working_mem):
	if start_mode == START_MODE_BEGIN_SEQUENCE:
		Wwise.load_bank_id(bank.get("Id"))

	return 0

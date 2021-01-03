tool
extends VisualScriptCustomNode

export(AK.BANKS._enum) var bank:int = AK.BANKS._enum.values()[0]

func _get_caption():
	return "Ak Unload Bank"
	
func _get_category():
	return "Wwise"
	
func _get_text():
	for idx in AK.BANKS._enum.values().size():
		if bank == AK.BANKS._enum.values()[idx]:
			return AK.BANKS._enum.keys()[idx]

func _has_input_sequence_port():
	return true
	
func _get_output_sequence_port_count():
	return 1

func _step(_inputs, _outputs, start_mode, _working_mem):
	if start_mode == START_MODE_BEGIN_SEQUENCE:
		Wwise.unload_bank_id(bank)

	return 0

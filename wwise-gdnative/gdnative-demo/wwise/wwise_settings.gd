tool
extends Node

func _init():

	_add_setting("wwise/common_user_settings/base_path", String("res://Banks"), 
				TYPE_STRING, PROPERTY_HINT_DIR, "res://Banks")
	_add_setting("wwise/common_user_settings/startup_language", String("English(US)"), TYPE_STRING, 
				PROPERTY_HINT_ENUM, "English(US),English(UK)")
	_add_setting("wwise/common_user_settings/callback_manager_buffer_size", 4096, TYPE_INT, 
				PROPERTY_HINT_NONE, "2048,4096,8192")

	var error = ProjectSettings.save()
	
	if error: 
		push_error("Encountered error %d when saving project settings." % error)

func _add_setting(name, defaultValue, type, hint, hintString):

	if ProjectSettings.has_setting(name): return

	var settingInfo = {
		"name": name,
		"type": type,
		"hint": hint,
		"hint_string": hintString
	}

	ProjectSettings.set_setting(name, defaultValue)
	ProjectSettings.add_property_info(settingInfo)
	ProjectSettings.set_initial_value(name, defaultValue)

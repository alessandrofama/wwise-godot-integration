tool
extends EditorPlugin

var timer:Timer

func _enter_tree():
	timer = Timer.new()
	timer.connect("timeout",self,"_on_timer_timeout") 
	timer.wait_time = 5
	timer.one_shot = true
	add_child(timer) 
	timer.start()
	print("Starting Wwise Setup in 5 seconds...") 

func _on_timer_timeout():
	call_deferred("add_autoload_singleton", "WwiseSettings", "res://wwise/wwise_settings.gd")
	call_deferred("add_autoload_singleton", "Wwise", "res://wwise/bin/wwise-gdnative.gdns")
	call_deferred("add_autoload_singleton", "Waapi", "res://wwise/bin/waapi-gdnative-debug.gdns")
	get_editor_interface().call_deferred("set_plugin_enabled", "res://addons/wwise_build_export/plugin.cfg", true)
	get_editor_interface().call_deferred("set_plugin_enabled", "res://addons/wwise_ids_converter/plugin.cfg", true)
	get_editor_interface().call_deferred("set_plugin_enabled", "res://addons/wwise_custom_nodes/plugin.cfg", true)
	get_editor_interface().call_deferred("set_plugin_enabled", "res://addons/waapi_picker/plugin.cfg", true)
	ProjectSettings.save()
	timer.call_deferred("free")
	get_editor_interface().call_deferred("set_plugin_enabled", "res://addons/wwise_setup/plugin.cfg", false)

func _exit_tree():
	print("Wwise setup completed successfully")
	ProjectSettings.save()

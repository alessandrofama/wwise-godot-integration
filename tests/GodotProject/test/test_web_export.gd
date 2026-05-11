class_name TestWebExport
extends GdUnitTestSuite


func test_web_audio_worklet_processor_url_setting_registered() -> void:
	var key := "wwise/web_advanced_settings/audio_worklet_processor_url"
	assert_bool(ProjectSettings.has_setting(key)).is_true()
	assert_str(str(ProjectSettings.get_setting(key))).is_equal("WwiseAudioWorklet.processor.js")


func test_web_verbose_output_setting_registered() -> void:
	var key := "wwise/web_advanced_settings/verbose_system_output"
	assert_bool(ProjectSettings.has_setting(key)).is_true()
	assert_bool(bool(ProjectSettings.get_setting(key))).is_false()


func test_web_platform_info_setting_registered() -> void:
	var key := "wwise/project_settings/web_platform_info"
	assert_bool(ProjectSettings.has_setting(key)).is_true()


func test_ak_web_export_plugin_class_registered() -> void:
	assert_bool(ClassDB.class_exists("AkWebExportPlugin")).is_true()
	assert_bool(ClassDB.is_parent_class("AkWebExportPlugin", "EditorExportPlugin")).is_true()


func test_gdextension_has_web_library_entries() -> void:
	var config := ConfigFile.new()
	var err := config.load("res://addons/Wwise/native/wwise.gdextension")
	assert_int(err).is_equal(OK)
	assert_bool(config.has_section_key("libraries", "web.debug")).is_true()
	assert_bool(config.has_section_key("libraries", "web.release")).is_true()
	var debug_path: String = config.get_value("libraries", "web.debug", "")
	assert_str(debug_path).ends_with(".wasm")

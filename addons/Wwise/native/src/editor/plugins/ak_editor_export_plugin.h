#pragma once

#include "core/wwise_platform_info.h"
#include "core/wwise_settings.h"
#include "editor/ak_editor_settings.h"
#include <godot_cpp/classes/config_file.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/editor_export_plugin.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <map>

using namespace godot;

class AkEditorExportPlugin : public EditorExportPlugin
{
	GDCLASS(AkEditorExportPlugin, EditorExportPlugin);

protected:
	static void _bind_methods() {}

private:
	String get_platform_banks_path(
			const Ref<WwisePlatformInfo>& p_platform_info, const String& p_root_output_path) const;
	String get_platform_library_path(const Ref<ConfigFile>& p_gdextension_config, const String& p_platform,
			const PackedStringArray& p_features, bool p_is_debug) const;
	void export_plugins_for_platform(const String& p_platform, const Ref<WwisePlatformInfo>& p_platform_info,
			const String& p_dsp_path, const PackedStringArray& p_features);
	void handle_windows_plugin(const Ref<WwisePluginInfo>& p_plugin_info, const String& p_dsp_path);
	void handle_macos_plugin(const Ref<WwisePluginInfo>& p_plugin_info, const String& p_dsp_path);
	void handle_linux_plugin(const Ref<WwisePluginInfo>& p_plugin_info, const String& p_dsp_path);
	void handle_ios_plugins(const TypedArray<WwisePluginInfo>& p_plugin_infos, const String& p_dsp_path);
	void handle_android_plugin(
			const Ref<WwisePluginInfo>& p_plugin_info, const String& p_dsp_path, const PackedStringArray& p_features);
	void add_files_recursive(Ref<DirAccess> dir, const String& base_path);

public:
	virtual String _get_name() const override;
	virtual void _export_begin(
			const PackedStringArray& p_features, bool p_is_debug, const String& p_path, uint32_t p_flags) override;
};
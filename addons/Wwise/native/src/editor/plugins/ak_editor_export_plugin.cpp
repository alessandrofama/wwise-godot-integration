#include "ak_editor_export_plugin.h"

String AkEditorExportPlugin::get_platform_banks_path(
		const Ref<WwisePlatformInfo>& p_platform_info, const String& p_root_output_path) const
{
	String platform_path = p_platform_info->get_platform_path();
	return p_root_output_path.path_join(platform_path);
}

String AkEditorExportPlugin::get_platform_library_path(const Ref<ConfigFile>& p_gdextension_config,
		const String& p_platform, const PackedStringArray& p_features, bool p_is_debug) const
{
	if (p_platform == "android")
	{
		bool is_arm64 = p_features.has("arm64");
		String sub_arch = is_arm64 ? "arm64" : "arm32";
		return p_gdextension_config->get_value(
				"libraries", vformat("%s.%s.%s", p_platform, p_is_debug ? "debug" : "release", sub_arch), "");
	}

	return p_gdextension_config->get_value(
			"libraries", vformat("%s.%s", p_platform, p_is_debug ? "debug" : "release"), "");
}

void AkEditorExportPlugin::export_plugins_for_platform(const String& p_platform,
		const Ref<WwisePlatformInfo>& p_platform_info, const String& p_dsp_path, const PackedStringArray& p_features)
{
	TypedArray<WwisePluginInfo> plugin_infos = p_platform_info->get_plugin_info();

	if (p_platform == "ios")
	{
		handle_ios_plugins(plugin_infos, p_dsp_path);
		return;
	}

	for (int i = 0; i < plugin_infos.size(); i++)
	{
		Ref<WwisePluginInfo> plugin_info = plugin_infos[i];
		if (!plugin_info.is_valid())
		{
			continue;
		}

		if (p_platform == "windows")
		{
			handle_windows_plugin(plugin_info, p_dsp_path);
		}
		else if (p_platform == "macos")
		{
			handle_macos_plugin(plugin_info, p_dsp_path);
		}
		else if (p_platform == "linux")
		{
			handle_linux_plugin(plugin_info, p_dsp_path);
		}
		else if (p_platform == "android")
		{
			handle_android_plugin(plugin_info, p_dsp_path, p_features);
		}
	}
}

void AkEditorExportPlugin::handle_windows_plugin(const Ref<WwisePluginInfo>& p_plugin_info, const String& p_dsp_path)
{
	String plugin_path = p_dsp_path.path_join(vformat("%s.dll", p_plugin_info->get_dll_name()));
	add_shared_object(plugin_path, PackedStringArray({ "windows", "x86_64" }), "");
}

void AkEditorExportPlugin::handle_macos_plugin(const Ref<WwisePluginInfo>& p_plugin_info, const String& p_dsp_path)
{
	String plugin_path = p_dsp_path.path_join(vformat("lib%s.dylib", p_plugin_info->get_dll_name()));
	add_shared_object(plugin_path, PackedStringArray({ "macos", "arm64", "x86_64" }), "");
}

void AkEditorExportPlugin::handle_linux_plugin(const Ref<WwisePluginInfo>& p_plugin_info, const String& p_dsp_path)
{
	String plugin_path = p_dsp_path.path_join(vformat("lib%s.so", p_plugin_info->get_dll_name()));
	add_shared_object(plugin_path, PackedStringArray({ "linux", "x86_64" }), "");
}

void AkEditorExportPlugin::handle_ios_plugins(
		const TypedArray<WwisePluginInfo>& p_plugin_infos, const String& p_dsp_path)
{
	String accumulated_header_text;

	for (int i = 0; i < p_plugin_infos.size(); i++)
	{
		Ref<WwisePluginInfo> plugin_info = p_plugin_infos[i];
		if (!plugin_info.is_valid())
		{
			continue;
		}

		String plugin_path = p_dsp_path.path_join(vformat("lib%s.a", plugin_info->get_static_lib_name()));
		add_ios_project_static_lib(plugin_path);

		auto plugin_it =
				AkEditorSettings::ios_header_mapping.find((AkEditorSettings::PluginID)plugin_info->get_plugin_id());
		if (plugin_it != AkEditorSettings::ios_header_mapping.end())
		{
			String header_path = p_dsp_path.path_join(vformat("%s.h", plugin_it->second));
			if (FileAccess::file_exists(header_path))
			{
				Ref<FileAccess> file = FileAccess::open(header_path, FileAccess::ModeFlags::READ);
				String code = file->get_as_text();
				file->close();

				accumulated_header_text += code;
				accumulated_header_text += "\n\n";
			}
		}
	}

	String prelude;
	prelude += "namespace AK { class PluginRegistration; };\n";
	prelude += "class AkGodotStaticPlugin;\n";
	prelude += "AkGodotStaticPlugin * g_pAkGodotStaticPluginList = nullptr;\n";
	prelude += "class AkGodotStaticPlugin {\n";
	prelude += "\tpublic:\n";
	prelude += "\t\tAkGodotStaticPlugin(AK::PluginRegistration* pReg) : m_pNext(g_pAkGodotStaticPluginList), "
			   "m_pReg(pReg)\n";
	prelude += "\t\t{\n";
	prelude += "\t\t\tg_pAkGodotStaticPluginList = this;\n";
	prelude += "\t\t}\n";
	prelude += "\t\tAkGodotStaticPlugin *m_pNext;\n";
	prelude += "\t\tAK::PluginRegistration * m_pReg;\n";
	prelude += "};\n\n";

	prelude += "#define AK_STATIC_LINK_PLUGIN(_pluginName_) \\\n";
	prelude += "extern AK::PluginRegistration _pluginName_##Registration; \\\n";
	prelude += "AkGodotStaticPlugin _pluginName_##GodotStaticPlugin(&_pluginName_##Registration);\n";

	String final_cpp_code = prelude + accumulated_header_text;

	add_ios_cpp_code(final_cpp_code);
}

void AkEditorExportPlugin::handle_android_plugin(
		const Ref<WwisePluginInfo>& p_plugin_info, const String& p_dsp_path, const PackedStringArray& p_features)
{
	if (p_features.has("arm64"))
	{
		String plugin_path_arm64 = p_dsp_path.path_join(vformat("lib%s.so", p_plugin_info->get_dll_name()));
		add_shared_object(plugin_path_arm64, PackedStringArray({ "android", "arm64" }), "");
	}

	if (p_features.has("arm32"))
	{
		String plugin_path_arm32 = p_dsp_path.path_join(vformat("lib%s.so", p_plugin_info->get_dll_name()));
		add_shared_object(plugin_path_arm32, PackedStringArray({ "android", "arm32" }), "");
	}
}

void AkEditorExportPlugin::add_files_recursive(Ref<DirAccess> dir, const String& base_path)
{
	if (!dir.is_valid())
		return;

	dir->list_dir_begin();
	String file_name;

	while ((file_name = dir->get_next()) != String())
	{
		if (file_name == "." || file_name == "..")
			continue;

		String file_path = dir->get_current_dir().path_join(file_name);

		if (dir->current_is_dir())
		{
			Ref<DirAccess> sub_dir = DirAccess::open(file_path);
			if (sub_dir.is_valid())
			{
				add_files_recursive(sub_dir, base_path);
			}
		}
		else
		{
			String extension = file_name.get_extension();
			if (extension == "bnk" || extension == "wem")
			{
				PackedByteArray file_data;
				Ref<FileAccess> file = FileAccess::open(file_path, FileAccess::ModeFlags::READ);
				if (file.is_valid())
				{
					file_data = file->get_buffer(file->get_length());
					file->close();
					add_file(file_path, file_data, false);
				}
			}
		}
	}
	dir->list_dir_end();
}

String AkEditorExportPlugin::_get_name() const { return "AkEditorExportPlugin"; }

void AkEditorExportPlugin::_export_begin(
		const PackedStringArray& p_features, bool p_is_debug, const String& p_path, uint32_t p_flags)
{
	Ref<ConfigFile> gdextension_config;
	gdextension_config.instantiate();
	gdextension_config->load(AkEditorSettings::get_gdextension_config_path());

	WwiseSettings* settings = WwiseSettings::get_singleton();
	String root_output_path = settings->get_setting(settings->common_user_settings.root_output_path);

	std::map<String, StringName> platform_settings{ { "windows", settings->project_settings.windows_platform_info },
		{ "macos", settings->project_settings.mac_platform_info },
		{ "linux", settings->project_settings.linux_platform_info },
		{ "ios", settings->project_settings.ios_platform_info },
		{ "android", settings->project_settings.android_platform_info } };

	for (const auto& feature : p_features)
	{
		auto it = platform_settings.find(feature);
		if (it == platform_settings.end())
		{
			continue;
		}

		String platform_info_res_path = settings->get_setting(it->second);
		Ref<WwisePlatformInfo> platform_info = ResourceLoader::get_singleton()->load(platform_info_res_path);
		if (!platform_info.is_valid())
		{
			continue;
		}

		String platform_banks_path = get_platform_banks_path(platform_info, root_output_path);
		Ref<DirAccess> dir = DirAccess::open(platform_banks_path);
		if (!dir.is_valid())
		{
			continue;
		}

		add_files_recursive(dir, platform_banks_path);

		String library_path = get_platform_library_path(gdextension_config, it->first, p_features, p_is_debug);
		String dsp_path = library_path.get_base_dir().path_join("DSP");

		export_plugins_for_platform(it->first, platform_info, dsp_path, p_features);
	}
}
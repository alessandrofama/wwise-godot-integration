#include "wwise_settings.h"

WwiseSettings* WwiseSettings::singleton = nullptr;

void WwiseSettings::_bind_methods() {}

WwiseSettings* WwiseSettings::get_singleton() { return singleton; }

WwiseSettings::WwiseSettings()
{
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;

	load_options_from_json("res://addons/Wwise/metadata/Options.Godot.json", "");
	load_options_from_json("res://addons/Wwise/metadata/Options.Common.json", "");

	if (Engine::get_singleton()->is_editor_hint())
	{
		load_options_from_json("res://addons/Wwise/metadata/Options.Windows.json", "windows");
		load_options_from_json("res://addons/Wwise/metadata/Options.Mac.json", "macos");
		load_options_from_json("res://addons/Wwise/metadata/Options.Linux.json", "linux");
		load_options_from_json("res://addons/Wwise/metadata/Options.Android.json", "android");
		load_options_from_json("res://addons/Wwise/metadata/Options.iOS.json", "ios");
		load_options_from_json("res://addons/Wwise/metadata/Options.Emscripten.json", "web");

		remove_undefined_settings();
		defined_settings.reset();

		godot::Error error = godot::ProjectSettings::get_singleton()->save();
		if (error)
		{
			UtilityFunctions::printerr(
					vformat("WwiseGodot: Encountered error %d while saving Wwise Project Settings.", (int)error));
		}
		else
		{
			UtilityFunctions::print("WwiseGodot: Saved Project Settings.");
		}
	}
	else
	{
#ifdef AK_WIN
		load_options_from_json("res://addons/Wwise/metadata/Options.Windows.json", "windows");
#elif defined(AK_MAC_OS_X)
		load_options_from_json("res://addons/Wwise/metadata/Options.Mac.json", "macos");
#elif defined(AK_LINUX)
		load_options_from_json("res://addons/Wwise/metadata/Options.Linux.json", "linux");
#elif defined(AK_ANDROID)
		load_options_from_json("res://addons/Wwise/metadata/Options.Android.json", "android");
#elif defined(AK_IOS)
		load_options_from_json("res://addons/Wwise/metadata/Options.iOS.json", "ios");
#elif defined(AK_EMSCRIPTEN)
		load_options_from_json("res://addons/Wwise/metadata/Options.Emscripten.json", "web");
#endif
	}
}

WwiseSettings::~WwiseSettings()
{
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}

void WwiseSettings::set_setting(const String& p_name, const Variant& p_value)
{
	godot::ProjectSettings* settings = godot::ProjectSettings::get_singleton();

	if (settings && settings->has_setting(p_name))
	{
		settings->set_setting(p_name, p_value);
	}
	else
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: Trying to set non-existing setting: %s.", p_name));
	}
}

Variant WwiseSettings::get_setting(const StringName& p_setting, const Variant& p_default) const
{
	String platform_setting = p_setting;

#ifdef AK_WIN
	platform_setting += GODOT_WINDOWS_SETTING_POSTFIX;
#elif defined(AK_MAC_OS_X)
	platform_setting += GODOT_MAC_OSX_SETTING_POSTFIX;
#elif defined(AK_IOS)
	platform_setting += GODOT_IOS_SETTING_POSTFIX;
#elif defined(AK_ANDROID)
	platform_setting += GODOT_ANDROID_SETTING_POSTFIX;
#elif defined(AK_LINUX)
	platform_setting += GODOT_LINUX_SETTING_POSTFIX;
#elif defined(AK_EMSCRIPTEN)
	platform_setting += GODOT_WEB_SETTING_POSTFIX;
#else
#error "Platform not supported"
#endif

	godot::ProjectSettings* settings = godot::ProjectSettings::get_singleton();

	// Try to get the platform-specific setting, if it exists.
	if (settings && settings->has_setting(platform_setting))
	{
		return settings->get(platform_setting);
	}

	// Otherwise, try to get the default platform-agnostic setting.
	if (settings && settings->has_setting(p_setting))
	{
		return settings->get(p_setting);
	}
	else
	{
		UtilityFunctions::push_warning(
				vformat("WwiseGodot: Setting '%s' not found. Returning default value.", p_setting));
		return p_default;
	}
}

void WwiseSettings::set_setting_by_wwise_key(uint32_t p_key, const Variant& p_value)
{
	if (!wwise_key_to_godot_path.has(p_key))
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: Key %d not found in mapping.", p_key));
		return;
	}

	String base_setting = wwise_key_to_godot_path[p_key];
	String platform_setting = base_setting;

#ifdef AK_WIN
	platform_setting += GODOT_WINDOWS_SETTING_POSTFIX;
#elif defined(AK_MAC_OS_X)
	platform_setting += GODOT_MAC_OSX_SETTING_POSTFIX;
#elif defined(AK_IOS)
	platform_setting += GODOT_IOS_SETTING_POSTFIX;
#elif defined(AK_ANDROID)
	platform_setting += GODOT_ANDROID_SETTING_POSTFIX;
#elif defined(AK_LINUX)
	platform_setting += GODOT_LINUX_SETTING_POSTFIX;
#elif defined(AK_EMSCRIPTEN)
	platform_setting += GODOT_WEB_SETTING_POSTFIX;
#else
#error "Platform not supported"
#endif

	godot::ProjectSettings* settings = godot::ProjectSettings::get_singleton();
	if (!settings)
	{
		return;
	}

	if (settings->has_setting(platform_setting))
	{
		set_setting(platform_setting, p_value);
	}
	else if (settings->has_setting(base_setting))
	{
		set_setting(base_setting, p_value);
	}
	else
	{
		UtilityFunctions::push_warning(
				vformat("WwiseGodot: Setting '%s' not found for configuration update.", base_setting));
	}
}

Variant WwiseSettings::get_setting_by_wwise_key(uint32_t p_key, const Variant& p_default_value) const
{
	if (!wwise_key_to_godot_path.has(p_key))
		return p_default_value;

	return get_setting(wwise_key_to_godot_path[p_key], p_default_value);
}

void WwiseSettings::apply_options_to_wwise()
{
	for (const KeyValue<StringName, uint32_t>& E : setting_key_map)
	{
		StringName godot_path = E.key;
		uint32_t wwise_key = E.value;
		String wwise_type = setting_type_map[godot_path];

		Variant value = get_setting(godot_path);
		AKRESULT res = AK_Success;

		if (wwise_type == "Boolean" || wwise_type == "Integer" || wwise_type == "ChannelConfig")
		{
			res = AK::Option::SetI(wwise_key, (int64_t)value);
		}
		else if (wwise_type == "Real")
		{
			res = AK::Option::SetF(wwise_key, (double)value);
		}
		else if (wwise_type == "String")
		{
			String str_val = value;
			res = AK::Option::SetS(wwise_key, str_val.utf8().get_data());
		}

		if (res != AK_Success)
		{
			UtilityFunctions::printerr(vformat("WwiseGodot: Failed to set Wwise Option for %s. AKRESULT: %s.",
					godot_path, wwise_error_string(res)));
		}
	}
}

void WwiseSettings::add_setting(const StringName& name, const Variant& default_value, Variant::Type type,
		PropertyHint hint, const StringName& hint_string, Visibility visibility)
{
	defined_settings.insert(name);

	Dictionary setting;
	setting["name"] = name;
	setting["type"] = type;
	setting["hint"] = hint;
	setting["hint_string"] = hint_string;

	godot::ProjectSettings* project_settings = godot::ProjectSettings::get_singleton();

	if (!project_settings->has_setting(name))
	{
		project_settings->set_setting(name, default_value);
	}

	project_settings->add_property_info(setting);
	project_settings->set_initial_value(name, default_value);

	switch (visibility)
	{
		case Visibility::DEFAULT:
			project_settings->set_as_basic(name, true);
			project_settings->set_as_internal(name, false);
			break;

		case Visibility::ADVANCED:
			project_settings->set_as_basic(name, false);
			project_settings->set_as_internal(name, false);
			break;

		case Visibility::HIDDEN:
			project_settings->set_as_basic(name, false);
			project_settings->set_as_internal(name, true);
			break;
	}
}

void WwiseSettings::remove_undefined_settings()
{
	godot::ProjectSettings* project_settings = godot::ProjectSettings::get_singleton();

	TypedArray<Dictionary> props = project_settings->get_property_list();

	for (int i = 0; i < props.size(); i++)
	{
		Dictionary prop = props[i];
		String name = prop["name"];

		if (name.begins_with("wwise/"))
		{
			StringName base_name = name;

			int dot_pos = name.find(".");
			if (dot_pos != -1)
			{
				base_name = name.substr(0, dot_pos);
			}

			if (!defined_settings.has(base_name))
			{
				UtilityFunctions::print(vformat("WwiseGodot: Removing obsolete setting: %s", name));
				project_settings->set_setting(name, Variant());
			}
		}
	}
}

void WwiseSettings::load_options_from_json(const String& p_json_path, const String& p_platform_tag)
{
	Ref<FileAccess> file = FileAccess::open(p_json_path, FileAccess::READ);
	if (file.is_null())
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: Failed to open Wwise options JSON at: %s", p_json_path));
		return;
	}

	String json_string = file->get_as_text();
	Ref<JSON> json;
	json.instantiate();

	godot::Error err = json->parse(json_string);
	if (err != OK)
	{
		UtilityFunctions::printerr(vformat("WwiseGodot Failed to parse JSON: %s", p_json_path));
		return;
	}

	Dictionary root = json->get_data();
	Array namespaces = root["Namespaces"];

	for (int i = 0; i < namespaces.size(); i++)
	{
		Dictionary ns = namespaces[i];
		String module_name = ns["DisplayName"];
		module_name = module_name.to_snake_case();
		Array options = ns["Options"];

		for (int j = 0; j < options.size(); j++)
		{
			Dictionary opt = options[j];
			String option_name = opt["Name"];
			option_name = option_name.to_snake_case();
			String wwise_type = opt["Type"];

			if (wwise_type == "Pointer")
				continue;

			Variant default_val = opt.get("DefaultValue", Variant());
			String godot_setting_path = "wwise/" + module_name + "/" + option_name;

			bool has_key = opt.has("Key");

			if (has_key && setting_key_map.has(godot_setting_path) && !p_platform_tag.is_empty())
			{
				String override_path = godot_setting_path + "." + p_platform_tag;
				godot::ProjectSettings::get_singleton()->set_setting(override_path, default_val);
				continue;
			}

			Variant::Type godot_type = Variant::Type::NIL;
			PropertyHint hint = PROPERTY_HINT_NONE;
			String hint_string = "";

			if (wwise_type == "Boolean")
			{
				godot_type = Variant::Type::BOOL;
			}
			else if (wwise_type == "Integer" || wwise_type == "ChannelConfig")
			{
				godot_type = Variant::Type::INT;
				default_val = (int)default_val;
			}
			else if (wwise_type == "Real")
			{
				godot_type = Variant::Type::FLOAT;
			}
			else if (wwise_type == "String")
			{
				godot_type = Variant::Type::STRING;

				if (default_val.get_type() == Variant::Type::NIL)
				{
					continue;
				}
			}

			if (has_key)
			{
				uint32_t key = opt["Key"];
				setting_key_map[godot_setting_path] = key;
				setting_type_map[godot_setting_path] = wwise_type;
				wwise_key_to_godot_path[key] = godot_setting_path;

				if (key == AkOption_Acoustics::AkOption_Acoustics_Enable)
				{
					default_val = true;
				}
			}

			if (opt.has("AllowedValues"))
			{
				Array allowed_values = opt["AllowedValues"];
				PackedStringArray enum_strings;
				for (int k = 0; k < allowed_values.size(); k++)
				{
					Dictionary val_dict = allowed_values[k];
					enum_strings.append(val_dict["Value"]);
				}
				hint = PROPERTY_HINT_ENUM;
				hint_string = String(",").join(enum_strings);
			}
			else if (opt.has("MinValue") && opt.has("MaxValue"))
			{
				hint = PROPERTY_HINT_RANGE;

				if (godot_type == Variant::Type::INT)
				{
					Variant min_val = (int)opt["MinValue"];
					Variant max_val = (int)opt["MaxValue"];

					hint_string = String(min_val) + "," + String(max_val) + "," + "1" + "," + "prefer_slider";
				}
				else if (godot_type == Variant::Type::FLOAT)
				{
					Variant min_val = opt["MinValue"];
					Variant max_val = opt["MaxValue"];

					hint_string = String(min_val) + "," + String(max_val);
				}
			}

			if (opt.has("GodotHint"))
			{
				String godot_hint = opt["GodotHint"];
				if (godot_hint == "Dir")
				{
					hint = PROPERTY_HINT_DIR;
				}
			}

			String visibility_str = opt.get("Visibility", "Default");
			Visibility visibility = Visibility::DEFAULT;

			if (visibility_str == "Advanced")
			{
				visibility = Visibility::ADVANCED;
			}
			else if (visibility_str == "Hidden")
			{
				visibility = Visibility::HIDDEN;
			}

			add_setting(godot_setting_path, default_val, godot_type, hint, hint_string, visibility);
		}
	}
}

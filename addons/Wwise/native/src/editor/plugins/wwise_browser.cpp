#include "wwise_browser.h"

void WwiseBrowser::_bind_methods() { ClassDB::bind_method(D_METHOD("populate_tree"), &WwiseBrowser::populate_tree); }

void WwiseBrowser::_enter_tree()
{
	BoxContainer* box_container = memnew(BoxContainer);
	box_container->set_anchors_preset(Control::LayoutPreset::PRESET_FULL_RECT);
	box_container->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	box_container->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	const float ed_scale = EditorInterface::get_singleton()->get_editor_scale();
	box_container->set_custom_minimum_size(Size2(0, 300) * ed_scale);
	VBoxContainer* parent_vbox_container = memnew(VBoxContainer);
	parent_vbox_container->set_anchors_preset(Control::LayoutPreset::PRESET_FULL_RECT);
	parent_vbox_container->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	parent_vbox_container->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	box_container->add_child(parent_vbox_container);

	VBoxContainer* top_vbox_container = memnew(VBoxContainer);
	parent_vbox_container->add_child(top_vbox_container);

	HBoxContainer* button_hbox_container = memnew(HBoxContainer);

	Button* refresh_button = memnew(Button);
	refresh_button->set_text("Refresh Project");
	refresh_button->connect("button_up", callable_mp(this, &WwiseBrowser::_on_refresh_project_button_up));
	button_hbox_container->add_child(refresh_button);

	Button* generate_soundbanks_button = memnew(Button);
	generate_soundbanks_button->set_text("Generate SoundBanks");
	generate_soundbanks_button->connect(
			"button_up", callable_mp(this, &WwiseBrowser::_on_generate_soundbanks_button_up));
	button_hbox_container->add_child(generate_soundbanks_button);

	Button* generate_wwise_ids_button = memnew(Button);
	generate_wwise_ids_button->set_text("Generate Wwise IDs");
	generate_wwise_ids_button->connect("button_up", callable_mp(this, &WwiseBrowser::_on_generate_ids_button_up));
	button_hbox_container->add_child(generate_wwise_ids_button);

	top_vbox_container->add_child(button_hbox_container);

	LineEdit* search_text = memnew(LineEdit);
	search_text->set_placeholder("Search...");
	search_text->set_caret_blink_enabled(true);
	search_text->set_caret_blink_interval(0.5f);
	top_vbox_container->add_child(search_text);

	tree = memnew(AkWwiseTree);
	tree->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	tree->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	tree->set_allow_reselect(true);
	parent_vbox_container->add_child(tree);
	search_text->connect("text_changed", Callable(tree, "_on_search_text_changed").bind(true));

	WwiseProjectInfo::get_singleton()->connect("ws_refresh_completed", callable_mp(this, &WwiseBrowser::populate_tree));

	add_control_to_bottom_panel(box_container, "Wwise Browser");
}

void WwiseBrowser::_exit_tree() {}

bool WwiseBrowser::ensure_wwise_connection()
{
#if defined(AK_WIN) || defined(AK_MAC_OS_X)
	auto* waapi = Waapi::get_singleton();
	if (!waapi->is_client_connected())
	{
		auto* settings = WwiseSettings::get_singleton();
		auto port = settings->get_setting(settings->communication_settings.waapi_port);
		bool connection_result = waapi->connect_client("127.0.0.1", port);
		return connection_result;
	}
	return true;
#else
	return false;
#endif
}

bool WwiseBrowser::generate_soundbanks()
{
#if defined(AK_WIN) || defined(AK_MAC_OS_X)
	auto* waapi = Waapi::get_singleton();

	Dictionary args;
	args["command"] = "GenerateAllSoundbanksAllPlatformsAutoClose";
	Dictionary options;

	Dictionary response = waapi->client_call_with_timeout(
			"ak.wwise.ui.commands.execute", JSON::stringify(args), JSON::stringify(options), 120000);

	String json_document_str = response.get("result_string", "");
	bool call_result = response.get("call_result", false);

	JSON* json = memnew(JSON);
	godot::Error parse_error = json->parse(json_document_str);

	bool success = parse_error == OK && call_result;

	if (!success)
	{
		UtilityFunctions::push_error("WwiseGodot: SoundBank generation failed.");
	}

	memdelete(json);
	return success;
#else
	return false;
#endif
}

void WwiseBrowser::generate_ids(const String& class_type, const Array& p_parent)
{
	output += "class " + class_type + ":\n";

	if (p_parent.is_empty())
	{
		output += "\tpass\n\n";
	}
	else
	{
		output += "\n";
		process_children(p_parent, 1);
		output += "\n";
	}
}

void WwiseBrowser::process_children(const Array& p_parent, int level)
{
	String indent = String("\t").repeat(level);

	for (int64_t i = 0; i < p_parent.size(); i++)
	{
		Ref<WwiseTreeObject> item = p_parent[i];
		if (item.is_valid())
		{
			int type = item->get_type();
			String name = sanitize_name(item->get_name().to_upper());
			String short_id = itos(item->get_id());

			if (type == WwiseObjectType::StateGroup)
			{
				output += indent + "class " + name + ":\n";
				output += indent + "\tconst GROUP : int = " + short_id + "\n";
				output += indent + "\n";
				output += indent + "\tclass STATE:\n";
				process_children(item->get_children(), level);
				output += "\n";
			}
			else if (type == WwiseObjectType::SwitchGroup)
			{
				output += indent + "class " + name + ":\n";
				output += indent + "\tconst GROUP : int = " + short_id + "\n";
				output += indent + "\n";
				Array children = item->get_children();
				if (children.size() > 0)
				{
					output += indent + "\tclass SWITCH:\n";
					process_children(children, level);
					output += "\n";
				}
			}
			else if (type == WwiseObjectType::State || type == WwiseObjectType::Switch)
			{
				output += indent + "\t\tconst " + name + " : int = " + short_id + "\n";
			}
			else if (type == WwiseObjectType::Event || type == WwiseObjectType::GameParameter ||
					type == WwiseObjectType::Trigger || type == WwiseObjectType::Soundbank ||
					type == WwiseObjectType::AuxBus || type == WwiseObjectType::AcousticTexture)
			{
				output += indent + "const " + name + " : int = " + short_id + "\n";
			}
			else
			{
				process_children(item->get_children(), level);
			}
		}
	}
}

void WwiseBrowser::populate_tree()
{
	if (data_source)
		memdelete(data_source);

	data_source = memnew(AkWwiseTreeDataSource);
	data_source->init_browser_tree(tree);
}

String WwiseBrowser::sanitize_name(const String& p_name)
{
	String sanitized = p_name.replace("-", "_").replace(" ", "_");

	if (sanitized.is_valid_identifier() == false || sanitized.left(1).is_valid_int())
	{
		sanitized = "_" + sanitized;
	}

	return sanitized;
}

bool WwiseBrowser::has_aux_busses(const Array& p_bus_array)
{
	for (int i = 0; i < p_bus_array.size(); ++i)
	{
		Ref<WwiseTreeObject> bus = p_bus_array[i];
		if (!bus.is_valid())
		{
			continue;
		}

		if (bus->get_type() == (int)WwiseObjectType::AuxBus)
		{
			return true;
		}

		Array children = bus->get_children();
		if (has_aux_busses(children))
		{
			return true;
		}
	}

	return false;
}

void WwiseBrowser::_on_refresh_project_button_up() { WwiseProjectInfo::get_singleton()->refresh(); }

void WwiseBrowser::_on_generate_soundbanks_button_up()
{
	if (!ensure_wwise_connection())
	{
		UtilityFunctions::push_error(
				"WwiseGodot: Failed to connect to Wwise Authoring Application. Retry or update the WAMP Port in Wwise "
				"Authoring User Preferences and Wwise-Godot Project Settings.");
		return;
	}

	if (generate_soundbanks())
	{
		UtilityFunctions::print("WwiseGodot: SoundBanks generated successfully.");
	}
}

void WwiseBrowser::_on_generate_ids_button_up()
{
	output = "class_name AK";
	output += "\n\n";

	generate_ids("EVENTS", WwiseProjectInfo::get_singleton()->get_data()->get_event_root());
	generate_ids("STATES", WwiseProjectInfo::get_singleton()->get_data()->get_state_root());
	generate_ids("SWITCHES", WwiseProjectInfo::get_singleton()->get_data()->get_switch_root());
	generate_ids("GAME_PARAMETERS", WwiseProjectInfo::get_singleton()->get_data()->get_game_parameter_root());
	generate_ids("TRIGGERS", WwiseProjectInfo::get_singleton()->get_data()->get_trigger_root());
	generate_ids("BANKS", WwiseProjectInfo::get_singleton()->get_data()->get_bank_root());

	Array bus_array = WwiseProjectInfo::get_singleton()->get_data()->get_bus_root();

	if (has_aux_busses(bus_array))
	{
		generate_ids("AUX_BUSSES", bus_array);
	}
	else
	{
		generate_ids("AUX_BUSSES", Array());
	}

	generate_ids("ACOUSTIC_TEXTURES", WwiseProjectInfo::get_singleton()->get_data()->get_acoustic_texture_root());

	String wwise_ids_path = AkEditorSettings::get_wwise_ids_path();
	bool file_exists = FileAccess::file_exists(wwise_ids_path);

	if (!file_exists)
	{
		auto directory_path = wwise_ids_path.get_base_dir();
		make_dir_recursive(directory_path);
		Ref<FileAccess> wwise_ids_gd_file = FileAccess::open(wwise_ids_path, FileAccess::WRITE);
		wwise_ids_gd_file->close();
		EditorFileSystem* filesystem = get_editor_interface()->get_resource_filesystem();
		filesystem->update_file(wwise_ids_path);
	}

	Ref<Script> script =
			ResourceLoader::get_singleton()->load(wwise_ids_path, "Script", ResourceLoader::CACHE_MODE_REPLACE);
	script->set_source_code(output);
	ResourceSaver::get_singleton()->save(script, wwise_ids_path);
	EditorFileSystem* filesystem = get_editor_interface()->get_resource_filesystem();
	filesystem->update_file(wwise_ids_path);
	filesystem->scan();
	UtilityFunctions::print(vformat("WwiseGodot: Generated IDs at %s", wwise_ids_path));
}
#include "ak_waapi_picker.h"

void AkWaapiPicker::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("_on_resized_editor_viewport"), &AkWaapiPicker::_on_resized_editor_viewport);
	ClassDB::bind_method(
			D_METHOD("_on_visibility_changed_editor_viewport"), &AkWaapiPicker::_on_visibility_changed_editor_viewport);
	ClassDB::bind_method(D_METHOD("_on_refresh_project_button_up"), &AkWaapiPicker::_on_refresh_project_button_up);
	ClassDB::bind_method(D_METHOD("_on_export_soundbanks_button_up"), &AkWaapiPicker::_on_export_soundbanks_button_up);
	ClassDB::bind_method(D_METHOD("_on_generate_ids_button_up"), &AkWaapiPicker::_on_generate_ids_button_up);
	ClassDB::bind_method(
			D_METHOD("_on_file_dialog_file_selected", "path"), &AkWaapiPicker::_on_file_dialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_search_text_changed", "text_filter"), &AkWaapiPicker::_on_search_text_changed);

	ADD_SIGNAL(MethodInfo("connection_changed", PropertyInfo(Variant::BOOL, "result")));
}

void AkWaapiPicker::create_project_objects_tree()
{
	Tree* tree = picker_data.scene_data.objects_tree;

	if (!tree && !tree->get_root())
		return;

	tree->clear();

	TreeItem* root = tree->create_item();

	root->set_text(0, "Wwise Project");
	root->set_icon(0, picker_data.icon_data.project);

	TreeItem* events = tree->create_item(root);
	events->set_text(0, "Events");
	events->set_icon(0, picker_data.icon_data.folder);

	TreeItem* switches = tree->create_item(root);
	switches->set_text(0, "Switches");
	switches->set_icon(0, picker_data.icon_data.folder);

	TreeItem* states = tree->create_item(root);
	states->set_text(0, "States");
	states->set_icon(0, picker_data.icon_data.folder);

	TreeItem* soundbanks = tree->create_item(root);
	soundbanks->set_text(0, "Soundbanks");
	soundbanks->set_icon(0, picker_data.icon_data.folder);

	TreeItem* aux_busses = tree->create_item(root);
	aux_busses->set_text(0, "Auxiliary Busses");
	aux_busses->set_icon(0, picker_data.icon_data.folder);

	TreeItem* virtual_acoustics = tree->create_item(root);
	virtual_acoustics->set_text(0, "Virtual Acoustics");
	virtual_acoustics->set_icon(0, picker_data.icon_data.folder);

	if (parse_result != Error::OK)
		return;

	Variant data = json->get_data();
	Array data_array = data.get("return");

	for (int64_t i = 0; i < data_array.size(); i++)
	{
		Dictionary object = data_array[i];
		if (object["type"] == String("Project"))
		{
			root->set_text(0, object["name"]);
			break;
		}
	}

	for (int64_t i = 0; i < data_array.size(); i++)
	{
		Dictionary object = data_array[i];

		if (object["type"] == String("WorkUnit"))
		{
			TreeItem* item{};

			String path = object["path"];

			if (path.contains("\\Events\\"))
			{
				item = tree->create_item(events);
			}
			else if (path.contains("\\Switches\\"))
			{
				item = tree->create_item(switches);
			}
			else if (path.contains("\\States\\"))
			{
				item = tree->create_item(states);
			}
			else if (path.contains("\\SoundBanks\\"))
			{
				item = tree->create_item(soundbanks);
			}
			else if (path.contains("\\Master-Mixer Hierarchy\\"))
			{
				item = tree->create_item(aux_busses);
			}
			else if (path.contains("\\Virtual Acoustics\\"))
			{
				item = tree->create_item(virtual_acoustics);
			}

			if (item)
			{
				item->set_text(0, object["name"]);
				item->set_icon(0, picker_data.icon_data.work_unit);
			}
		}
	}

	int num_switch_groups{}, num_state_groups{}, num_bus_groups{};

	for (int64_t i = 0; i < data_array.size(); i++)
	{
		Dictionary object = data_array[i];

		TreeItem* item{};

		String type = object["type"];
		String path = object["path"];
		String name = object["name"];
		int short_id = object["shortId"];

		if (type == "SwitchGroup")
		{
			TreeItem* work_unit = switches->get_first_child();

			while (work_unit)
			{
				if (path.contains(work_unit->get_text(0)))
				{
					if (text_filter.is_empty() || name.findn(text_filter) != -1)
					{
						item = tree->create_item(work_unit);
						item->set_meta("short_id", short_id);
						item->set_icon(0, picker_data.icon_data.switch_group);
						num_switch_groups += 1;
					}
					break;
				}
				work_unit = work_unit->get_next();
			}
		}
		else if (type == "StateGroup")
		{
			TreeItem* work_unit = states->get_first_child();

			while (work_unit)
			{
				if (path.contains(work_unit->get_text(0)))
				{
					if (text_filter.is_empty() || name.findn(text_filter) != -1)
					{
						item = tree->create_item(work_unit);
						item->set_meta("short_id", short_id);
						item->set_icon(0, picker_data.icon_data.state_group);
						num_state_groups += 1;
					}
					break;
				}
				work_unit = work_unit->get_next();
			}
		}
		else if (type == "Bus")
		{
			TreeItem* work_unit = aux_busses->get_first_child();

			while (work_unit)
			{
				if (path.contains(work_unit->get_text(0)))
				{
					if (text_filter.is_empty() || name.findn(text_filter) != -1)
					{
						item = tree->create_item(work_unit);
						item->set_meta("short_id", short_id);
						// todo(alex): why bus and not aux_bux icon?
						item->set_icon(0, picker_data.icon_data.bus);
						num_bus_groups += 1;
					}
					break;
				}
				work_unit = work_unit->get_next();
			}
		}
		if (item)
		{
			item->set_text(0, name);
		}
	}

	int num_switches{}, num_states{}, num_aux_busses{}, num_events{}, num_soundbanks{}, num_acoustic_textures{};

	for (int64_t i = 0; i < data_array.size(); i++)
	{
		Dictionary object = data_array[i];

		TreeItem* item{};

		String type = object["type"];
		String path = object["path"];
		String name = object["name"];
		int short_id = object["shortId"];

		if (type == "Switch")
		{
			TreeItem* work_unit = switches->get_first_child();

			while (work_unit)
			{
				if (path.contains(work_unit->get_text(0)))
				{
					TreeItem* switch_group = work_unit->get_first_child();

					while (switch_group)
					{
						if (path.contains(switch_group->get_text(0)))
						{
							if (text_filter.is_empty() || name.findn(text_filter) != -1)
							{
								item = tree->create_item(switch_group);
								item->set_icon(0, picker_data.icon_data.switch_);
								item->set_meta("type", "switch");
								item->set_meta("switch_group_id", switch_group->get_meta("short_id"));
								item->set_meta("switch_group_name", switch_group->get_text(0));
								item->set_meta("switch_value_id", short_id);
								num_switches += 1;
							}
							break;
						}
						switch_group = switch_group->get_next();
					}
					break;
				}
				work_unit = work_unit->get_next();
			}
		}
		else if (type == "State")
		{
			TreeItem* work_unit = states->get_first_child();

			while (work_unit)
			{
				if (path.contains(work_unit->get_text(0)))
				{
					TreeItem* state_group = work_unit->get_first_child();

					while (state_group)
					{
						if (path.contains(state_group->get_text(0)))
						{
							if (text_filter.is_empty() || name.findn(text_filter) != -1)
							{
								item = tree->create_item(state_group);
								item->set_icon(0, picker_data.icon_data.state);
								item->set_meta("type", "state");
								item->set_meta("state_group_id", state_group->get_meta("short_id"));
								item->set_meta("state_group_name", state_group->get_text(0));
								item->set_meta("state_value_id", short_id);
								num_states += 1;
							}
							break;
						}
						state_group = state_group->get_next();
					}
					break;
				}
				work_unit = work_unit->get_next();
			}
		}
		else if (type == "AuxBus")
		{
			TreeItem* work_unit = aux_busses->get_first_child();

			while (work_unit)
			{
				if (path.contains(work_unit->get_text(0)))
				{
					TreeItem* bus = work_unit->get_first_child();

					while (bus)
					{
						if (path.contains(bus->get_text(0)))
						{
							if (text_filter.is_empty() || name.findn(text_filter) != -1)
							{
								item = tree->create_item(bus);
								item->set_icon(0, picker_data.icon_data.aux_bus);
								num_aux_busses += 1;
							}
							break;
						}
						bus = bus->get_next();
					}
					break;
				}
				work_unit = work_unit->get_next();
			}
		}
		else if (type == "Event")
		{
			TreeItem* work_unit = events->get_first_child();

			while (work_unit)
			{
				if (path.contains(work_unit->get_text(0)))
				{
					if (text_filter.is_empty() || name.findn(text_filter) != -1)
					{
						item = tree->create_item(work_unit);
						item->set_icon(0, picker_data.icon_data.event);
						item->set_meta("type", "event");
						item->set_meta("short_id", short_id);
						item->set_meta("id", object["id"]);
						num_events += 1;
					}
					break;
				}
				work_unit = work_unit->get_next();
			}
		}
		else if (type == "SoundBank")
		{
			TreeItem* work_unit = soundbanks->get_first_child();

			while (work_unit)
			{
				if (path.contains(work_unit->get_text(0)))
				{
					if (text_filter.is_empty() || name.findn(text_filter) != -1)
					{
						item = tree->create_item(work_unit);
						item->set_icon(0, picker_data.icon_data.soundbank);
						item->set_meta("type", "soundbank");
						item->set_meta("short_id", short_id);
						num_soundbanks += 1;
					}
					break;
				}
				work_unit = work_unit->get_next();
			}
		}
		else if (type == "AcousticTexture")
		{
			TreeItem* work_unit = virtual_acoustics->get_first_child();

			while (work_unit)
			{
				if (path.contains(work_unit->get_text(0)))
				{
					if (text_filter.is_empty() || name.findn(text_filter) != -1)
					{
						item = tree->create_item(work_unit);
						item->set_icon(0, picker_data.icon_data.acoustic_texture);
						item->set_meta("type", "acoustic_texture");
						item->set_meta("short_id", short_id);
						num_acoustic_textures += 1;
					}
					break;
				}
				work_unit = work_unit->get_next();
			}
		}
		if (item)
		{
			item->set_text(0, name);
		}
	}
}

void AkWaapiPicker::generate_ids(const Array& data)
{
	Array event_array;
	Array state_group_array;
	Array state_array;
	Array switch_group_array;
	Array switch_array;
	Array game_param_array;
	Array trigger_array;
	Array soundbank_array;
	Array bus_array;
	Array aux_bus_array;
	Array audio_device_array;
	Array external_src_array;
	Array acoustic_texture_array;

	for (int i = 0; i < data.size(); ++i)
	{
		const String type = data[i].operator godot::Dictionary()["type"];
		if (type == "Event")
		{
			event_array.append(data[i]);
		}
		else if (type == "StateGroup")
		{
			state_group_array.append(data[i]);
		}
		else if (type == "State")
		{
			state_array.append(data[i]);
		}
		else if (type == "SwitchGroup")
		{
			switch_group_array.append(data[i]);
		}
		else if (type == "Switch")
		{
			switch_array.append(data[i]);
		}
		else if (type == "GameParameter")
		{
			game_param_array.append(data[i]);
		}
		else if (type == "Trigger")
		{
			trigger_array.append(data[i]);
		}
		else if (type == "SoundBank")
		{
			soundbank_array.append(data[i]);
		}
		else if (type == "Bus")
		{
			bus_array.append(data[i]);
		}
		else if (type == "AuxBus")
		{
			aux_bus_array.append(data[i]);
		}
		else if (type == "AudioDevice")
		{
			audio_device_array.append(data[i]);
		}
		else if (type == "ExternalSource")
		{
			external_src_array.append(data[i]);
		}
		else if (type == "AcousticTexture")
		{
			acoustic_texture_array.append(data[i]);
		}
	}

	Dictionary init_soundbank;
	init_soundbank["name"] = "Init";
	init_soundbank["shortId"] = 1355168291;
	init_soundbank["type"] = "SoundBank";
	soundbank_array.insert(0, init_soundbank);

	final_text = "class_name AK";
	final_text += "\n\n";

	create_class(event_array, "EVENTS");
	create_state_switch_class(state_group_array, state_array, "STATES", "STATE");
	create_state_switch_class(switch_group_array, switch_array, "SWITCHES", "SWITCH");
	create_class(game_param_array, "GAME_PARAMETERS");
	create_class(trigger_array, "TRIGGERS");
	create_class(soundbank_array, "BANKS");
	create_class(bus_array, "BUSSES");
	create_class(aux_bus_array, "AUX_BUSSES");
	create_class(audio_device_array, "AUDIO_DEVICES");
	create_class(external_src_array, "EXTERNAL_SOURCES");
	create_class(acoustic_texture_array, "ACOUSTIC_TEXTURES");
}

void AkWaapiPicker::create_class(const Array& data, const String& type)
{
	if (data.size() > 0)
	{
		final_text += "class " + type + ":\n\n";

		for (int i = 0; i < data.size(); ++i)
		{
			const String name = data[i].operator godot::Dictionary()["name"];
			const String short_id = data[i].operator godot::Dictionary()["shortId"];

			final_text += "\tconst " + name.to_upper().replace(" ", "_") + ": int = " + short_id + "\n";
		}

		final_text += "\n\tconst _dict: Dictionary = {\n";

		for (int i = 0; i < data.size(); ++i)
		{
			const String name = data[i].operator godot::Dictionary()["name"];

			final_text +=
					"\t\t\"" + name + "\": " + name.to_upper().replace(" ", "_") + (i < data.size() - 1 ? ",\n" : "\n");
		}

		final_text += "\t}\n\n";
	}
	else
	{
		create_empty_class(type);
	}
}

void AkWaapiPicker::create_state_switch_class(
		const Array& parent_array, const Array& child_array, const String& parent_type, const String& child_type)
{
	if (parent_array.size() > 0)
	{
		final_text += "class " + parent_type + ":\n\n";

		for (int i = 0; i < parent_array.size(); ++i)
		{
			const String name = parent_array[i].operator godot::Dictionary()["name"];
			const String short_id = parent_array[i].operator godot::Dictionary()["shortId"];

			final_text += "\tclass " + name.to_upper().replace(" ", "_") + ":\n";
			final_text += "\t\tconst GROUP: int = " + short_id + "\n\n";

			final_text += "\t\tclass " + child_type + ":\n";
			for (int j = 0; j < child_array.size(); ++j)
			{
				if (child_array[j].get("parent.name") == parent_array[i].get("name"))
				{
					const String name = child_array[j].operator godot::Dictionary()["name"];
					const String short_id = child_array[j].operator godot::Dictionary()["shortId"];

					final_text += "\t\t\tconst " + name.to_upper().replace(" ", "_") + ": int = " + short_id + "\n";
				}
			}
			final_text += "\n";
		}

		final_text += "\tconst _dict: Dictionary = {\n";

		for (int i = 0; i < parent_array.size(); ++i)
		{
			const String name = parent_array[i].operator godot::Dictionary()["name"];
			const String short_id = parent_array[i].operator godot::Dictionary()["shortId"];

			final_text += "\t\t\"" + name + "\": {\n";
			final_text += "\t\t\t\"GROUP\": " + short_id + ",\n";
			final_text += "\t\t\t\"" + child_type + "\": {\n";
			for (int j = 0; j < child_array.size(); ++j)
			{
				if (child_array[j].get("parent.name") == parent_array[i].get("name"))
				{
					const String name = child_array[j].operator godot::Dictionary()["name"];
					const String short_id = child_array[j].operator godot::Dictionary()["shortId"];

					final_text += "\t\t\t\t\"" + name + "\": " + short_id + (j < child_array.size() - 1 ? ",\n" : "\n");
				}
			}
			final_text += "\t\t\t}\n";
			final_text += (i < parent_array.size() - 1 ? "\t\t},\n" : "\t\t}\n");
		}

		final_text += "\t}\n\n";
	}
	else
	{
		create_empty_class(parent_type);
	}
}

void AkWaapiPicker::create_empty_class(const String& type)
{
	final_text += "class " + type + ":\n\n";
	final_text += "\tconst _dict: Dictionary = {}\n\n";
}

void AkWaapiPicker::_enter_tree()
{
	json = memnew(JSON);

	AkEditorUtils* editor_utils = AkEditorUtils::get_singleton();
	using IconType = AkEditorUtils::AkEditorIconType;

	WaapiPickerIconData icon_data{ editor_utils->get_editor_icon(IconType::AK_ICON_PROJECT),
		editor_utils->get_editor_icon(IconType::AK_ICON_FOLDER), editor_utils->get_editor_icon(IconType::AK_ICON_EVENT),
		editor_utils->get_editor_icon(IconType::AK_ICON_SWITCHGROUP),
		editor_utils->get_editor_icon(IconType::AK_ICON_SWITCH),
		editor_utils->get_editor_icon(IconType::AK_ICON_STATEGROUP),
		editor_utils->get_editor_icon(IconType::AK_ICON_STATE),
		editor_utils->get_editor_icon(IconType::AK_ICON_SOUNDBANK),
		editor_utils->get_editor_icon(IconType::AK_ICON_BUS), editor_utils->get_editor_icon(IconType::AK_ICON_AUXBUS),
		editor_utils->get_editor_icon(IconType::AK_ICON_ACOUSTICTEXTURE),
		editor_utils->get_editor_icon(IconType::AK_ICON_WORKUNIT) };

	picker_data.icon_data = icon_data;

	scene = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/waapi_picker/waapi_picker.tscn");
	picker_data.scene_data.control = Object::cast_to<Control>(scene->instantiate(PackedScene::GEN_EDIT_STATE_INSTANCE));
	picker_data.scene_data.parent_vbox_container =
			picker_data.scene_data.control->get_node<VBoxContainer>("ParentVBoxContainer");
	picker_data.scene_data.refresh_project_button = picker_data.scene_data.control->get_node<Button>(
			"ParentVBoxContainer/TopVBoxContainer/ButtonsHContainer/RefreshProjectButton");
	picker_data.scene_data.export_soundbanks_button = picker_data.scene_data.control->get_node<Button>(
			"ParentVBoxContainer/TopVBoxContainer/ButtonsHContainer/ExportSoundbanksButton");
	picker_data.scene_data.generate_ids_button = picker_data.scene_data.control->get_node<Button>(
			"ParentVBoxContainer/TopVBoxContainer/ButtonsHContainer/GenerateIdsButton");
	picker_data.scene_data.search_text =
			picker_data.scene_data.control->get_node<LineEdit>("ParentVBoxContainer/TopVBoxContainer/SearchText");
	picker_data.scene_data.objects_tree =
			picker_data.scene_data.control->get_node<Tree>("ParentVBoxContainer/ProjectObjectsTree");

	add_control_to_bottom_panel(picker_data.scene_data.control, "WAAPI Picker");

	editor_viewport = get_editor_interface()->get_base_control()->get_viewport();

	picker_data.scene_data.control->set_custom_minimum_size(
			Vector2(picker_data.scene_data.control->get_custom_minimum_size().x,
					editor_viewport->get_visible_rect().get_size().y * 0.3f));

	Error error =
			editor_viewport->connect("size_changed", callable_mp(this, &AkWaapiPicker::_on_resized_editor_viewport));
	AKASSERT(error == Error::OK);
	error = editor_viewport->connect(
			"visibility_changed", callable_mp(this, &AkWaapiPicker::_on_visibility_changed_editor_viewport));
	AKASSERT(error == Error::OK);
	error = picker_data.scene_data.refresh_project_button->connect(
			"button_up", callable_mp(this, &AkWaapiPicker::_on_refresh_project_button_up));
	AKASSERT(error == Error::OK);
	error = picker_data.scene_data.export_soundbanks_button->connect(
			"button_up", callable_mp(this, &AkWaapiPicker::_on_export_soundbanks_button_up));
	AKASSERT(error == Error::OK);
	error = picker_data.scene_data.generate_ids_button->connect(
			"button_up", callable_mp(this, &AkWaapiPicker::_on_generate_ids_button_up));
	AKASSERT(error == Error::OK);
	error = picker_data.scene_data.search_text->connect(
			"text_changed", callable_mp(this, &AkWaapiPicker::_on_search_text_changed));
	AKASSERT(error == Error::OK);

	auto port = ProjectSettings::get_singleton()->get_setting("wwise/communication_settings/waapi_port", 8080);
	Waapi::get_singleton()->connect_client("127.0.0.1", port);

	_on_refresh_project_button_up();
}

void AkWaapiPicker::_process(double delta)
{
	if (Waapi::get_singleton()->is_client_connected())
	{
		Waapi::get_singleton()->process_callbacks();
	}
}

void AkWaapiPicker::_exit_tree()
{
	Waapi* waapi = Waapi::get_singleton();

	if (waapi)
	{
		if (waapi->is_client_connected())
		{
			waapi->disconnect_client();
			if (picker_data.scene_data.control)
			{
				remove_control_from_bottom_panel(picker_data.scene_data.control);
				memdelete(json);
			}
		}
	}
}

void AkWaapiPicker::_on_resized_editor_viewport()
{
	if (editor_viewport)
	{
		if (picker_data.scene_data.control)
		{
			if (picker_data.scene_data.control->get_parent())
			{
				VBoxContainer* parent = Object::cast_to<VBoxContainer>(picker_data.scene_data.control->get_parent());
				Vector2 size{ parent->get_size().x - 10, parent->get_size().y - 80 };
				parent->set_size(size);
			}
		}
	}
}

void AkWaapiPicker::_on_visibility_changed_editor_viewport()
{
	if (editor_viewport)
	{
		real_t width = editor_viewport->get_visible_rect().get_size().x - 6;
		real_t height = 0.0f;

		if (is_showing_viewport)
		{
			height = get_editor_interface()->get_base_control()->get_size().y - 110;
			is_showing_viewport = false;
		}
		else
		{
			height = get_editor_interface()->get_base_control()->get_size().y -
					editor_viewport->get_visible_rect().get_size().y - 150.0f;
			is_showing_viewport = true;
		}

		VBoxContainer* parent = Object::cast_to<VBoxContainer>(picker_data.scene_data.control->get_parent());
		parent->set_size(Vector2(width, height));
	}
}

void AkWaapiPicker::_on_refresh_project_button_up()
{
	bool connect_result{};
	if (!Waapi::get_singleton()->is_client_connected())
	{
		auto port = ProjectSettings::get_singleton()->get_setting("wwise/communication_settings/waapi_port", 8080);
		connect_result = Waapi::get_singleton()->connect_client("127.0.0.1", port);
		emit_signal("connection_changed", connect_result);
	}
	else
	{
		connect_result = true;
	}

	if (connect_result)
	{
		Dictionary args;
		Dictionary of_type;
		Array type_array = Array::make("Project", "SwitchGroup", "StateGroup", "Bus", "Switch", "State", "AuxBus",
				"Event", "SoundBank", "AcousticTexture", "WorkUnit");

		of_type["ofType"] = type_array;
		args["from"] = of_type;

		Dictionary options;
		Array returnArray = Array::make("id", "name", "type", "workunit", "path", "shortId");

		options["return"] = returnArray;

		Dictionary client_call_dict = Waapi::get_singleton()->client_call(
				"ak.wwise.core.object.get", JSON::stringify(args), JSON::stringify(options));
		parse_result = json->parse(client_call_dict["result_string"]);

		if (parse_result == Error::OK)
		{
			if (json->get_data().has_key("return"))
			{
				create_project_objects_tree();
			}
		}
	}
}

void AkWaapiPicker::_on_export_soundbanks_button_up()
{
	bool connect_result{};
	if (!Waapi::get_singleton()->is_client_connected())
	{
		auto port = ProjectSettings::get_singleton()->get_setting("wwise/communication_settings/waapi_port", 8080);
		connect_result = Waapi::get_singleton()->connect_client("127.0.0.1", port);
		emit_signal("connection_changed", connect_result);
	}
	else
	{
		connect_result = true;
	}

	if (connect_result)
	{
		Dictionary args;
		args["command"] = "GenerateAllSoundbanksAllPlatformsAutoClose";
		Dictionary options;

		Dictionary dict = Waapi::get_singleton()->client_call(
				"ak.wwise.ui.commands.execute", JSON::stringify(args), JSON::stringify(options));

		String json_document_str = dict["result_string"];
		bool call_result = dict["call_result"];

		Error error = json->parse(json_document_str);

		if (error == OK && call_result)
		{
			UtilityFunctions::print("WwiseGodot: Soundbanks generated successfully.");
		}
		else
		{
			UtilityFunctions::print("WwiseGodot: Error during soundbank generation with result " +
					String(json->get_data().operator godot::Dictionary()["result"]));
		}
	}
}

void AkWaapiPicker::_on_generate_ids_button_up()
{
	file_dialog = memnew(FileDialog);
	file_dialog->set_file_mode(FileDialog::FILE_MODE_SAVE_FILE);
	file_dialog->set_access(FileDialog::ACCESS_RESOURCES);

	String file_type = "*.gd ; GDScript Files";
	PackedStringArray filters;
	filters.append(file_type);
	file_dialog->set_filters(filters);
	file_dialog->connect("file_selected", callable_mp(this, &AkWaapiPicker::_on_file_dialog_file_selected));

	EditorInterface* editor_interface = get_editor_interface();
	Control* base_control = editor_interface->get_base_control();
	base_control->add_child(file_dialog);
	file_dialog->popup_centered_ratio();
}

void AkWaapiPicker::_on_file_dialog_file_selected(const String& path)
{
	bool connect_result{};
	if (!Waapi::get_singleton()->is_client_connected())
	{
		auto port = ProjectSettings::get_singleton()->get_setting("wwise/communication_settings/waapi_port", 8080);
		connect_result = Waapi::get_singleton()->connect_client("127.0.0.1", port);
	}
	else
	{
		connect_result = true;
	}

	if (connect_result)
	{
		Dictionary args;
		Dictionary of_type;
		Array type_array = Array::make("Event", "StateGroup", "State", "SwitchGroup", "Switch", "GameParameter",
				"Trigger", "SoundBank", "Bus", "AuxBus", "AudioDevice", "ExternalSource", "AcousticTexture");

		of_type["ofType"] = type_array;
		args["from"] = of_type;

		Dictionary options;
		options["return"] = Array::make("name", "type", "shortId", "parent.name");
		Dictionary dict = Waapi::get_singleton()->client_call(
				"ak.wwise.core.object.get", JSON::stringify(args), JSON::stringify(options));
		Error error = json->parse(dict["result_string"]);

		if (error == OK)
		{
			Variant data = json->get_data();
			Array data_arr = data.get("return");
			generate_ids(data_arr);
		}
	}
	else
	{
		UtilityFunctions::print("WwiseGodot: Failed to generate Wwise IDs. Wwise Authoring is not launched!");

		if (Waapi::get_singleton()->is_client_connected())
		{
			Waapi::get_singleton()->disconnect_client();
		}

		return;
	}

	bool file_exists = FileAccess::file_exists(path);

	if (!file_exists)
	{
		Ref<FileAccess> wwise_ids_gd_file = FileAccess::open(path, FileAccess::WRITE);
		wwise_ids_gd_file->close();
		EditorFileSystem* filesystem = get_editor_interface()->get_resource_filesystem();
		filesystem->update_file(path);
	}

	Ref<Script> script = ResourceLoader::get_singleton()->load(path, "Script", ResourceLoader::CACHE_MODE_REPLACE);
	script->set_source_code(final_text);
	ResourceSaver::get_singleton()->save(script, path);

	UtilityFunctions::print("WwiseGodot: Generated IDs at " + path);

	file_dialog->queue_free();
	final_text = "";

	if (Waapi::get_singleton()->is_client_connected())
	{
		Waapi::get_singleton()->disconnect_client();
	}
}

void AkWaapiPicker::_on_search_text_changed(const String& text_filter)
{
	this->text_filter = text_filter;
	_on_refresh_project_button_up();
}
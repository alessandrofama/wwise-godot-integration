#include "ak_inspector_plugin.h"

void AkInspectorTree::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("_on_text_changed", "text_filter"), &AkInspectorTree::_on_text_changed);
	ClassDB::bind_method(D_METHOD("_on_size_changed"), &AkInspectorTree::_on_size_changed);
}

Dictionary AkInspectorTree::get_wwise_ids(const AkEditorUtils::AkType ak_type)
{
	Dictionary result{};

	TypedArray<Dictionary> global_class_list = ProjectSettings::get_singleton()->get_global_class_list();

	for (int64_t i = 0; i < global_class_list.size(); i++)
	{
		Dictionary dict = global_class_list[i];

		if (dict.get("class", "") == StringName("AK"))
		{
			String path = dict.get("path", "");

			if (!path.is_empty())
			{
				Ref<Script> script = ResourceLoader::get_singleton()->load(
						path, String(), ResourceLoader::CacheMode::CACHE_MODE_IGNORE);

				Dictionary script_constants = script->get_script_constant_map();

				String type_constant{};

				switch (ak_type)
				{
					case AkEditorUtils::AkType::AKTYPE_EVENT:
					{
						type_constant = "EVENTS";
						break;
					}
					case AkEditorUtils::AkType::AKTYPE_BANK:
					{
						type_constant = "BANKS";
						break;
					}
					case AkEditorUtils::AkType::AKTYPE_RTPC:
					{
						type_constant = "GAME_PARAMETERS";
						break;
					}
					case AkEditorUtils::AkType::AKTYPE_STATE_GROUP:
					{
						type_constant = "STATES";
						break;
					}
					case AkEditorUtils::AkType::AKTYPE_STATE:
					{
						type_constant = "STATES";
						break;
					}
					case AkEditorUtils::AkType::AKTYPE_SWITCH_GROUP:
					{
						type_constant = "SWITCHES";
						break;
					}
					case AkEditorUtils::AkType::AKTYPE_SWITCH:
					{
						type_constant = "SWITCHES";
						break;
					}
					case AkEditorUtils::AkType::AKTYPE_AUX_BUS:
					{
						type_constant = "AUX_BUSSES";
						break;
					}
					case AkEditorUtils::AkType::AKTYPE_ACOUSTIC_TEXTURE:
					{
						type_constant = "ACOUSTIC_TEXTURES";
						break;
					}
					default:
						break;
				}

				Ref<Script> type_script = script_constants.get(type_constant, "");
				if (type_script.is_valid())
				{
					Dictionary type_constants = type_script->get_script_constant_map();
					Dictionary _dict = type_constants.get("_dict", "");

					if (ak_type == AkEditorUtils::AkType::AKTYPE_STATE)
					{
						if (!user_data.is_empty())
						{
							if (_dict.get(user_data.get("name", Variant(nullptr)), Variant(nullptr)).get_type() !=
									Variant::Type::NIL)
							{
								_dict = _dict.get(user_data.get("name", ""), "").get("STATE");
							}
							else
							{
								_dict = Dictionary();
							}
						}
					}
					else if (ak_type == AkEditorUtils::AkType::AKTYPE_SWITCH)
					{
						if (!user_data.is_empty())
						{
							if (_dict.get(user_data.get("name", Variant(nullptr)), Variant(nullptr)).get_type() !=
									Variant::Type::NIL)
							{
								_dict = _dict.get(user_data.get("name", ""), "").get("SWITCH");
							}
							else
							{
								_dict = Dictionary();
							}
						}
					}
					result = _dict;
				}
			}
		}
	}

	return result;
}

void AkInspectorTree::initialize(const AkEditorUtils::AkType item_type, const Dictionary& user_data_)
{
	ak_type = item_type;
	user_data = user_data_;

	window = Object::cast_to<AkInspectorEditor>(get_parent()->get_parent());
	window->connect("size_changed", callable_mp(this, &AkInspectorTree::_on_size_changed));

	search_text = window->search_text;
	search_text->connect("text_changed", callable_mp(this, &AkInspectorTree::_on_text_changed));

	populate_browser("");
}

void AkInspectorTree::populate_browser(const String& text_filter)
{
	if (root_item)
	{
		clear();
	}

	root_item = create_item();
	set_hide_root(true);
	root_item->set_text(0, "Root");

	Dictionary dict = get_wwise_ids(ak_type);

	const Array keys = dict.keys();
	const Array values = dict.values();

	for (int64_t i = 0; i < dict.size(); i++)
	{
		String name = keys[i];
		Variant id = values[i];

		if (name.findn(text_filter) == -1 && !text_filter.is_empty())
		{
			continue;
		}

		TreeItem* item = create_item(root_item);
		AkEditorUtils* editor_utils = AkEditorUtils::get_singleton();

		Ref<Texture2D> icon;

		if (ak_type == AkEditorUtils::AkType::AKTYPE_BANK)
		{
			AkEditorUtils::AkEditorIconType icon_type = (name == "Init")
					? AkEditorUtils::AkEditorIconType::AK_ICON_SOUNDBANK_INIT
					: AkEditorUtils::AkEditorIconType::AK_ICON_SOUNDBANK;
			icon = editor_utils->get_editor_icon(icon_type);
		}
		else
		{
			icon = editor_utils->get_editor_icon(ak_type);
		}

		if ((ak_type == AkEditorUtils::AkType::AKTYPE_STATE_GROUP) ||
				(ak_type == AkEditorUtils::AkType::AKTYPE_SWITCH_GROUP))
		{
			id = values[i].get("GROUP");
		}

		item->set_text(0, name);
		item->set_meta("name", name);
		item->set_meta("id", id);
		item->set_icon(0, icon);
	}
}

void AkInspectorTree::_on_text_changed(const String& text_filter) { populate_browser(text_filter); }

void AkInspectorTree::_on_size_changed() { window->root_vbox->set_size(window->get_size()); }

void AkInspectorEditor::initialize()
{
	set_name("Window");
	set_title("Wwise Browser");
	set_disable_3d(true);
	set_exclusive(true);

	root_vbox = memnew(VBoxContainer);
	root_vbox->set_name("ParentVBoxContainer");

	EditorInterface* editor_interface = EditorInterface::get_singleton();
	double editor_scale = editor_interface->get_editor_scale();
	root_vbox->set_size(Size2(400, 600) * editor_scale);

	root_vbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	root_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	add_child(root_vbox);

	search_vbox = memnew(VBoxContainer);
	search_vbox->set_name("TopVBoxContainer");
	root_vbox->add_child(search_vbox);

	search_text = memnew(LineEdit);
	search_text->set_name("SearchText");
	search_vbox->add_child(search_text);

	tree = memnew(AkInspectorTree);
	tree->set_name("ProjectObjectsTree");
	tree->set_allow_reselect(true);
	tree->set_hide_folding(true);
	tree->set_hide_root(true);
	tree->set_v_size_flags(Control::SIZE_EXPAND_FILL);

	root_vbox->add_child(tree);

	set_ok_button_text("Close");
}

void AkInspectorEditorProperty::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("reset"), &AkInspectorEditorProperty::reset);
	ClassDB::bind_method(D_METHOD("_on_button_pressed"), &AkInspectorEditorProperty::_on_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_item_selected"), &AkInspectorEditorProperty::_on_item_selected);
}

void AkInspectorEditorProperty::init(const AkEditorUtils::AkType type, const Dictionary& user_data_)
{
	ak_type = type;
	user_data = user_data_;

	current_value["name"] = "";
	current_value["id"] = 0;

	property_control = memnew(Button);
	property_control->set_clip_text(true);
	window = memnew(AkInspectorEditor);
	window->initialize();
	add_child(property_control);
	window->connect("close_requested", callable_mp(this, &AkInspectorEditorProperty::reset));
	window->connect("confirmed", callable_mp(this, &AkInspectorEditorProperty::reset));

	AkInspectorTree* tree = window->tree;
	tree->initialize(ak_type, user_data);

	const AkInspectorEditorPropertyInfo info = get_ak_inspector_property_info(ak_type);
	property_control->set_text(info.text);
	tree->search_text->set("placeholder_text", info.placeholder);
	icon = AkEditorUtils::get_singleton()->get_editor_icon(ak_type);

	tree->connect("item_selected", callable_mp(this, &AkInspectorEditorProperty::_on_item_selected));
	property_control->connect("pressed", callable_mp(this, &AkInspectorEditorProperty::_on_button_pressed));
}

void AkInspectorEditorProperty::open_popup()
{
	add_child(window);
	window->tree->populate_browser("");

	EditorInterface* editor_interface = EditorInterface::get_singleton();
	double editor_scale = editor_interface->get_editor_scale();

	Point2i popup_position{};
	popup_position.x = get_global_mouse_position().x - (550.0f * editor_scale);
	popup_position.y = get_global_mouse_position().y - (115.0f * editor_scale);

	window->popup_on_parent(Rect2i(popup_position, Size2i(1, 1)));
}

void AkInspectorEditorProperty::close_popup()
{
	for (int i = 0; i < get_child_count(); i++)
	{
		if (get_child(i)->get_class() == "AkInspectorEditor")
		{
			window->hide();
			remove_child(window);
		}
	}

	window->search_text->set_text("");
}

void AkInspectorEditorProperty::_update_property()
{
	Variant new_value_variant = get_edited_object()->get(get_edited_property());

	if (new_value_variant.get_type() == Variant::NIL)
	{
		close_popup();
		get_edited_object()->notify_property_list_changed();
		return;
	}

	updating = true;
	current_value = new_value_variant;

	if (current_value["name"].get_type() == Variant::NIL)
	{
		return;
	}

	String name = current_value["name"];
	if (!name.is_empty())
	{
		if (ak_type == AkEditorUtils::AkType::AKTYPE_BANK)
		{
			if (name == "Init")
			{
				icon = AkEditorUtils::get_singleton()->get_editor_icon(
						AkEditorUtils::AkEditorIconType::AK_ICON_SOUNDBANK_INIT);
			}
			else
			{
				icon = AkEditorUtils::get_singleton()->get_editor_icon(
						AkEditorUtils::AkEditorIconType::AK_ICON_SOUNDBANK);
			}
		}

		property_control->set_text(name);
		property_control->set_button_icon(icon);
	}
	else
	{
		const AkInspectorEditorPropertyInfo info = get_ak_inspector_property_info(ak_type);
		property_control->set_text(info.text);
		property_control->set_button_icon(Ref<Texture2D>());
	}

	switch (ak_type)
	{
		case AkEditorUtils::AkType::AKTYPE_STATE:
			window->tree->user_data = get_edited_object()->get("state_group");
			break;
		case AkEditorUtils::AkType::AKTYPE_SWITCH:
			window->tree->user_data = get_edited_object()->get("switch_group");
			break;
		default:
			break;
	}

	close_popup();
	get_edited_object()->notify_property_list_changed();
	updating = false;
}

void AkInspectorEditorProperty::reset() { emit_changed(get_edited_property(), current_value); }

void AkInspectorEditorProperty::_on_button_pressed()
{
	if (updating)
	{
		return;
	}

	window->set_min_size(Vector2(window->root_vbox->get_size().x, window->root_vbox->get_size().y));
	open_popup();
}

void AkInspectorEditorProperty::_on_item_selected()
{
	AkInspectorTree* tree = window->tree;

	TreeItem* selected_item = tree->get_selected();

	if (selected_item)
	{
		Dictionary current_value;
		current_value["name"] = selected_item->get_meta("name");
		current_value["id"] = selected_item->get_meta("id");

		property_control->set_text(current_value["name"]);
		property_control->set_button_icon(icon);

		Variant new_value = get_edited_object()->get(get_edited_property());

		if (new_value.get_type() != Variant::Type::DICTIONARY)
		{
			emit_changed(get_edited_property(), current_value);
			return;
		}

		if (new_value.get("id", 0) != current_value.get("id", 0))
		{
			if (ak_type == AkEditorUtils::AkType::AKTYPE_STATE_GROUP)
			{
				Dictionary default_value;
				default_value["name"] = "";
				default_value["id"] = 0;
				emit_changed("state_value", default_value);
			}
			if (ak_type == AkEditorUtils::AkType::AKTYPE_SWITCH_GROUP)
			{
				Dictionary default_value;
				default_value["name"] = "";
				default_value["id"] = 0;
				emit_changed("switch_value", default_value);
			}
		}

		emit_changed(get_edited_property(), current_value);
	}
}

bool AkInspectorEditorInspectorPlugin::_can_handle(Object* object) const
{
	return (object->get_class() == "AkBank" || object->get_class() == "AkState" || object->get_class() == "AkSwitch" ||
			object->get_class() == "AkEvent2D" || object->get_class() == "AkEvent3D" ||
			object->get_class() == "AkEnvironment" || object->get_class() == "AkRoom" ||
			object->get_class() == "AkEarlyReflections" || object->get_class() == "AkGeometry");
}

bool AkInspectorEditorInspectorPlugin::_parse_property(Object* object, Variant::Type type, const String& name,
		PropertyHint hint_type, const String& hint_string, BitField<PropertyUsageFlags> usage_flags, bool wide)
{
	if (type == Variant::DICTIONARY)
	{
		if (name == "event")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkEditorUtils::AkType::AKTYPE_EVENT);
			add_property_editor(name, property);
			return true;
		}
		else if (name == "bank")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkEditorUtils::AkType::AKTYPE_BANK);
			add_property_editor(name, property);
			return true;
		}
		else if (name == "state_group")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkEditorUtils::AkType::AKTYPE_STATE_GROUP, object->get("state_group"));
			add_property_editor(name, property);
			return true;
		}
		else if (name == "state_value")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkEditorUtils::AkType::AKTYPE_STATE, object->get("state_group"));
			add_property_editor(name, property);
			return true;
		}
		else if (name == "switch_group")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkEditorUtils::AkType::AKTYPE_SWITCH_GROUP, object->get("switch_group"));
			add_property_editor(name, property);
			return true;
		}
		else if (name == "switch_value")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkEditorUtils::AkType::AKTYPE_SWITCH, object->get("switch_group"));
			add_property_editor(name, property);
			return true;
		}
		else if (name == "aux_bus")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkEditorUtils::AkType::AKTYPE_AUX_BUS);
			add_property_editor(name, property);
			return true;
		}
		else if (name == "game_parameter")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkEditorUtils::AkType::AKTYPE_RTPC);
			add_property_editor(name, property);
			return true;
		}
		else if (name == "acoustic_texture")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkEditorUtils::AkType::AKTYPE_ACOUSTIC_TEXTURE);
			add_property_editor(name, property);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
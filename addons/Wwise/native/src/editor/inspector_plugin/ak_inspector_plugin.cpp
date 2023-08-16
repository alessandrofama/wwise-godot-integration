#include "ak_inspector_plugin.h"

using namespace godot;

void AkInspectorTree::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("_on_text_changed", "text_filter"), &AkInspectorTree::_on_text_changed);
	ClassDB::bind_method(D_METHOD("_on_size_changed"), &AkInspectorTree::_on_size_changed);
}

Dictionary AkInspectorTree::get_wwise_ids(const AkUtils::AkType ak_type)
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
				Ref<Script> script = ResourceLoader::get_singleton()->load(path);
				Dictionary script_constants = script->get_script_constant_map();

				String type_constant{};

				switch (ak_type)
				{
					case AkUtils::AkType::AKTYPE_EVENT:
					{
						type_constant = "EVENTS";
						break;
					}
					case AkUtils::AkType::AKTYPE_BANK:
					{
						type_constant = "BANKS";
						break;
					}
					case AkUtils::AkType::AKTYPE_RTPC:
					{
						type_constant = "GAME_PARAMETERS";
						break;
					}
					case AkUtils::AkType::AKTYPE_STATE_GROUP:
					{
						type_constant = "STATES";
						break;
					}
					case AkUtils::AkType::AKTYPE_STATE:
					{
						type_constant = "STATES";
						break;
					}
					case AkUtils::AkType::AKTYPE_SWITCH_GROUP:
					{
						type_constant = "SWITCHES";
						break;
					}
					case AkUtils::AkType::AKTYPE_SWITCH:
					{
						type_constant = "SWITCHES";
						break;
					}
					case AkUtils::AkType::AKTYPE_AUX_BUS:
					{
						type_constant = "AUX_BUSSES";
						break;
					}
					default:
						break;
				}

				Ref<Script> type_script = script_constants.get(type_constant, "");
				Dictionary type_constants = type_script->get_script_constant_map();
				Dictionary _dict = type_constants.get("_dict", "");

				if (ak_type == AkUtils::AKTYPE_STATE)
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
				else if (ak_type == AkUtils::AKTYPE_SWITCH)
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

	return result;
}

void AkInspectorTree::initialize(const AkUtils::AkType item_type, const Dictionary& user_data_)
{
	ak_type = item_type;
	user_data = user_data_;

	window = Object::cast_to<AkInspectorWindow>(get_parent()->get_parent());
	window->connect("size_changed", Callable(this, "_on_size_changed"));

	search_text = window->search_text;
	search_text->connect("text_changed", Callable(this, "_on_text_changed"));

	connect("item_collapsed", Callable(this, "_on_item_collapsed"));

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

	for (int64_t i = 0; i < dict.size(); i++)
	{
		String name = dict.keys()[i];
		if (name.findn(text_filter) == -1 && !text_filter.is_empty())
		{
			continue;
		}

		TreeItem* item = create_item(root_item);

		if (ak_type == AkUtils::AKTYPE_EVENT)
		{
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/event.svg");
			item->set_text(0, dict.keys()[i]);
			item->set_meta("name", dict.keys()[i]);
			item->set_meta("id", dict.values()[i]);
		}
		else if (ak_type == AkUtils::AKTYPE_BANK)
		{
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/soundbank.svg");
			item->set_text(0, dict.keys()[i]);
			item->set_meta("name", dict.keys()[i]);
			item->set_meta("id", dict.values()[i]);
		}
		else if (ak_type == AkUtils::AKTYPE_STATE_GROUP)
		{
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/stategroup.svg");
			item->set_text(0, dict.keys()[i]);
			item->set_meta("name", dict.keys()[i]);
			item->set_meta("id", dict.values()[i].get("GROUP"));
		}
		else if (ak_type == AkUtils::AKTYPE_STATE)
		{
			if (user_data.is_empty())
			{
				return;
			}
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images//state.svg");
			item->set_text(0, dict.keys()[i]);
			item->set_meta("name", dict.keys()[i]);
			item->set_meta("id", dict.values()[i]);
		}
		else if (ak_type == AkUtils::AKTYPE_SWITCH_GROUP)
		{
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/switchgroup.svg");
			item->set_text(0, dict.keys()[i]);
			item->set_meta("name", dict.keys()[i]);
			item->set_meta("id", dict.values()[i].get("GROUP"));
		}
		else if (ak_type == AkUtils::AKTYPE_SWITCH)
		{
			if (user_data.is_empty())
			{
				return;
			}
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/switch.svg");
			item->set_text(0, dict.keys()[i]);
			item->set_meta("name", dict.keys()[i]);
			item->set_meta("id", dict.values()[i]);
		}
		else if (ak_type == AkUtils::AKTYPE_AUX_BUS)
		{
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/auxbus.svg");
			item->set_text(0, dict.keys()[i]);
			item->set_meta("name", dict.keys()[i]);
			item->set_meta("id", dict.values()[i]);
		}
		else if (ak_type == AkUtils::AKTYPE_RTPC)
		{
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/rtpc.svg");
			item->set_text(0, dict.keys()[i]);
			item->set_meta("name", dict.keys()[i]);
			item->set_meta("id", dict.values()[i]);
		}

		item->set_icon(0, icon);
	}
}

void AkInspectorTree::_on_text_changed(const String& text_filter) { populate_browser(text_filter); }

void AkInspectorTree::_on_size_changed() { window->root_vbox->set_size(window->get_size()); }

void AkInspectorWindow::initialize()
{
	set_name("Window");
	set_title("Wwise Browser");
	set_disable_3d(true);

	root_vbox = memnew(VBoxContainer);
	root_vbox->set_name("ParentVBoxContainer");
	root_vbox->set_size(Size2(700, 1100));
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
}

void AkInspectorEditorProperty::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("reset"), &AkInspectorEditorProperty::reset);
	ClassDB::bind_method(D_METHOD("_on_button_pressed"), &AkInspectorEditorProperty::_on_button_pressed);
	ClassDB::bind_method(D_METHOD("_on_item_selected"), &AkInspectorEditorProperty::_on_item_selected);
}

void AkInspectorEditorProperty::init(const AkUtils::AkType type, const Dictionary& user_data_)
{
	ak_type = type;
	user_data = user_data_;

	current_value["name"] = "";
	current_value["id"] = 0;

	property_control = memnew(Button);
	property_control->set_clip_text(true);
	window = memnew(AkInspectorWindow);
	window->initialize();
	window->set_exclusive(true);
	add_child(property_control);
	window->connect("close_requested", Callable(this, "reset"));
	window->connect("focus_exited", Callable(this, "reset"));

	AkInspectorTree* tree = window->tree;
	tree->initialize(type, user_data);

	switch (type)
	{
		case AkUtils::AKTYPE_EVENT:
			property_control->set_text("Select Event...");
			tree->search_text->set("placeholder_text", "Search Events...");
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/event.svg");
			break;
		case AkUtils::AKTYPE_BANK:
			property_control->set_text("Select Bank...");
			tree->search_text->set("placeholder_text", "Search Banks...");
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/soundbank.svg");
			break;
		case AkUtils::AKTYPE_STATE_GROUP:
			property_control->set_text("Select State Group...");
			tree->search_text->set("placeholder_text", "Search State Groups...");
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/stategroup.svg");
			break;
		case AkUtils::AKTYPE_STATE:
			property_control->set_text("Select State...");
			tree->search_text->set("placeholder_text", "Search State...");
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/state.svg");
			break;
		case AkUtils::AKTYPE_SWITCH_GROUP:
			property_control->set_text("Select Switch Group...");
			tree->search_text->set("placeholder_text", "Search Switch Groups...");
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/switchgroup.svg");
			break;
		case AkUtils::AKTYPE_SWITCH:
			property_control->set_text("Select Switch...");
			tree->search_text->set("placeholder_text", "Search Switches...");
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/switch.svg");
			break;
		case AkUtils::AKTYPE_AUX_BUS:
			property_control->set_text("Select Aux Bus...");
			tree->search_text->set("placeholder_text", "Search Aux Busses...");
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/auxbus.svg");
			break;
		case AkUtils::AKTYPE_RTPC:
			property_control->set_text("Select Game Parameter...");
			tree->search_text->set("placeholder_text", "Search Game Parameters...");
			icon = ResourceLoader::get_singleton()->load("res://addons/Wwise/editor/images/rtpc.svg");
			break;
		default:
			break;
	}

	tree->connect("item_selected", Callable(this, "_on_item_selected"));
	property_control->connect("pressed", Callable(this, "_on_button_pressed"));
}

void AkInspectorEditorProperty::open_popup()
{
	add_child(window);
	window->popup(Rect2i(
			Vector2i(get_global_mouse_position().x - 900.0f, get_global_mouse_position().y - 200.0f), Vector2(1, 1)));
}

void AkInspectorEditorProperty::close_popup()
{
	for (int i = 0; i < get_child_count(); i++)
	{
		if (get_child(i)->get_class() == "AkInspectorWindow")
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

	if (!current_value["name"].operator String().is_empty())
	{
		property_control->set_text(current_value["name"]);
		property_control->set_button_icon(icon);
	}
	else
	{
		switch (ak_type)
		{
			case AkUtils::AKTYPE_EVENT:
				property_control->set_text("Select Event...");
				break;
			case AkUtils::AKTYPE_BANK:
				property_control->set_text("Select Bank...");
				break;
			case AkUtils::AKTYPE_STATE_GROUP:
				property_control->set_text("Select State Group...");
				break;
			case AkUtils::AKTYPE_STATE:
				property_control->set_text("Select State...");
				break;
			case AkUtils::AKTYPE_SWITCH_GROUP:
				property_control->set_text("Select Switch Group...");
				break;
			case AkUtils::AKTYPE_SWITCH:
				property_control->set_text("Select Switch...");
				break;
			case AkUtils::AKTYPE_AUX_BUS:
				property_control->set_text("Select Aux Bus...");
				break;
			case AkUtils::AKTYPE_RTPC:
				property_control->set_text("Select Game Parameter...");
				break;
			default:
				break;
		}
		property_control->set_button_icon(Ref<Texture2D>());
	}

	switch (ak_type)
	{
		case AkUtils::AKTYPE_STATE:
			window->tree->user_data = get_edited_object()->get("state_group");
			break;
		case AkUtils::AKTYPE_SWITCH:
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
			if (ak_type == AkUtils::AKTYPE_STATE_GROUP)
			{
				Dictionary default_value;
				default_value["name"] = "";
				default_value["id"] = 0;
				emit_changed("state_value", default_value);
			}
			if (ak_type == AkUtils::AKTYPE_SWITCH_GROUP)
			{
				Dictionary default_value;
				default_value["name"] = "";
				default_value["id"] = 0;
				emit_changed("state_value", default_value);
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
			object->get_class() == "AkEarlyReflections");
}

bool godot::AkInspectorEditorInspectorPlugin::_parse_property(Object* object, Variant::Type type, const String& name,
		PropertyHint hint_type, const String& hint_string, BitField<PropertyUsageFlags> usage_flags, bool wide)
{
	if (type == Variant::DICTIONARY)
	{
		if (name == "event")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkUtils::AKTYPE_EVENT);
			add_property_editor(name, property);
			return true;
		}
		else if (name == "bank")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkUtils::AKTYPE_BANK);
			add_property_editor(name, property);
			return true;
		}
		else if (name == "state_group")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkUtils::AKTYPE_STATE_GROUP, object->get("state_group"));
			add_property_editor(name, property);
			return true;
		}
		else if (name == "state_value")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkUtils::AKTYPE_STATE, object->get("state_group"));
			add_property_editor(name, property);
			return true;
		}
		else if (name == "switch_group")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkUtils::AKTYPE_SWITCH_GROUP, object->get("switch_group"));
			add_property_editor(name, property);
			return true;
		}
		else if (name == "switch_value")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkUtils::AKTYPE_SWITCH, object->get("switch_group"));
			add_property_editor(name, property);
			return true;
		}
		else if (name == "aux_bus")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkUtils::AKTYPE_AUX_BUS);
			add_property_editor(name, property);
			return true;
		}
		else if (name == "game_parameter")
		{
			AkInspectorEditorProperty* property = memnew(AkInspectorEditorProperty);
			property->init(AkUtils::AKTYPE_RTPC);
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
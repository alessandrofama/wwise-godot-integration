#include "ak_switch.h"

void AkSwitch::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handle_game_event", "game_event"), &AkSwitch::handle_game_event);
	ClassDB::bind_method(D_METHOD("set_switch"), &AkSwitch::set_switch);
	ClassDB::bind_method(D_METHOD("set_ak_event", "ak_event"), &AkSwitch::set_ak_event);
	ClassDB::bind_method(D_METHOD("get_ak_event"), &AkSwitch::get_ak_event);
	ClassDB::bind_method(D_METHOD("set_switch_group", "switch_group"), &AkSwitch::set_switch_group);
	ClassDB::bind_method(D_METHOD("get_switch_group"), &AkSwitch::get_switch_group);
	ClassDB::bind_method(D_METHOD("set_switch_value", "switch_value"), &AkSwitch::set_switch_value);
	ClassDB::bind_method(D_METHOD("get_switch_value"), &AkSwitch::get_switch_value);
	ClassDB::bind_method(D_METHOD("set_trigger_on", "trigger_on"), &AkSwitch::set_trigger_on);
	ClassDB::bind_method(D_METHOD("get_trigger_on"), &AkSwitch::get_trigger_on);

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "ak_event", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Node"),
			"set_ak_event", "get_ak_event");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "switch_group", PROPERTY_HINT_NONE), "set_switch_group",
			"get_switch_group");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "switch_value", PROPERTY_HINT_NONE), "set_switch_value",
			"get_switch_value");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "trigger_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_trigger_on", "get_trigger_on");
	ADD_SIGNAL(MethodInfo("wwise_switch_set"));
}

AkSwitch::AkSwitch()
{
	switch_group["name"] = "";
	switch_group["id"] = 0;

	switch_value["name"] = "";
	switch_value["id"] = 0;
}

void AkSwitch::_enter_tree() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_ENTER_TREE); }

void AkSwitch::_ready() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_READY); }

void AkSwitch::_exit_tree() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_EXIT_TREE); }

void AkSwitch::handle_game_event(AkUtils::GameEvent game_event)
{
	RETURN_IF_EDITOR;

	if (trigger_on == game_event)
	{
		set_switch();
	}
}

void AkSwitch::set_switch()
{
	if (ak_event.is_empty())
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: AkEvent NodePath in AkSwitch is empty, id: %d", get_instance_id()));
		return;
	}

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		unsigned int switch_group_id = switch_group.get("id", 0);
		unsigned int switch_value_id = switch_value.get("id", 0);

		Node* event = get_node<Node>(ak_event);

		if (soundengine->set_switch_id(switch_group_id, switch_value_id, event))
		{
			emit_signal("wwise_switch_set", event, switch_group, switch_value);
		}
	}
}

void AkSwitch::set_ak_event(const NodePath& ak_event) { this->ak_event = ak_event; }

NodePath AkSwitch::get_ak_event() const { return ak_event; }

void AkSwitch::set_switch_group(const Dictionary& switch_group) { this->switch_group = switch_group; }

Dictionary AkSwitch::get_switch_group() const { return switch_group; }

void AkSwitch::set_switch_value(const Dictionary& switch_value) { this->switch_value = switch_value; }

Dictionary AkSwitch::get_switch_value() const { return switch_value; }

void AkSwitch::set_trigger_on(AkUtils::GameEvent trigger_on) { this->trigger_on = trigger_on; }

AkUtils::GameEvent AkSwitch::get_trigger_on() const { return trigger_on; }
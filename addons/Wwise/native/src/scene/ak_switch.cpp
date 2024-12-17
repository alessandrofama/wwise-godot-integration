#include "ak_switch.h"

void AkSwitch::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handle_game_event", "game_event"), &AkSwitch::handle_game_event);
	ClassDB::bind_method(D_METHOD("set_value"), &AkSwitch::set_value);
	ClassDB::bind_method(D_METHOD("set_wwise_switch", "switch"), &AkSwitch::set_wwise_switch);
	ClassDB::bind_method(D_METHOD("get_wwise_switch"), &AkSwitch::get_wwise_switch);
	ClassDB::bind_method(D_METHOD("set_game_object", "game_object"), &AkSwitch::set_game_object);
	ClassDB::bind_method(D_METHOD("get_game_object"), &AkSwitch::get_game_object);
	ClassDB::bind_method(D_METHOD("set_trigger_on", "trigger_on"), &AkSwitch::set_trigger_on);
	ClassDB::bind_method(D_METHOD("get_trigger_on"), &AkSwitch::get_trigger_on);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "switch", PROPERTY_HINT_RESOURCE_TYPE, "WwiseSwitch"),
			"set_wwise_switch", "get_wwise_switch");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "game_object", PROPERTY_HINT_NODE_TYPE, "Node"), "set_game_object",
			"get_game_object");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "trigger_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_trigger_on", "get_trigger_on");
}

void AkSwitch::_enter_tree() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_ENTER_TREE); }

void AkSwitch::_ready() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_READY); }

void AkSwitch::_exit_tree() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_EXIT_TREE); }

void AkSwitch::handle_game_event(AkUtils::GameEvent game_event)
{
	RETURN_IF_EDITOR;

	if (trigger_on == game_event)
	{
		set_value();
	}
}

void AkSwitch::set_value()
{
	if (!game_object)
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: Assigned Node in AkSwitch %s is null!", get_name()));
		return;
	}

	if (wwise_switch.is_valid())
	{
		wwise_switch->set_value(game_object);
	}
}

void AkSwitch::set_wwise_switch(const Ref<WwiseSwitch>& p_switch)
{
	wwise_switch = p_switch;
	notify_property_list_changed();
}

Ref<WwiseSwitch> AkSwitch::get_wwise_switch() const { return wwise_switch; }

void AkSwitch::set_game_object(Node* p_game_object) { game_object = p_game_object; }

Node* AkSwitch::get_game_object() const { return game_object; }

void AkSwitch::set_trigger_on(AkUtils::GameEvent trigger_on) { this->trigger_on = trigger_on; }

AkUtils::GameEvent AkSwitch::get_trigger_on() const { return trigger_on; }
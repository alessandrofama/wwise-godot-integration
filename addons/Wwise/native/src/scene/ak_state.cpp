#include "ak_state.h"

void AkState::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handle_game_event", "game_event"), &AkState::handle_game_event);
	ClassDB::bind_method(D_METHOD("set_state"), &AkState::set_state);
	ClassDB::bind_method(D_METHOD("set_state_group", "state_group"), &AkState::set_state_group);
	ClassDB::bind_method(D_METHOD("get_state_group"), &AkState::get_state_group);
	ClassDB::bind_method(D_METHOD("set_state_value", "state_value"), &AkState::set_state_value);
	ClassDB::bind_method(D_METHOD("get_state_value"), &AkState::get_state_value);
	ClassDB::bind_method(D_METHOD("set_trigger_on", "trigger_on"), &AkState::set_trigger_on);
	ClassDB::bind_method(D_METHOD("get_trigger_on"), &AkState::get_trigger_on);

	ADD_PROPERTY(
			PropertyInfo(Variant::DICTIONARY, "state_group", PROPERTY_HINT_NONE), "set_state_group", "get_state_group");
	ADD_PROPERTY(
			PropertyInfo(Variant::DICTIONARY, "state_value", PROPERTY_HINT_NONE), "set_state_value", "get_state_value");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "trigger_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_trigger_on", "get_trigger_on");
	ADD_SIGNAL(MethodInfo("wwise_state_set"));
}

AkState::AkState()
{
	state_group["name"] = "";
	state_group["id"] = 0;

	state_value["name"] = "";
	state_value["id"] = 0;
}

void AkState::_enter_tree() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_ENTER_TREE); }

void AkState::_ready() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_READY); }

void AkState::_exit_tree() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_EXIT_TREE); }

void AkState::handle_game_event(AkUtils::GameEvent game_event)
{
	RETURN_IF_EDITOR;

	if (trigger_on == game_event)
	{
		set_state();
	}
}

void AkState::set_state()
{
	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		unsigned int state_group_id = state_group.get("id", 0);
		unsigned int state_value_id = state_value.get("id", 0);

		if (soundengine->set_state_id(state_group_id, state_value_id))
		{
			emit_signal("wwise_state_set", state_group, state_value);
		}
	}
}

void AkState::set_state_group(const Dictionary& state_group) { this->state_group = state_group; }

Dictionary AkState::get_state_group() const { return state_group; }

void AkState::set_state_value(const Dictionary& state_value) { this->state_value = state_value; }

Dictionary AkState::get_state_value() const { return state_value; }

void AkState::set_trigger_on(AkUtils::GameEvent trigger_on) { this->trigger_on = trigger_on; }

AkUtils::GameEvent AkState::get_trigger_on() const { return trigger_on; }
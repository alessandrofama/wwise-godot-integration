#include "ak_state.h"

void AkState::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handle_game_event", "game_event"), &AkState::handle_game_event);
	ClassDB::bind_method(D_METHOD("set_value"), &AkState::set_value);
	ClassDB::bind_method(D_METHOD("set_state", "state"), &AkState::set_state);
	ClassDB::bind_method(D_METHOD("get_state"), &AkState::get_state);
	ClassDB::bind_method(D_METHOD("set_trigger_on", "trigger_on"), &AkState::set_trigger_on);
	ClassDB::bind_method(D_METHOD("get_trigger_on"), &AkState::get_trigger_on);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "state", PROPERTY_HINT_RESOURCE_TYPE, "WwiseState"), "set_state",
			"get_state");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "trigger_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_trigger_on", "get_trigger_on");
}

void AkState::_enter_tree() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_ENTER_TREE); }

void AkState::_ready() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_READY); }

void AkState::_exit_tree() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_EXIT_TREE); }

void AkState::handle_game_event(AkUtils::GameEvent game_event)
{
	RETURN_IF_EDITOR;

	if (trigger_on == game_event)
	{
		set_value();
	}
}

void AkState::set_value()
{
	if (state.is_valid())
	{
		state->set_value();
	}
}

void AkState::set_state(const Ref<WwiseState>& p_state)
{
	state = p_state;
	notify_property_list_changed();
}

Ref<WwiseState> AkState::get_state() const { return state; }

void AkState::set_trigger_on(AkUtils::GameEvent trigger_on) { this->trigger_on = trigger_on; }

AkUtils::GameEvent AkState::get_trigger_on() const { return trigger_on; }
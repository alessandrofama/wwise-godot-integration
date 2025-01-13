#pragma once

#include "core/ak_utils.h"
#include "core/types/wwise_state.h"
#include "core/utils.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

class AkState : public Node
{
	GDCLASS(AkState, Node);

protected:
	static void _bind_methods();

private:
	Ref<WwiseState> state;
	AkUtils::GameEvent trigger_on = AkUtils::GameEvent::GAMEEVENT_NONE;

public:
	virtual void _enter_tree() override;
	virtual void _ready() override;
	virtual void _exit_tree() override;

	void handle_game_event(AkUtils::GameEvent game_event);

	void set_value();

	void set_state(const Ref<WwiseState>& p_state);
	Ref<WwiseState> get_state() const;

	void set_trigger_on(AkUtils::GameEvent trigger_on);
	AkUtils::GameEvent get_trigger_on() const;
};
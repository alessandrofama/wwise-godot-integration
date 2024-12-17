#pragma once

#include "core/ak_utils.h"
#include "core/types/wwise_switch.h"
#include "core/utils.h"
#include <godot_cpp/classes/node.hpp>

using namespace godot;

class AkSwitch : public Node
{
	GDCLASS(AkSwitch, Node);

protected:
	static void _bind_methods();

private:
	Ref<WwiseSwitch> wwise_switch;
	Node* game_object{ nullptr };
	AkUtils::GameEvent trigger_on = AkUtils::GameEvent::GAMEEVENT_NONE;

public:
	virtual void _enter_tree() override;
	virtual void _ready() override;
	virtual void _exit_tree() override;

	void handle_game_event(AkUtils::GameEvent game_event);

	void set_value();

	void set_wwise_switch(const Ref<WwiseSwitch>& p_switch);
	Ref<WwiseSwitch> get_wwise_switch() const;

	void set_game_object(Node* p_game_object);
	Node* get_game_object() const;

	void set_trigger_on(AkUtils::GameEvent trigger_on);
	AkUtils::GameEvent get_trigger_on() const;
};
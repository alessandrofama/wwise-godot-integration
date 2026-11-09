#include "wwise_switch.h"

void WwiseSwitch::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_value", "game_object"), &WwiseSwitch::set_value);
}

void WwiseSwitch::set_value(Node* p_game_object)
{
	auto switch_group_id = get_group_id();
	auto switch_id = get_id();
	Wwise::get_singleton()->set_switch_id(switch_group_id, switch_id, p_game_object);
}

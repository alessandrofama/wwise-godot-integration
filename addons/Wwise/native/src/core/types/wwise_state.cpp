#include "wwise_state.h"

void WwiseState::_bind_methods() { ClassDB::bind_method(D_METHOD("set_value"), &WwiseState::set_value); }

void WwiseState::set_value()
{
	auto group_id = get_group_id();
	auto id = get_id();
	Wwise::get_singleton()->set_state_id(group_id, id);
}
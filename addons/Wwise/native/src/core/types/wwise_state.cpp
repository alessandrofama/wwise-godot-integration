#include "wwise_state.h"

void WwiseState::_bind_methods() { ClassDB::bind_method(D_METHOD("set_value"), &WwiseState::set_value); }

void WwiseState::set_value()
{
	auto state_group_id = get_group_id();
	auto state_id = get_id();
	Wwise::get_singleton()->set_state_id(state_group_id, state_id);
}

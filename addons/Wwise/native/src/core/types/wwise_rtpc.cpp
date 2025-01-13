#include "wwise_rtpc.h"

void WwiseRTPC::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_value", "game_object", "value"), &WwiseRTPC::set_value);
	ClassDB::bind_method(D_METHOD("get_value", "game_object"), &WwiseRTPC::get_value);
	ClassDB::bind_method(D_METHOD("set_global_value", "value"), &WwiseRTPC::set_global_value);
	ClassDB::bind_method(D_METHOD("get_global_value"), &WwiseRTPC::get_global_value);
}

void WwiseRTPC::set_value(Node* p_game_object, float p_value)
{
	AkUniqueID id = get_id();
	Wwise::get_singleton()->set_rtpc_value_id(id, p_value, p_game_object);
}

float WwiseRTPC::get_value(Node* p_game_object)
{
	AkUniqueID id = get_id();
	float value = Wwise::get_singleton()->get_rtpc_value_id(id, p_game_object);
	return value;
}

void WwiseRTPC::set_global_value(float p_value)
{
	AkUniqueID id = get_id();
	Wwise::get_singleton()->set_rtpc_value_id(id, p_value, nullptr);
}

float WwiseRTPC::get_global_value()
{
	AkUniqueID id = get_id();
	float value = Wwise::get_singleton()->get_rtpc_value_id(id, nullptr);
	return value;
}

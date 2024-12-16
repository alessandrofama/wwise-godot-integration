#include "wwise_tree_object_event.h"

void WwiseTreeObjectEvent::_bind_methods()
{
	ClassDB::bind_method(
			D_METHOD("set_max_attenuation", "p_max_attenuation"), &WwiseTreeObjectEvent::set_max_attenuation);
	ClassDB::bind_method(D_METHOD("get_max_attenuation"), &WwiseTreeObjectEvent::get_max_attenuation);

	ClassDB::bind_method(D_METHOD("set_max_duration", "p_max_duration"), &WwiseTreeObjectEvent::set_max_duration);
	ClassDB::bind_method(D_METHOD("get_max_duration"), &WwiseTreeObjectEvent::get_max_duration);

	ClassDB::bind_method(D_METHOD("set_min_duration", "p_min_duration"), &WwiseTreeObjectEvent::set_min_duration);
	ClassDB::bind_method(D_METHOD("get_min_duration"), &WwiseTreeObjectEvent::get_min_duration);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_attenuation"), "set_max_attenuation", "get_max_attenuation");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_duration"), "set_max_duration", "get_max_duration");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_duration"), "set_min_duration", "get_min_duration");
}

void WwiseTreeObjectEvent::set_max_attenuation(float p_max_attenuation) { max_attenuation = p_max_attenuation; }

float WwiseTreeObjectEvent::get_max_attenuation() const { return max_attenuation; }

void WwiseTreeObjectEvent::set_max_duration(float p_max_duration) { max_duration = p_max_duration; }

float WwiseTreeObjectEvent::get_max_duration() const { return max_duration; }

void WwiseTreeObjectEvent::set_min_duration(float p_min_duration) { min_duration = p_min_duration; }

float WwiseTreeObjectEvent::get_min_duration() const { return min_duration; }
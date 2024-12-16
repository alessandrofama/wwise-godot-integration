#include "wwise_group_type.h"

void WwiseGroupType::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_group_id", "group_id"), &WwiseGroupType::set_group_id);
	ClassDB::bind_method(D_METHOD("get_group_id"), &WwiseGroupType::get_group_id);

	ClassDB::bind_method(D_METHOD("set_group_ref", "group_ref"), &WwiseGroupType::set_group_ref);
	ClassDB::bind_method(D_METHOD("get_group_ref"), &WwiseGroupType::get_group_ref);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "group_ref", PROPERTY_HINT_RESOURCE_TYPE, ""), "set_group_ref",
			"get_group_ref");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "group_id", PROPERTY_HINT_NONE, ""), "set_group_id", "get_group_id");
}

void WwiseGroupType::set_group_id(uint32_t p_id)
{
	group_id = p_id;
	notify_property_list_changed();
}

uint32_t WwiseGroupType::get_group_id() const
{
	if (get_group_ref().is_valid())
	{
		return get_group_ref()->get_id();
	}
	return group_id;
}

void WwiseGroupType::set_group_ref(const Ref<WwiseBaseType>& p_group_ref)
{
	group_ref = p_group_ref;
	set_group_id(p_group_ref->get_id());
	notify_property_list_changed();
}

Ref<WwiseBaseType> WwiseGroupType::get_group_ref() const { return group_ref; }

String WwiseGroupType::get_display_name()
{
	auto group_ref = get_group_ref();

	if (group_ref.is_valid())
	{
		return vformat("%s / %s", group_ref->get_name(), get_name());
	}

	return get_name();
}

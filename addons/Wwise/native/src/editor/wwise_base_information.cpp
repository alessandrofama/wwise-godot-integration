#include "wwise_base_information.h"

void WwiseBaseInformation::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_name", "name"), &WwiseBaseInformation::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &WwiseBaseInformation::get_name);
	ClassDB::bind_method(D_METHOD("set_guid", "guid"), &WwiseBaseInformation::set_guid);
	ClassDB::bind_method(D_METHOD("get_guid"), &WwiseBaseInformation::get_guid);
	ClassDB::bind_method(D_METHOD("set_id", "id"), &WwiseBaseInformation::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &WwiseBaseInformation::get_id);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "guid"), "set_guid", "get_guid");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");
}

void WwiseBaseInformation::set_name(const String& p_name)
{
	name = p_name;
	set_id(ShortIDGenerator::compute(p_name));
}

String WwiseBaseInformation::get_name() const { return name; }

void WwiseBaseInformation::set_guid(const String& p_guid)
{
	guid = p_guid;
	emit_changed();
}

String WwiseBaseInformation::get_guid() const { return guid; }

void WwiseBaseInformation::set_id(uint32_t p_id)
{
	id = p_id;
	emit_changed();
}

uint32_t WwiseBaseInformation::get_id() const { return id; }
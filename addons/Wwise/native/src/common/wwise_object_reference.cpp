#include "wwise_object_reference.h"

HashMap<WwiseObjectType, HashMap<String, Ref<WwiseObjectReference>>> WwiseObjectReference::s_objectReferenceDictionary;


void WwiseObjectReference::_bind_methods() {

    ClassDB::bind_method(D_METHOD("set_object_name", "object_name"), &WwiseObjectReference::set_object_name);
	ClassDB::bind_method(D_METHOD("get_object_name"), &WwiseObjectReference::get_object_name);
    ClassDB::bind_method(D_METHOD("set_id", "id"), &WwiseObjectReference::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &WwiseObjectReference::get_id);
    ClassDB::bind_method(D_METHOD("set_guid", "guid"), &WwiseObjectReference::set_guid);
	ClassDB::bind_method(D_METHOD("get_guid"), &WwiseObjectReference::get_guid);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "object_name", PROPERTY_HINT_NONE, ""),
			"set_object_name", "get_object_name");
ADD_PROPERTY(PropertyInfo(Variant::INT, "id", PROPERTY_HINT_NONE, ""),
			"set_id", "get_id");
ADD_PROPERTY(PropertyInfo(Variant::STRING, "guid", PROPERTY_HINT_NONE, ""),
			"set_guid", "get_guid");

}

void WwiseObjectReference::set_object_name(const String& p_object_name) { object_name = p_object_name; }

String WwiseObjectReference::get_object_name() const { return object_name; }

void WwiseObjectReference::set_id(uint32_t p_id) { id = p_id; }

uint32_t WwiseObjectReference::get_id() const { return id; }

void WwiseObjectReference::set_guid(const String& p_guid) { guid = p_guid; }

String WwiseObjectReference::get_guid() const
{
	if (guid.is_empty())
	{
		return "00000000-0000-0000-0000-000000000000";
	}
	else
	{
		return guid;
	}
}

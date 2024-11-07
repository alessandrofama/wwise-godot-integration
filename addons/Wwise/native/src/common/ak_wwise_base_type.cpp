#include "ak_wwise_base_type.h"

void AkWwiseBaseType::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_name", "name"), &AkWwiseBaseType::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &AkWwiseBaseType::get_name);
    ClassDB::bind_method(D_METHOD("set_id", "id"), &AkWwiseBaseType::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &AkWwiseBaseType::get_id);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name", PROPERTY_HINT_NONE, ""),
			"set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "id", PROPERTY_HINT_NONE, ""),
			"set_id", "get_id");
}

void AkWwiseBaseType::set_name(const String& p_name) { name = p_name; }

String AkWwiseBaseType::get_name() const { return name; }

void AkWwiseBaseType::set_id(uint32_t p_id) { id = p_id; }

uint32_t AkWwiseBaseType::get_id() const { return id; }
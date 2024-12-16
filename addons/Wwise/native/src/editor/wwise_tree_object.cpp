#include "wwise_tree_object.h"

void WwiseTreeObject::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_path", "path"), &WwiseTreeObject::set_path);
	ClassDB::bind_method(D_METHOD("get_path"), &WwiseTreeObject::get_path);
	ClassDB::bind_method(D_METHOD("set_children", "children"), &WwiseTreeObject::set_children);
	ClassDB::bind_method(D_METHOD("get_children"), &WwiseTreeObject::get_children);
	ClassDB::bind_method(D_METHOD("set_type", "type"), &WwiseTreeObject::set_type);
	ClassDB::bind_method(D_METHOD("get_type"), &WwiseTreeObject::get_type);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "path"), "set_path", "get_path");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "children"), "set_children", "get_children");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");
}

void WwiseTreeObject::set_path(const String& p_path) { path = p_path; }

String WwiseTreeObject::get_path() const { return path; }

void WwiseTreeObject::set_children(const Array& p_children) { children = p_children; }

Array WwiseTreeObject::get_children() const { return children; }

void WwiseTreeObject::set_type(int p_type) { type = p_type; }

int WwiseTreeObject::get_type() const { return type; }

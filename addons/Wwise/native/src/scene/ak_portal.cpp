#include "ak_portal.h"

using namespace godot;

void AkPortal::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_portal"), &AkPortal::set_portal);
	ClassDB::bind_method(D_METHOD("set_front_room", "front_room"), &AkPortal::set_front_room);
	ClassDB::bind_method(D_METHOD("get_front_room"), &AkPortal::get_front_room);
	ClassDB::bind_method(D_METHOD("set_back_room", "back_room"), &AkPortal::set_back_room);
	ClassDB::bind_method(D_METHOD("get_back_room"), &AkPortal::get_back_room);
	ClassDB::bind_method(D_METHOD("set_enabled", "enabled"), &AkPortal::set_enabled);
	ClassDB::bind_method(D_METHOD("get_enabled"), &AkPortal::get_enabled);

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "front_room", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Node"),
			"set_front_room", "get_front_room");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "back_room", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Node"),
			"set_back_room", "get_back_room");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enabled", PROPERTY_HINT_NONE), "set_enabled", "get_enabled");
}

void AkPortal::_ready()
{
	RETURN_IF_EDITOR;

	if (!front_room.is_empty())
	{
		front_room_node = get_node<Node>(front_room);
	}

	if (!back_room.is_empty())
	{
		back_room_node = get_node<Node>(back_room);
	}

	extent = get_global_transform().get_basis().get_scale();
	set_portal();
}

void AkPortal::set_portal() const
{
	RETURN_IF_EDITOR;

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		soundengine->set_portal(this, get_global_transform(), extent, front_room_node, back_room_node, enabled);
	}
}

void AkPortal::set_front_room(const NodePath& front_room) { this->front_room = front_room; }

NodePath AkPortal::get_front_room() const { return front_room; }

void AkPortal::set_back_room(const NodePath& back_room) { this->back_room = back_room; }

NodePath AkPortal::get_back_room() const { return back_room; }

void AkPortal::set_enabled(bool enabled)
{
	if (this->enabled != enabled)
	{
		this->enabled = enabled;
		set_portal();
	}
}

bool AkPortal::get_enabled() const { return enabled; }

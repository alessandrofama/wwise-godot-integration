#include "ak_portal.h"

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

void AkPortal::_enter_tree()
{
	if (get_child_count() == 0)
	{
		collision_shape = memnew(CollisionShape3D);
		box_shape = memnew(BoxShape3D);

		collision_shape->set_name("Shape");
		collision_shape->set_shape(box_shape);
		box_shape->set_size(Vector3(0.9f, 2.0f, 0.3f));
		add_child(collision_shape);

		collision_shape->set_owner(get_tree()->get_edited_scene_root());
	}
}

void AkPortal::_ready()
{
	RETURN_IF_EDITOR;

	front_room_node = get_node_or_null(front_room);
	back_room_node = get_node_or_null(back_room);

	collision_shape = Object::cast_to<CollisionShape3D>(get_child(0));

	if (collision_shape)
	{
		box_shape = (BoxShape3D*)collision_shape->get_shape().ptr();
		if (box_shape)
		{
			extent = box_shape->get_size();
			set_portal();
		}
	}
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

		RETURN_IF_EDITOR;

		if (is_inside_tree())
		{
			set_portal();
		}
	}
}

bool AkPortal::get_enabled() const { return enabled; }
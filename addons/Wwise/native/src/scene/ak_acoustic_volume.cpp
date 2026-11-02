#include "ak_acoustic_volume.h"
#include "ak_geometry.h"
#include "ak_room.h"
#include <godot_cpp/classes/scene_tree.hpp>

void AkAcousticVolume::_bind_methods() {}

void AkAcousticVolume::_notification(int p_what)
{
#if defined(TOOLS_ENABLED)
	switch (p_what)
	{
		case NOTIFICATION_ENTER_TREE:
		{
			if (Engine::get_singleton()->is_editor_hint())
			{
				Node* scene_root = get_tree()->get_edited_scene_root();
				Node* owner_node = scene_root ? scene_root : this;

				CollisionShape3D* collision_shape = nullptr;
				AkGeometry* geometry = nullptr;
				AkRoom* room = nullptr;

				for (int i = 0; i < get_child_count(); i++)
				{
					Node* child = get_child(i);

					if (!collision_shape)
					{
						collision_shape = Object::cast_to<CollisionShape3D>(child);
					}
					if (!geometry)
					{
						geometry = Object::cast_to<AkGeometry>(child);
					}
					if (!room)
					{
						room = Object::cast_to<AkRoom>(child);
					}
				}

				String new_parent_name = get_name();

				if (!collision_shape)
				{
					collision_shape = memnew(CollisionShape3D);
					collision_shape->set_name(new_parent_name + "_Collision");

					Ref<BoxShape3D> box_shape;
					box_shape.instantiate();
					collision_shape->set_shape(box_shape);

					add_child(collision_shape);
					collision_shape->set_owner(owner_node);
				}

				if (!geometry)
				{
					geometry = memnew(AkGeometry);
					geometry->set_name(new_parent_name + "_AkGeometry");

					add_child(geometry);
					geometry->set_owner(owner_node);
				}

				if (!room)
				{
					room = memnew(AkRoom);
					room->set_name(new_parent_name + "_AkRoom");

					add_child(room);
					room->set_owner(owner_node);

					if (geometry)
					{
						room->set_associated_geometry(room->get_path_to(geometry));
					}
				}
			}
			break;
		}
		case Node::NOTIFICATION_PATH_RENAMED:
		{
			if (Engine::get_singleton()->is_editor_hint())
			{
				String new_parent_name = get_name();

				AkGeometry* geometry = nullptr;
				AkRoom* room = nullptr;

				for (int i = 0; i < get_child_count(); i++)
				{
					Node* child = get_child(i);

					if (Object::cast_to<CollisionShape3D>(child))
					{
						child->set_name(new_parent_name + "_Collision");
					}
					else if (Object::cast_to<AkGeometry>(child))
					{
						geometry = Object::cast_to<AkGeometry>(child);
						geometry->set_name(new_parent_name + "_AkGeometry");
					}
					else if (Object::cast_to<AkRoom>(child))
					{
						room = Object::cast_to<AkRoom>(child);
						room->set_name(new_parent_name + "_AkRoom");
					}
				}

				if (room && geometry)
				{
					room->set_associated_geometry(room->get_path_to(geometry));
				}
			}
			break;
		}
	}
#endif
}

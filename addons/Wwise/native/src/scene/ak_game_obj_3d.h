#pragma once

#include "core/ak_game_obj_helper.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node3d.hpp>

class AkGameObj3D : public Node3D
{
	GDCLASS(AkGameObj3D, Node3D);

protected:
	static void _bind_methods() {};

	void _notification(int p_what)
	{
		if (Engine::get_singleton()->is_editor_hint())
			return;

		switch (p_what)
		{
			case NOTIFICATION_ENTER_TREE:
			{
				game_object = get_parent_node_3d();

				if (!game_object)
					return;

				is_registered = AkGameObjHelper::register_game_obj(game_object);

				if (!is_registered)
					return;

				AkGameObjHelper::set_position(game_object, cached_transform);

				set_process(true);
				break;
			}
			case NOTIFICATION_PROCESS:
			{
				if (is_registered && game_object)
				{
					AkGameObjHelper::set_position(game_object, cached_transform);
				}
				break;
			}
			case NOTIFICATION_PREDELETE:
			{
				if (is_registered && game_object)
				{
					AkGameObjHelper::unregister_game_obj(game_object);
					is_registered = false;
				}
				break;
			}
		}
	}

private:
	Node3D* game_object{ nullptr };
	bool is_registered{ false };
	Transform3D cached_transform{};
};
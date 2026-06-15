#pragma once

#include "core/ak_game_obj_helper.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node2d.hpp>

class AkGameObj2D : public Node2D
{
	GDCLASS(AkGameObj2D, Node2D);

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
				game_object = Object::cast_to<Node2D>(get_parent());

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
	Node2D* game_object{ nullptr };
	bool is_registered{ false };
	Transform2D cached_transform{};
};
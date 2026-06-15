#pragma once

#include "core/ak_game_obj_helper.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>

class AkGameObj : public Node
{
	GDCLASS(AkGameObj, Node);

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
				game_object = get_parent();

				if (!game_object)
					return;

				is_registered = AkGameObjHelper::register_game_obj(game_object);

				if (!is_registered)
					return;

				set_process(false);
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
	Node* game_object{ nullptr };
	bool is_registered{ false };
};
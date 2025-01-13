#pragma once

#include "core/ak_game_obj_helper.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node2d.hpp>

class AkGameObj2D : public Node2D
{
	GDCLASS(AkGameObj2D, Node2D);

private:
	AkGameObjHelper<Node2D>* helper{ nullptr };

protected:
	static void _bind_methods() {};

	void _notification(int p_what)
	{
		if (Engine::get_singleton()->is_editor_hint())
			return;

		if (p_what == NOTIFICATION_ENTER_TREE)
		{
			if (!helper)
			{
				Node* parent = get_parent();
				Node2D* node_2d = Object::cast_to<Node2D>(parent);
				if (parent && node_2d)
				{
					helper = new AkGameObjHelper<Node2D>(node_2d);
					if (helper->register_game_obj())
					{
						helper->set_position();
					}
					set_process(true);
				}
			}
		}

		if (p_what == NOTIFICATION_PROCESS)
		{
			if (helper && helper->get_is_registered())
			{
				helper->set_position();
			}
		}

		if (p_what == NOTIFICATION_PREDELETE)
		{
			if (helper)
			{
				helper->unregister_game_obj();
				delete helper;
				helper = nullptr;
			}
		}
	}
};
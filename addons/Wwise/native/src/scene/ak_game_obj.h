#pragma once

#include "core/ak_game_obj_helper.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>

class AkGameObj : public Node
{
	GDCLASS(AkGameObj, Node);

private:
	AkGameObjHelper<Node>* helper{ nullptr };

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
				if (parent)
				{
					helper = new AkGameObjHelper<Node>(parent);
					if (helper->register_game_obj())
					{
						helper->set_position();
					}
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
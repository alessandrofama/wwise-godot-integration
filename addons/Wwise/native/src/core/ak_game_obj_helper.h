#pragma once

#include "core/wwise_gdextension.h"

class AkGameObjHelper
{
public:
	static bool register_game_obj(const Node* p_game_object)
	{
		return Wwise::get_singleton()->register_game_obj(p_game_object, p_game_object->get_name());
	}

	static bool unregister_game_obj(const Node* p_game_object)
	{
		return Wwise::get_singleton()->unregister_game_obj(p_game_object);
	}

	static void set_position(const Node2D* p_game_object, Transform2D& p_cached_transform)
	{
		Transform2D current = p_game_object->get_global_transform();

		if (!current.is_equal_approx(p_cached_transform))
		{
			Wwise::get_singleton()->set_2d_position(p_game_object, current, 0);
			p_cached_transform = current;
		}
	}

	static void set_position(const Node3D* p_game_object, Transform3D& p_cached_transform)
	{
		Transform3D current = p_game_object->get_global_transform();

		if (!current.is_equal_approx(p_cached_transform))
		{
			Wwise::get_singleton()->set_3d_position(p_game_object, current);
			p_cached_transform = current;
		}
	}
};
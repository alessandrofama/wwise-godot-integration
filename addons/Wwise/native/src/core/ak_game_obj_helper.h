#pragma once

#include "core/wwise_gdextension.h"
#include <type_traits>

template <class T> class AkGameObjHelper
{
private:
	static constexpr bool is_node = std::is_base_of<Node, T>::value;
	bool is_registered = false;
	T* game_object;

public:
	explicit AkGameObjHelper(T* node) : game_object(node) { static_assert(is_node, "T must inherit from Node"); }

	bool register_game_obj()
	{
		if (is_registered)
		{
			return AKRESULT::AK_Success;
		}

		if (!game_object)
		{
			return false;
		}

		is_registered = true;
		return Wwise::get_singleton()->register_game_obj(game_object, game_object->get_name());
	}

	bool unregister_game_obj()
	{
		if (!is_registered)
		{
			AKRESULT::AK_Success;
		}

		is_registered = false;
		return Wwise::get_singleton()->unregister_game_obj(game_object);
	}

	void set_position();

	bool get_is_registered() const { return is_registered; }
};

template <> inline void AkGameObjHelper<Node>::set_position() {}

template <> inline void AkGameObjHelper<Node2D>::set_position()
{
	if (game_object)
	{
		Wwise::get_singleton()->set_2d_position(game_object, game_object->get_global_transform(), 0);
	}
}

template <> inline void AkGameObjHelper<Node3D>::set_position()
{
	if (game_object)
	{
		Wwise::get_singleton()->set_3d_position(game_object, game_object->get_global_transform());
	}
}
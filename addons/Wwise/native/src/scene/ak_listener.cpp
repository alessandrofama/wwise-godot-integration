#include "ak_listener.h"

void AkListener2D::_bind_methods()
{
	ClassDB::bind_method(
			D_METHOD("set_is_default_listener", "is_default_listener"), &AkListener2D::set_is_default_listener);
	ClassDB::bind_method(D_METHOD("get_is_default_listener"), &AkListener2D::get_is_default_listener);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_default_listener", PROPERTY_HINT_NONE), "set_is_default_listener",
			"get_is_default_listener");
}

void AkListener2D::_enter_tree()
{
	RETURN_IF_EDITOR;

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		if (get_is_default_listener())
		{
			soundengine->add_default_listener(this);
		}
	}
}

void AkListener2D::_exit_tree()
{
	RETURN_IF_EDITOR;

	if (Wwise* soundengine = Wwise::get_singleton())
	{
		if (get_is_default_listener())
		{
			soundengine->remove_default_listener(this);
		}

		soundengine->unregister_game_obj(this);
	}
}

void AkListener2D::set_is_default_listener(bool p_is_default) { is_default_listener = p_is_default; }

bool AkListener2D::get_is_default_listener() const { return is_default_listener; }

void AkListener3D::_bind_methods()
{
	ClassDB::bind_method(
			D_METHOD("set_is_default_listener", "is_default_listener"), &AkListener3D::set_is_default_listener);
	ClassDB::bind_method(D_METHOD("get_is_default_listener"), &AkListener3D::get_is_default_listener);
	ClassDB::bind_method(D_METHOD("set_is_spatial", "is_spatial"), &AkListener3D::set_is_spatial);
	ClassDB::bind_method(D_METHOD("get_is_spatial"), &AkListener3D::get_is_spatial);
	ClassDB::bind_method(D_METHOD("set_room_id", "room_id"), &AkListener3D::set_room_id);
	ClassDB::bind_method(D_METHOD("get_room_id"), &AkListener3D::get_room_id);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_default_listener", PROPERTY_HINT_NONE), "set_is_default_listener",
			"get_is_default_listener");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_spatial", PROPERTY_HINT_NONE), "set_is_spatial", "get_is_spatial");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "room_id", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_INTERNAL), "set_room_id",
			"get_room_id");
}

void AkListener3D::_enter_tree()
{
	RETURN_IF_EDITOR;

	if (Wwise* soundengine = Wwise::get_singleton())
	{
		if (get_is_default_listener())
		{
			soundengine->add_default_listener(this);
		}

		if (is_spatial)
		{
			soundengine->register_spatial_listener(this);
		}
	}
}

void AkListener3D::_exit_tree()
{
	RETURN_IF_EDITOR;

	if (Wwise* soundengine = Wwise::get_singleton())
	{
		if (get_is_default_listener())
		{
			soundengine->remove_default_listener(this);
		}

		soundengine->unregister_game_obj(this);
	}
}

void AkListener3D::set_is_default_listener(bool p_is_default) { is_default_listener = p_is_default; }

bool AkListener3D::get_is_default_listener() const { return is_default_listener; }

void AkListener3D::set_is_spatial(bool p_is_spatial) { is_spatial = p_is_spatial; }

bool AkListener3D::get_is_spatial() const { return is_spatial; }

void AkListener3D::set_room_id(uint64_t p_room_id) { room_id = p_room_id; }

uint64_t AkListener3D::get_room_id() const { return room_id; }
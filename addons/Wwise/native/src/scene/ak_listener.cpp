#include "ak_listener.h"

using namespace godot;

void AkListener2D::_bind_methods() {}

void AkListener2D::_enter_tree()
{
	RETURN_IF_EDITOR;

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		soundengine->registerListener(this);
	}
}

void AkListener2D::_process(double p_delta)
{
	RETURN_IF_EDITOR;

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		soundengine->set2DPosition(this, get_global_transform(), get_z_index());
	}
}

void AkListener3D::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_is_spatial", "is_spatial"), &AkListener3D::set_is_spatial);
	ClassDB::bind_method(D_METHOD("get_is_spatial"), &AkListener3D::get_is_spatial);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_spatial", PROPERTY_HINT_NONE), "set_is_spatial", "get_is_spatial");
}

void AkListener3D::_enter_tree()
{
	RETURN_IF_EDITOR;

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		soundengine->registerListener(this);

		if (is_spatial)
		{
			soundengine->registerSpatialListener(this);
		}
	}
}

void AkListener3D::_process(double p_delta)
{
	RETURN_IF_EDITOR;

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		soundengine->set3DPosition(this, get_global_transform());
	}
}

void AkListener3D::set_is_spatial(bool is_spatial) { this->is_spatial = is_spatial; }

bool AkListener3D::get_is_spatial() const { return is_spatial; }
#pragma once

#include <godot_cpp/classes/area3d.hpp>

using namespace godot;

class AkAcousticVolume : public Area3D
{
	GDCLASS(AkAcousticVolume, Area3D)

protected:
	static void _bind_methods();
	void _notification(int p_what);
};
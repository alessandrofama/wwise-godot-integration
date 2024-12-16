#pragma once

#include "editor/wwise_tree_object.h"

using namespace godot;

class WwiseTreeObjectEvent : public WwiseTreeObject
{
	GDCLASS(WwiseTreeObjectEvent, WwiseTreeObject);

protected:
	static void _bind_methods();

private:
	float max_attenuation{};
	float max_duration{ -1 };
	float min_duration{ -1 };

public:
	void set_max_attenuation(float p_max_attenuation);
	float get_max_attenuation() const;

	void set_max_duration(float p_max_duration);
	float get_max_duration() const;

	void set_min_duration(float p_min_duration);
	float get_min_duration() const;
};
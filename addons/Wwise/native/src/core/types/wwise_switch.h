#pragma once

#include "core/types/wwise_group_type.h"
#include "core/wwise_gdextension.h"

using namespace godot;

class WwiseSwitch : public WwiseGroupType
{
	GDCLASS(WwiseSwitch, WwiseGroupType);

protected:
	static void _bind_methods();

public:
	virtual WwiseObjectType get_object_type() const override { return WwiseObjectType::Switch; };
	void set_value(Node* p_game_object);
};
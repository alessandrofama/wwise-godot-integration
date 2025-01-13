#pragma once

#include "core/types/wwise_group_type.h"
#include "core/wwise_gdextension.h"

using namespace godot;

class WwiseState : public WwiseGroupType
{
	GDCLASS(WwiseState, WwiseGroupType);

protected:
	static void _bind_methods();

public:
	virtual WwiseObjectType get_object_type() const override { return WwiseObjectType::State; };
	void set_value();
};
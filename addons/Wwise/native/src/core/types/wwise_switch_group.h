#pragma once

#include "core/types/wwise_base_type.h"

using namespace godot;

class WwiseSwitchGroup : public WwiseBaseType
{
	GDCLASS(WwiseSwitchGroup, WwiseBaseType);

protected:
	static void _bind_methods() {};

public:
	virtual WwiseObjectType get_object_type() const override { return WwiseObjectType::SwitchGroup; };
};
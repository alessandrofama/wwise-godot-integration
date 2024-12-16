#pragma once

#include "core/types/wwise_base_type.h"
#include "core/wwise_gdextension.h"

using namespace godot;

class WwiseTrigger : public WwiseBaseType
{
	GDCLASS(WwiseTrigger, WwiseBaseType);

protected:
	static void _bind_methods();

public:
	virtual WwiseObjectType get_object_type() const override { return WwiseObjectType::Trigger; };
	void post(Node* p_game_object);
};
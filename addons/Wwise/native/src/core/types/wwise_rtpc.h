#pragma once

#include "core/types/wwise_base_type.h"
#include "core/wwise_gdextension.h"

using namespace godot;

class WwiseRTPC : public WwiseBaseType
{
	GDCLASS(WwiseRTPC, WwiseBaseType);

protected:
	static void _bind_methods();

public:
	virtual WwiseObjectType get_object_type() const override { return WwiseObjectType::GameParameter; };
	void set_value(Node* p_game_object, float p_value);
	float get_value(Node* p_game_object);
	void set_global_value(float p_value);
	float get_global_value();
};
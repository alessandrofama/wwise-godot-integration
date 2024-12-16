#pragma once

#include "core/types/wwise_base_type.h"
#include "core/wwise_bank_manager.h"

using namespace godot;

class WwiseBank : public WwiseBaseType
{
	GDCLASS(WwiseBank, WwiseBaseType);

protected:
	static void _bind_methods();

public:
	virtual WwiseObjectType get_object_type() const override { return WwiseObjectType::Soundbank; };
	void load();
	void unload();
};
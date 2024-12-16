#pragma once

#include "editor/properties/ak_wwise_editor_property_acoustic_texture.h"
#include "editor/properties/ak_wwise_editor_property_aux_bus.h"
#include "editor/properties/ak_wwise_editor_property_bank.h"
#include "editor/properties/ak_wwise_editor_property_event.h"
#include "editor/properties/ak_wwise_editor_property_rtpc.h"
#include "editor/properties/ak_wwise_editor_property_state.h"
#include "editor/properties/ak_wwise_editor_property_switch.h"
#include "editor/properties/ak_wwise_editor_property_trigger.h"
#include <godot_cpp/classes/editor_inspector_plugin.hpp>

using namespace godot;

class AkWwiseEditorInspectorPlugin : public EditorInspectorPlugin
{
	GDCLASS(AkWwiseEditorInspectorPlugin, EditorInspectorPlugin);

protected:
	static void _bind_methods() {};

public:
	virtual bool _can_handle(Object* object) const override;
	virtual bool _parse_property(Object* object, Variant::Type type, const String& name, PropertyHint hint_type,
			const String& hint_string, BitField<PropertyUsageFlags> usage_flags, bool wide) override;
};
#include "ak_wwise_editor_inspector_plugin.h"

bool AkWwiseEditorInspectorPlugin::_can_handle(Object* object) const { return object != nullptr; }

bool AkWwiseEditorInspectorPlugin::_parse_property(Object* object, Variant::Type type, const String& name,
		PropertyHint hint_type, const String& hint_string, BitField<PropertyUsageFlags> usage_flags, bool wide)
{
	if (object != nullptr && type == Variant::Type::OBJECT)
	{
		if (hint_string == "WwiseEvent")
		{
			AkWwiseEditorPropertyEvent* property = memnew(AkWwiseEditorPropertyEvent);
			add_property_editor(name, property);
			return true;
		}
		else if (hint_string == "WwiseBank")
		{
			AkWwiseEditorPropertyBank* property = memnew(AkWwiseEditorPropertyBank);
			add_property_editor(name, property);
			return true;
		}
		else if (hint_string == "WwiseAcousticTexture")
		{
			AkWwiseEditorPropertyAcousticTexture* property = memnew(AkWwiseEditorPropertyAcousticTexture);
			add_property_editor(name, property);
			return true;
		}
		else if (hint_string == "WwiseState")
		{
			AkWwiseEditorPropertyState* property = memnew(AkWwiseEditorPropertyState);
			add_property_editor(name, property);
			return true;
		}
		else if (hint_string == "WwiseSwitch")
		{
			AkWwiseEditorPropertySwitch* property = memnew(AkWwiseEditorPropertySwitch);
			add_property_editor(name, property);
			return true;
		}
		else if (hint_string == "WwiseRTPC")
		{
			AkWwiseEditorPropertyRTPC* property = memnew(AkWwiseEditorPropertyRTPC);
			add_property_editor(name, property);
			return true;
		}
		else if (hint_string == "WwiseTrigger")
		{
			AkWwiseEditorPropertyTrigger* property = memnew(AkWwiseEditorPropertyTrigger);
			add_property_editor(name, property);
			return true;
		}
		else if (hint_string == "WwiseAuxBus")
		{
			AkWwiseEditorPropertyAuxBus* property = memnew(AkWwiseEditorPropertyAuxBus);
			add_property_editor(name, property);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

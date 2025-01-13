#include "ak_wwise_editor_property_rtpc.h"

String AkWwiseEditorPropertyRTPC::get_button_placeholder_text() const { return "Select Game Parameter..."; }

WwiseObjectType AkWwiseEditorPropertyRTPC::get_wwise_object_type() const { return WwiseObjectType::GameParameter; }

void AkWwiseEditorPropertyRTPC::set_wwise_reference(const AkWwiseTreeItem* p_item)
{
	auto reference = WwiseBaseType::find_or_create_wwise_object<WwiseRTPC>(
			p_item->get_object_type(), p_item->get_name(), p_item->get_guid());
	emit_changed(get_edited_property(), reference);
}
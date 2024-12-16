#include "ak_wwise_editor_property_event.h"

void AkWwiseEditorPropertyEvent::_bind_methods() {}

String AkWwiseEditorPropertyEvent::get_button_placeholder_text() const { return "Select Event..."; }

WwiseObjectType AkWwiseEditorPropertyEvent::get_wwise_object_type() const { return WwiseObjectType::Event; }

void AkWwiseEditorPropertyEvent::set_wwise_reference(const AkWwiseTreeItem* p_item)
{
	auto reference = WwiseBaseType::find_or_create_wwise_object<WwiseEvent>(
			p_item->get_object_type(), p_item->get_name(), p_item->get_guid());
	emit_changed(get_edited_property(), reference);
}
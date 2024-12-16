#include "ak_wwise_editor_property_bank.h"

String AkWwiseEditorPropertyBank::get_button_placeholder_text() const { return "Select Bank..."; }

WwiseObjectType AkWwiseEditorPropertyBank::get_wwise_object_type() const { return WwiseObjectType::Soundbank; }

void AkWwiseEditorPropertyBank::set_wwise_reference(const AkWwiseTreeItem* p_item)
{
	auto reference = WwiseBaseType::find_or_create_wwise_object<WwiseBank>(
			p_item->get_object_type(), p_item->get_name(), p_item->get_guid());
	emit_changed(get_edited_property(), reference);
}

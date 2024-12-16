#include "ak_wwise_editor_property_aux_bus.h"

String AkWwiseEditorPropertyAuxBus::get_button_placeholder_text() const { return "Select Aux Bus..."; }

WwiseObjectType AkWwiseEditorPropertyAuxBus::get_wwise_object_type() const { return WwiseObjectType::AuxBus; }

void AkWwiseEditorPropertyAuxBus::set_wwise_reference(const AkWwiseTreeItem* p_item)
{
	auto reference = WwiseBaseType::find_or_create_wwise_object<WwiseAuxBus>(
			p_item->get_object_type(), p_item->get_name(), p_item->get_guid());
	emit_changed(get_edited_property(), reference);
}

#include "ak_wwise_editor_property_switch.h"

String AkWwiseEditorPropertySwitch::get_button_placeholder_text() const { return "Select Switch..."; }

WwiseObjectType AkWwiseEditorPropertySwitch::get_wwise_object_type() const { return WwiseObjectType::Switch; }

void AkWwiseEditorPropertySwitch::set_wwise_reference(const AkWwiseTreeItem* p_item)
{
	auto reference = WwiseBaseType::find_or_create_wwise_object<WwiseSwitch>(
			p_item->get_object_type(), p_item->get_name(), p_item->get_guid());

	if (reference.is_valid())
	{
		TreeItem* parent_tree_item = p_item->tree_item->get_parent();
		if (parent_tree_item)
		{
			AkWwiseTreeItem* wwise_tree_item =
					Object::cast_to<AkWwiseTreeItem>(parent_tree_item->get_meta("ak_wwise_tree_item"));
			if (wwise_tree_item)
			{
				reference->setup_group_object_reference<WwiseSwitchGroup>(reference, WwiseObjectType::SwitchGroup,
						wwise_tree_item->get_name(), wwise_tree_item->get_guid());
			}
		}
	}

	emit_changed(get_edited_property(), reference);
}

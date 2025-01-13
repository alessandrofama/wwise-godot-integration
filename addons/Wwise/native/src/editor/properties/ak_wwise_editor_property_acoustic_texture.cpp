#include "ak_wwise_editor_property_acoustic_texture.h"

void AkWwiseEditorPropertyAcousticTexture::_bind_methods() {}

String AkWwiseEditorPropertyAcousticTexture::get_button_placeholder_text() const
{
	return "Select Acoustic Texture...";
}

WwiseObjectType AkWwiseEditorPropertyAcousticTexture::get_wwise_object_type() const
{
	return WwiseObjectType::AcousticTexture;
}

void AkWwiseEditorPropertyAcousticTexture::set_wwise_reference(const AkWwiseTreeItem* p_item)
{
	auto reference = WwiseBaseType::find_or_create_wwise_object<WwiseAcousticTexture>(
			p_item->get_object_type(), p_item->get_name(), p_item->get_guid());
	emit_changed(get_edited_property(), reference);
}

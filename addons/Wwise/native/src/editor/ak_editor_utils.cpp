#include "ak_editor_utils.h"

Ref<Texture2D> AkEditorUtils::get_editor_icon(const WwiseObjectType ak_type)
{
	const AkEditorIconType icon_type = get_ak_editor_icon_type(ak_type);
	return get_editor_icon(icon_type);
}

Ref<Texture2D> AkEditorUtils::get_editor_icon(const AkEditorIconType icon_type)
{
	ERR_FAIL_COND_V_MSG(
			icon_type >= AK_ICON_LENGTH, Ref<Texture2D>(), "[Wwise Editor] Tried to get invalid editor icon.");

	Ref<Texture2D> icon = cached_icons[icon_type];
	if (icon.is_valid())
	{
		return icon;
	}

	const String icon_path = get_ak_editor_icon_path(icon_type);
	icon = ResourceLoader::get_singleton()->load(icon_path);
	
	if (icon.is_valid())
	{
		cached_icons[icon_type] = icon;
	}

	return icon;
}
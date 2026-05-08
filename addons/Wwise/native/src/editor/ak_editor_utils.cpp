#include "ak_editor_utils.h"

Ref<Texture2D> AkEditorUtils::get_editor_icon(const WwiseObjectType p_type)
{
	ERR_FAIL_COND_V_MSG(
			p_type >= WwiseObjectType::Max, Ref<Texture2D>(), "WwiseGodot: Tried to get invalid editor icon.");
	
	const bool dark_mode =
			DisplayServer::get_singleton()->is_dark_mode();

	const int cache_index =
			get_cache_index(dark_mode, p_type);

	Ref<Texture2D> *cache = get_cache();
	Ref<Texture2D> &cached =
			cache[cache_index];

	if (cached.is_valid())
	{
		return cached;
	}

	String icon_path =
			get_theme_folder(dark_mode) +
			get_icon_name(p_type);

	cached = ResourceLoader::get_singleton()->load(icon_path);

	return cached;
}

String AkEditorUtils::get_icon_name(const WwiseObjectType p_type)
{
	switch (p_type)
	{
		case WwiseObjectType::AuxBus:
			return "ObjectIcons_AuxBus_nor.svg";
		case WwiseObjectType::Bus:
			return "ObjectIcons_Bus_nor.svg";
		case WwiseObjectType::Event:
			return "ObjectIcons_Event_nor.svg";
		case WwiseObjectType::Folder:
			return "ObjectIcons_Folder_nor.svg";
		case WwiseObjectType::PhysicalFolder:
			return "ObjectIcons_PhysicalFolder_nor.svg";
		case WwiseObjectType::Project:
			return "ObjectIcons_Project_nor.svg";
		case WwiseObjectType::Soundbank:
			return "ObjectIcons_Soundbank_nor.svg";
		case WwiseObjectType::State:
			return "ObjectIcons_State_nor.svg";
		case WwiseObjectType::StateGroup:
			return "ObjectIcons_StateGroup_nor.svg";
		case WwiseObjectType::Switch:
			return "ObjectIcons_Switch_nor.svg";
		case WwiseObjectType::SwitchGroup:
			return "ObjectIcons_SwitchGroup_nor.svg";
		case WwiseObjectType::Workunit:
			return "ObjectIcons_Workunit_nor.svg";
		case WwiseObjectType::GameParameter:
			return "ObjectIcons_GameParameter_nor.svg";
		case WwiseObjectType::Trigger:
			return "ObjectIcons_Trigger_nor.svg";
		case WwiseObjectType::AcousticTexture:
			return "ObjectIcons_AcousticTexture_nor.svg";
		default:
			return "ObjectIcons_Project_nor.svg";
	}
}

String AkEditorUtils::get_theme_folder(bool dark_mode)
{
	return dark_mode ? "res://addons/Wwise/editor/theme/dark/images/" : "res://addons/Wwise/editor/theme/light/images/";
}
#pragma once

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>

using namespace godot;

class AkEditorUtils : public Object
{
	GDCLASS(AkEditorUtils, Object);

public:
	enum AkType
	{
		AKTYPE_EVENT,
		AKTYPE_BANK,
		AKTYPE_RTPC,
		AKTYPE_STATE_GROUP,
		AKTYPE_STATE,
		AKTYPE_SWITCH_GROUP,
		AKTYPE_SWITCH,
		AKTYPE_BUS,
		AKTYPE_AUX_BUS,
		AKTYPE_ACOUSTIC_TEXTURE
	};

	enum AkEditorIconType
	{
		AK_ICON_ACOUSTICTEXTURE,
		AK_ICON_AUXBUS,
		AK_ICON_BUS,
		AK_ICON_EVENT,
		AK_ICON_FOLDER,
		AK_ICON_GAMEPARAMETER,
		AK_ICON_PROJECT,
		AK_ICON_SOUNDBANK,
		AK_ICON_SOUNDBANK_INIT,
		AK_ICON_STATE,
		AK_ICON_STATEGROUP,
		AK_ICON_SWITCH,
		AK_ICON_SWITCHGROUP,
		AK_ICON_WORKUNIT,
		AK_ICON_LENGTH
	};

protected:
	static void _bind_methods() {}

private:
	static AkEditorUtils* singleton;
	Ref<Texture2D> cached_icons[AK_ICON_LENGTH]{};

public:
	static constexpr const char* get_ak_editor_icon_path(const AkEditorIconType icon_type)
	{
		switch (icon_type)
		{
			case AK_ICON_ACOUSTICTEXTURE:
				return "res://addons/Wwise/editor/images/AcousticTexture_nor.svg";
			case AK_ICON_AUXBUS:
				return "res://addons/Wwise/editor/images/AuxBus_nor.svg";
			case AK_ICON_BUS:
				return "res://addons/Wwise/editor/images/BusSimple_nor.svg";
			case AK_ICON_EVENT:
				return "res://addons/Wwise/editor/images/Event_nor.svg";
			case AK_ICON_FOLDER:
				return "res://addons/Wwise/editor/images/Folder_nor.svg";
			case AK_ICON_GAMEPARAMETER:
				return "res://addons/Wwise/editor/images/GameParameter_nor.svg";
			case AK_ICON_PROJECT:
				return "res://addons/Wwise/editor/images/Project_nor.svg";
			case AK_ICON_SOUNDBANK:
				return "res://addons/Wwise/editor/images/Soundbank_nor.svg";
			case AK_ICON_SOUNDBANK_INIT:
				return "res://addons/Wwise/editor/images/SoundbankInit_nor.svg";
			case AK_ICON_STATE:
				return "res://addons/Wwise/editor/images/State_nor.svg";
			case AK_ICON_STATEGROUP:
				return "res://addons/Wwise/editor/images/StateGroup_nor.svg";
			case AK_ICON_SWITCH:
				return "res://addons/Wwise/editor/images/Switch_nor.svg";
			case AK_ICON_SWITCHGROUP:
				return "res://addons/Wwise/editor/images/SwitchGroup_nor.svg";
			case AK_ICON_WORKUNIT:
				return "res://addons/Wwise/editor/images/Workunit_nor.svg";
			default:
				return "res://addons/Wwise/editor/images/Project_nor.svg";
		}
	}

	static constexpr AkEditorIconType get_ak_editor_icon_type(const AkType type)
	{
		switch (type)
		{
			case AKTYPE_EVENT:
				return AK_ICON_EVENT;
			case AKTYPE_BANK:
				return AK_ICON_SOUNDBANK;
			case AKTYPE_STATE_GROUP:
				return AK_ICON_STATEGROUP;
			case AKTYPE_STATE:
				return AK_ICON_STATE;
			case AKTYPE_SWITCH_GROUP:
				return AK_ICON_SWITCHGROUP;
			case AKTYPE_SWITCH:
				return AK_ICON_SWITCH;
			case AKTYPE_BUS:
				return AK_ICON_BUS;
			case AKTYPE_AUX_BUS:
				return AK_ICON_AUXBUS;
			case AKTYPE_RTPC:
				return AK_ICON_GAMEPARAMETER;
			case AKTYPE_ACOUSTIC_TEXTURE:
				return AK_ICON_ACOUSTICTEXTURE;
			default:
				return AK_ICON_PROJECT;
		}
	}

	AkEditorUtils();
	~AkEditorUtils();

	static AkEditorUtils* get_singleton();

	Ref<Texture2D> get_editor_icon(const AkType ak_type);
	Ref<Texture2D> get_editor_icon(const AkEditorIconType icon_type);
};
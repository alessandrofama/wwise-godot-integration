#include "ak_editor_utils.h"

using namespace godot;

AkEditorUtils* AkEditorUtils::singleton = nullptr;

AkEditorUtils::AkEditorUtils()
{
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

AkEditorUtils::~AkEditorUtils()
{
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}

AkEditorUtils* AkEditorUtils::get_singleton() { return singleton; }

Ref<Texture2D> AkEditorUtils::get_editor_icon(const AkType ak_type)
{
	const AkEditorIconType icon_type = get_ak_editor_icon_type(ak_type);
	return get_editor_icon(icon_type);
}

Ref<Texture2D> AkEditorUtils::get_editor_icon(const AkEditorIconType icon_type)
{
	std::unordered_map<AkEditorIconType, Ref<Texture2D>>::iterator it = cached_icons.find(icon_type);

	if (it != cached_icons.end())
	{
		return it->second;
	}

	const String icon_path = get_ak_editor_icon_path(icon_type);
	Ref<Texture2D> icon = ResourceLoader::get_singleton()->load(icon_path);

	if (icon.is_valid())
	{
		it = cached_icons.emplace(icon_type, icon).first;
		return it->second;
	}

	return Ref<Texture2D>();
}
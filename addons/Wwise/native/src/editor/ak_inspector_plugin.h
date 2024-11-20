#pragma once

#include "editor/ak_editor_utils.h"
#include <godot_cpp/classes/accept_dialog.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/script.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

using namespace godot;

class AkInspectorEditor;

class AkInspectorTree : public Tree
{
	GDCLASS(AkInspectorTree, Tree);

protected:
	static void _bind_methods();

private:
	TreeItem* root_item{};
	AkEditorUtils::AkType ak_type{ AkEditorUtils::AkType::AKTYPE_EVENT };
	Ref<Texture2D> icon{};
	AkInspectorEditor* window{};

	Dictionary get_wwise_ids(const AkEditorUtils::AkType ak_type);

public:
	LineEdit* search_text{};
	Dictionary user_data{};

	void initialize(const AkEditorUtils::AkType item_type, const Dictionary& user_data_ = Dictionary());
	void populate_browser(const String& text_filter);
	void _on_text_changed(const String& text_filter);
	void _on_size_changed();
};

class AkInspectorEditor : public AcceptDialog
{
	GDCLASS(AkInspectorEditor, AcceptDialog);

protected:
	static void _bind_methods(){};

public:
	VBoxContainer* root_vbox{};
	VBoxContainer* search_vbox{};
	LineEdit* search_text{};
	AkInspectorTree* tree{};
	void initialize();
};

class AkInspectorEditorProperty : public EditorProperty
{
	GDCLASS(AkInspectorEditorProperty, EditorProperty);

protected:
	static void _bind_methods();

private:
	struct AkInspectorEditorPropertyInfo
	{
		const char* text;
		const char* placeholder;
	};

	static constexpr AkInspectorEditorPropertyInfo get_ak_inspector_property_info(const AkEditorUtils::AkType type)
	{
		switch (type)
		{
			case AkEditorUtils::AkType::AKTYPE_EVENT:
				return AkInspectorEditorPropertyInfo{ "Select Event...", "Search Events..." };
			case AkEditorUtils::AkType::AKTYPE_BANK:
				return AkInspectorEditorPropertyInfo{ "Select Bank...", "Search Banks..." };
			case AkEditorUtils::AkType::AKTYPE_RTPC:
				return AkInspectorEditorPropertyInfo{ "Select Game Parameter...", "Search Game Parameters..." };
			case AkEditorUtils::AkType::AKTYPE_STATE_GROUP:
				return AkInspectorEditorPropertyInfo{ "Select State Group...", "Search State Groups..." };
			case AkEditorUtils::AkType::AKTYPE_STATE:
				return AkInspectorEditorPropertyInfo{ "Select State...", "Search States..." };
			case AkEditorUtils::AkType::AKTYPE_SWITCH_GROUP:
				return AkInspectorEditorPropertyInfo{ "Select Switch Group...", "Search Switch Groups..." };
			case AkEditorUtils::AkType::AKTYPE_SWITCH:
				return AkInspectorEditorPropertyInfo{ "Select Switch...", "Search Switches..." };
			case AkEditorUtils::AkType::AKTYPE_BUS:
				return AkInspectorEditorPropertyInfo{ "Select Bus...", "Search Busses..." };
			case AkEditorUtils::AkType::AKTYPE_AUX_BUS:
				return AkInspectorEditorPropertyInfo{ "Select Aux Bus...", "Search Aux Busses..." };
			case AkEditorUtils::AkType::AKTYPE_ACOUSTIC_TEXTURE:
				return AkInspectorEditorPropertyInfo{ "Select Acoustic Texture...", "Search Acoustic Textures..." };
			default:
				return AkInspectorEditorPropertyInfo{ "Default Select Label", "Default Search Label" };
		}
	}

	Button* property_control{};
	Ref<Texture2D> icon{};
	Dictionary current_value{};
	bool updating{};
	AkInspectorEditor* window{};
	AkEditorUtils::AkType ak_type{ AkEditorUtils::AkType::AKTYPE_EVENT };
	PopupMenu* event_popup{};
	Dictionary user_data{};

	void open_popup();
	void close_popup();

public:
	void init(const AkEditorUtils::AkType type, const Dictionary& user_data_ = Dictionary());
	virtual void _update_property() override;
	void reset();
	void _on_button_pressed();
	void _on_item_selected();
};

class AkInspectorEditorInspectorPlugin : public EditorInspectorPlugin
{
	GDCLASS(AkInspectorEditorInspectorPlugin, EditorInspectorPlugin);

protected:
	static void _bind_methods(){};

public:
	virtual bool _can_handle(Object* object) const override;
	virtual bool _parse_property(Object* object, Variant::Type type, const String& name, PropertyHint hint_type,
			const String& hint_string, BitField<PropertyUsageFlags> usage_flags, bool wide) override;
};
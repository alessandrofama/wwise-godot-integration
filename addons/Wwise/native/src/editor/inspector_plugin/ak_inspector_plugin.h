#ifndef AK_INSPECTOR_PLUGIN_H
#define AK_INSPECTOR_PLUGIN_H

#include "ak_utils.h"
#include "wwise_editor_scale.h"
#include <godot_cpp/classes/accept_dialog.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/script.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{

class AkInspectorEditor;

class AkInspectorTree : public Tree
{
	GDCLASS(AkInspectorTree, Tree);

protected:
	static void _bind_methods();

private:
	TreeItem* root_item{};
	AkUtils::AkType ak_type{ AkUtils::AkType::AKTYPE_EVENT };
	Ref<Texture2D> icon{};
	AkInspectorEditor* window{};

	Dictionary get_wwise_ids(const AkUtils::AkType ak_type);

public:
	LineEdit* search_text{};
	Dictionary user_data{};

	void initialize(const AkUtils::AkType item_type, const Dictionary& user_data_ = Dictionary());
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
	Button* property_control{};
	Ref<Texture2D> icon{};
	Dictionary current_value{};
	bool updating{};
	AkInspectorEditor* window{};
	AkUtils::AkType ak_type{ AkUtils::AkType::AKTYPE_EVENT };
	PopupMenu* event_popup{};
	Dictionary user_data{};

	void open_popup();
	void close_popup();

public:
	void init(const AkUtils::AkType type, const Dictionary& user_data_ = Dictionary());
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

} //namespace godot

#endif
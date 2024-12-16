#pragma once

#include "core/types/wwise_base_type.h"
#include "core/wwise_object_type.h"
#include "editor/ak_editor_utils.h"
#include "editor/ak_wwise_inspector_picker.h"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_property.hpp>

using namespace godot;

class AkWwiseEditorProperty : public EditorProperty
{
	GDCLASS(AkWwiseEditorProperty, EditorProperty);

protected:
	static void _bind_methods();
	void _notification(int p_what);

private:
	AkWwiseInspectorPicker* picker{ nullptr };

public:
	Ref<WwiseBaseType> current_value;
	Button* button{ nullptr };
	bool updating{};

	virtual String get_button_placeholder_text() const;
	virtual Ref<Texture2D> get_property_icon() const;
	virtual WwiseObjectType get_wwise_object_type() const;
	virtual void set_wwise_reference(const AkWwiseTreeItem* p_item) = 0;
	virtual void _update_property() override;
	virtual bool _can_drop_data(const Vector2& p_at_position, const Variant& p_data) const override;
	virtual void _drop_data(const Vector2& p_at_position, const Variant& p_data) override;
	void _on_button_pressed();
	void _on_wwise_tree_item_selected(const AkWwiseTreeItem* p_item);
};
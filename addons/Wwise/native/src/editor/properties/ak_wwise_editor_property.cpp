#include "ak_wwise_editor_property.h"

void AkWwiseEditorProperty::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("_on_button_pressed"), &AkWwiseEditorProperty::_on_button_pressed);
	ClassDB::bind_method(
			D_METHOD("_on_wwise_tree_item_selected"), &AkWwiseEditorProperty::_on_wwise_tree_item_selected);
}

void AkWwiseEditorProperty::_notification(int p_what)
{
	switch (p_what)
	{
		case NOTIFICATION_ENTER_TREE:
		{
			button = memnew(Button);
			button->set_clip_text(true);
			const String placeholder_text = get_button_placeholder_text();
			button->set_text(placeholder_text);
			add_child(button);
			button->connect("pressed", callable_mp(this, &AkWwiseEditorProperty::_on_button_pressed));
			button->set_drag_forwarding(Callable(), callable_mp(this, &AkWwiseEditorProperty::_can_drop_data),
					callable_mp(this, &AkWwiseEditorProperty::_drop_data));
			break;
		}
		case NOTIFICATION_READY:
		{
			WwiseProjectInfo::get_singleton()->connect(
					"ws_refresh_completed", callable_mp(this, &AkWwiseEditorProperty::_update_property));
			break;
		}
		case NOTIFICATION_EXIT_TREE:
		{
			WwiseProjectInfo::get_singleton()->disconnect(
					"ws_refresh_completed", callable_mp(this, &AkWwiseEditorProperty::_update_property));
			break;
		}
	}
}

String AkWwiseEditorProperty::get_button_placeholder_text() const { return "Default"; }

Ref<Texture2D> AkWwiseEditorProperty::get_property_icon() const
{
	return AkEditorUtils::get_editor_icon(get_wwise_object_type());
}

WwiseObjectType AkWwiseEditorProperty::get_wwise_object_type() const { return WwiseObjectType::Project; }

void AkWwiseEditorProperty::_update_property()
{
	Variant new_value = get_edited_object()->get(get_edited_property());

	if (new_value.get_type() == Variant::NIL)
	{
		get_edited_object()->notify_property_list_changed();
		return;
	}

	Ref<WwiseBaseType> base_type = new_value;
	if (base_type.is_null())
	{
		return;
	}

	updating = true;
	current_value = new_value;
	button->set_text(current_value->get_display_name());
	button->set_button_icon(get_property_icon());
	get_edited_object()->notify_property_list_changed();

	updating = false;
}

bool AkWwiseEditorProperty::_can_drop_data(const Vector2& p_at_position, const Variant& p_data) const
{
	AkWwiseTreeItem* item = Object::cast_to<AkWwiseTreeItem>(p_data);
	if (item && item->get_object_type() == get_wwise_object_type())
	{
		return true;
	}
	return false;
}

void AkWwiseEditorProperty::_drop_data(const Vector2& p_at_position, const Variant& p_data)
{
	AkWwiseTreeItem* item = Object::cast_to<AkWwiseTreeItem>(p_data);
	if (item)
	{
		set_wwise_reference(item);
	}
}

void AkWwiseEditorProperty::_on_button_pressed()
{
	if (updating)
	{
		return;
	}

	picker = memnew(AkWwiseInspectorPicker);
	add_child(picker);
	picker->tree->connect(
			"ws_wwise_property_activated", callable_mp(this, &AkWwiseEditorProperty::_on_wwise_tree_item_selected));
	picker->open(get_wwise_object_type(), get_global_mouse_position());
}

void AkWwiseEditorProperty::_on_wwise_tree_item_selected(const AkWwiseTreeItem* p_item)
{
	set_wwise_reference(p_item);
	picker->close();
}
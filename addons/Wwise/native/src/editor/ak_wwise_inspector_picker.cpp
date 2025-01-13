#include "ak_wwise_inspector_picker.h"

void AkWwiseInspectorPicker::_notification(int p_what)
{
	switch (p_what)
	{
		case NOTIFICATION_VISIBILITY_CHANGED:
		{
			if (!is_visible())
			{
				EditorInterface::get_singleton()->get_editor_settings()->set_project_metadata(
						"dialog_bounds", "wwise_picker", Rect2(get_position(), get_size()));
			}
		}
		break;
	}
}

AkWwiseInspectorPicker::AkWwiseInspectorPicker()
{
	set_name("Wwise Picker");
	set_title("Wwise Picker");
	set_disable_3d(true);
	set_exclusive(true);

	root_vbox = memnew(VBoxContainer);
	root_vbox->set_name("ParentVBoxContainer");
	root_vbox->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	root_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	add_child(root_vbox);

	search_vbox = memnew(VBoxContainer);
	search_vbox->set_name("TopVBoxContainer");
	root_vbox->add_child(search_vbox);

	search_text = memnew(LineEdit);
	search_text->set_name("SearchText");
	search_vbox->add_child(search_text);

	tree = memnew(AkWwiseTree);
	search_text->connect("text_changed", Callable(tree, "_on_search_text_changed").bind(false));
	root_vbox->add_child(tree);

	set_ok_button_text("Close");

	data_source = memnew(AkWwiseTreeDataSource);
}

void AkWwiseInspectorPicker::open(WwiseObjectType p_wwise_object_type, Point2i p_pos)
{
	data_source->init(tree, p_wwise_object_type);

	Rect2 saved_size = EditorInterface::get_singleton()->get_editor_settings()->get_project_metadata(
			"dialog_bounds", "wwise_picker", Rect2());

	const auto ed_scale = EditorInterface::get_singleton()->get_editor_scale();
	Size2i min_size = Size2(400, 600) * ed_scale;

	if (saved_size != Rect2())
	{
		set_min_size(min_size);
		popup_on_parent(saved_size);
	}
	else
	{
		Rect2i rect = Rect2i(Point2i(p_pos.x - min_size.x, p_pos.y), min_size);
		set_min_size(min_size);
		popup_on_parent(rect);
	}

	search_text->set_placeholder("Search...");
	search_text->grab_focus();
}

void AkWwiseInspectorPicker::close() { queue_free(); }
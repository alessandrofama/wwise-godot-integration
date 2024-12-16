#include "ak_wwise_tree.h"

void AkWwiseTree::_bind_methods()
{
	ClassDB::bind_method(
			D_METHOD("_on_search_text_changed", "text_filter", "is_browser"), &AkWwiseTree::_on_search_text_changed);
	ClassDB::bind_method(D_METHOD("_on_item_activated"), &AkWwiseTree::_on_item_activated);

	ADD_SIGNAL(MethodInfo("ws_wwise_property_activated", PropertyInfo(Variant::OBJECT, "ak_wwise_tree_item")));
}

AkWwiseTree::AkWwiseTree()
{
	set_name("AkWwiseTree");
	set_allow_reselect(true);
	set_hide_folding(false);
	set_hide_root(true);
	set_v_size_flags(Control::SIZE_EXPAND_FILL);
	connect("item_activated", callable_mp(this, &AkWwiseTree::_on_item_activated));
}

bool AkWwiseTree::update_filter(TreeItem* p_parent, bool is_browser)
{
	TreeItem* item = p_parent;
	if (!item)
	{
		item = get_root();
	}
	ERR_FAIL_NULL_V(item, false);

	bool keep_visible = false;
	for (TreeItem* child = item->get_first_child(); child; child = child->get_next())
	{
		keep_visible = update_filter(child, is_browser) || keep_visible;
	}

	if (filter.is_empty())
	{
		item->set_visible(true);

		if (is_browser)
		{
			bool is_not_a_child_of_root = true;
			TreeItem* root = get_root();

			for (int i = 0; i < root->get_child_count(); i++)
			{
				if (item == root->get_child(i))
				{
					is_not_a_child_of_root = false;
					break;
				}
			}

			item->set_collapsed(item != get_root() && is_not_a_child_of_root);
		}
		else
		{
			item->set_collapsed(item != get_root() && item != get_root()->get_first_child());
		}

		return true;
	}

	if (keep_visible)
	{
		item->set_collapsed(false);
	}
	else
	{
		keep_visible = item == get_root();
		keep_visible = keep_visible || _matches_all_search_tokens(item->get_text(0));
	}
	item->set_visible(keep_visible);
	return keep_visible;
}

void AkWwiseTree::collapse_all(TreeItem* p_parent)
{
	if (!p_parent)
	{
		return;
	}

	p_parent->set_collapsed(false);

	for (TreeItem* child = p_parent->get_first_child(); child; child = child->get_next())
	{
		child->set_collapsed(true);
		collapse_all(child);
	}
}

void AkWwiseTree::_on_search_text_changed(const String& text_filter, bool is_browser)
{
	const String searched_string = text_filter.to_lower();
	filter = searched_string.split(" ", false);
	update_filter(get_root(), is_browser);
}

bool AkWwiseTree::_matches_all_search_tokens(const String& p_text)
{
	if (filter.is_empty())
	{
		return false;
	}
	const String s = p_text.to_lower();

	for (const String& t : filter)
	{
		if (!s.contains(t))
		{
			return false;
		}
	}

	return true;
}

Variant AkWwiseTree::_get_drag_data(const Vector2& p_at_position)
{
	TreeItem* selected = get_selected();

	if (!selected)
	{
		return Variant();
	}

	AkWwiseTreeItem* wwise_tree_item = Object::cast_to<AkWwiseTreeItem>(selected->get_meta("ak_wwise_tree_item"));

	if (wwise_tree_item)
	{
		WwiseObjectType type = wwise_tree_item->get_object_type();

		if (type == WwiseObjectType::Event || type == WwiseObjectType::Soundbank || type == WwiseObjectType::Switch ||
				type == WwiseObjectType::State || type == WwiseObjectType::GameParameter ||
				type == WwiseObjectType::AuxBus || type == WwiseObjectType::Trigger ||
				type == WwiseObjectType::AcousticTexture)
		{
			Button* button = memnew(Button);
			button->set_text(wwise_tree_item->get_name());
			button->set_button_icon(AkEditorUtils::get_editor_icon(wwise_tree_item->get_object_type()));

			Control* control = memnew(Control);
			control->add_child(button);
			set_drag_preview(control);

			return wwise_tree_item;
		}
	}
	return Variant();
}

void AkWwiseTree::_on_item_activated()
{
	TreeItem* selected = get_selected();
	AkWwiseTreeItem* wwise_tree_item = Object::cast_to<AkWwiseTreeItem>(selected->get_meta("ak_wwise_tree_item"));

	if (wwise_tree_item)
	{
		switch (wwise_tree_item->get_object_type())
		{
			case WwiseObjectType::AuxBus:
			case WwiseObjectType::Event:
			case WwiseObjectType::Soundbank:
			case WwiseObjectType::State:
			case WwiseObjectType::Switch:
			case WwiseObjectType::GameParameter:
			case WwiseObjectType::AcousticTexture:
			case WwiseObjectType::Trigger:
				emit_signal("ws_wwise_property_activated", wwise_tree_item);
				break;
			default:
				break;
		}
	}
}
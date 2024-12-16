#include "ak_wwise_tree_item.h"

void AkWwiseTreeItem::_bind_methods() {}

void AkWwiseTreeItem::init(AkWwiseTree* p_tree, TreeItem* p_tree_item, const String& p_display_name,
		const String& p_obj_guid, WwiseObjectType p_type)
{
	display_name = p_display_name;
	object_type = p_type;
	guid = p_obj_guid;

	if (p_tree)
	{
		tree_item = p_tree->create_item();
	}
	else if (p_tree_item)
	{
		tree_item = p_tree_item->create_child();
		tree_item->set_collapsed(true);
	}
	else
	{
		ERR_PRINT("AkWwiseTreeItem::init: Both p_tree and p_parent_tree_item are null.");
		return;
	}

	tree_item->set_text(0, display_name);
	tree_item->set_meta("ak_wwise_tree_item", this);
	tree_item->set_icon(0, AkEditorUtils::get_editor_icon(p_type));
}

void AkWwiseTreeItem::set_name(const String& p_name) { display_name = p_name; }

String AkWwiseTreeItem::get_name() const { return display_name; }

void AkWwiseTreeItem::set_object_type(WwiseObjectType p_type) { object_type = p_type; }

WwiseObjectType AkWwiseTreeItem::get_object_type() const { return object_type; }

void AkWwiseTreeItem::set_guid(const String& p_guid) { guid = p_guid; }

String AkWwiseTreeItem::get_guid() const { return guid; }

void AkWwiseTreeItem::add_wwise_item_child(AkWwiseTreeItem* p_child)
{
	if (tree_item && p_child && p_child->tree_item)
	{
		if (p_child->tree_item->get_parent())
		{
			p_child->tree_item->get_parent()->remove_child(p_child->tree_item);
		}

		tree_item->add_child(p_child->tree_item);
	}
}
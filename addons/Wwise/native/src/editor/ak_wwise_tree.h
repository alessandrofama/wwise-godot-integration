#pragma once

#include "core/wwise_object_type.h"
#include "editor/ak_editor_utils.h"
#include "editor/ak_wwise_tree_item.h"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/tree.hpp>

using namespace godot;

class AkWwiseTree : public Tree
{
	GDCLASS(AkWwiseTree, Tree);

protected:
	static void _bind_methods();

private:
	PackedStringArray filter;

public:
	AkWwiseTree();
	bool update_filter(TreeItem* p_parent, bool is_browser = false);
	void collapse_all(TreeItem* p_parent);
	void _on_search_text_changed(const String& text_filter, bool is_browser = false);
	bool _matches_all_search_tokens(const String& p_text);
	virtual Variant _get_drag_data(const Vector2& p_at_position) override;
	void _on_item_activated();
};
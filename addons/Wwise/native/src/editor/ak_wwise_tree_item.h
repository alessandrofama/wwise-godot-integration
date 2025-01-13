#pragma once

#include "core/wwise_object_type.h"
#include "editor/ak_editor_utils.h"
#include <godot_cpp/classes/tree_item.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/method_bind.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

class AkWwiseTree;

using namespace godot;

class AkWwiseTreeItem : public Object
{
	GDCLASS(AkWwiseTreeItem, Object);

protected:
	static void _bind_methods();

private:
	WwiseObjectType object_type{ WwiseObjectType::None };
	String display_name;
	String guid;
	bool is_sorted{};

public:
	TreeItem* tree_item;

	bool operator==(const AkWwiseTreeItem& other) const
	{
		return guid == other.guid && display_name == other.display_name && object_type == other.object_type;
	};

	void init(AkWwiseTree* p_tree, TreeItem* p_tree_item, const String& p_display_name, const String& p_obj_guid,
			WwiseObjectType p_type);

	void set_name(const String& p_name);
	String get_name() const;

	void set_object_type(WwiseObjectType p_type);
	WwiseObjectType get_object_type() const;

	void set_guid(const String& p_guid);
	String get_guid() const;

	void add_wwise_item_child(AkWwiseTreeItem* p_child);
};
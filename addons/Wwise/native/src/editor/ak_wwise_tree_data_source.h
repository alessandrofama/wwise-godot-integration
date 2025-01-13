#pragma once

#include "editor/ak_wwise_tree.h"
#include "editor/ak_wwise_tree_item.h"
#include "editor/plugins/wwise_project_info.h"
#include "editor/wwise_tree_object.h"
#include <godot_cpp/variant/utility_functions.hpp>

class AkWwiseTreeDataSource : public Object
{
	GDCLASS(AkWwiseTreeDataSource, Object)

protected:
	static void _bind_methods() {};

private:
	AkWwiseTree* tree{ nullptr };

public:
	AkWwiseTreeItem* project_root{ nullptr };

	void init(AkWwiseTree* p_tree, WwiseObjectType p_picker_type);
	void init_browser_tree(AkWwiseTree* p_tree);
	AkWwiseTreeItem* create_project_root_item();
	AkWwiseTreeItem* build_object_type_tree(WwiseObjectType p_object_type);
	AkWwiseTreeItem* build_tree(const String& p_name, const Array& p_wwise_tree_objects);
	AkWwiseTreeItem* build_tree(AkWwiseTreeItem* p_tree_item, const Array& p_children_info);
};
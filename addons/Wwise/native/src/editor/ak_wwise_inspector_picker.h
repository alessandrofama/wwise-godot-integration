#pragma once

#include "core/wwise_object_type.h"
#include "editor/ak_editor_utils.h"
#include "editor/ak_wwise_tree.h"
#include "editor/ak_wwise_tree_data_source.h"
#include <godot_cpp/classes/accept_dialog.hpp>
#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_settings.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

using namespace godot;

class AkWwiseInspectorPicker : public AcceptDialog
{
	GDCLASS(AkWwiseInspectorPicker, AcceptDialog);

protected:
	static void _bind_methods(){};
	void _notification(int p_what);

private:
	AkWwiseTreeDataSource* data_source{ nullptr };
	WwiseObjectType wwise_object_type;

public:
	AkWwiseInspectorPicker();
	VBoxContainer* root_vbox{ nullptr };
	VBoxContainer* search_vbox{ nullptr };
	LineEdit* search_text{ nullptr };
	AkWwiseTree* tree{ nullptr };
	void open(WwiseObjectType p_wwise_object_type, Point2i p_pos);
	void close();
};
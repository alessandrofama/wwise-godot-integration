#pragma once

#if defined(AK_WIN) || defined(AK_MAC_OS_X)
#include "core/waapi_gdextension.h"
#endif
#include "editor/ak_editor_utils.h"
#include "editor/ak_wwise_tree_data_source.h"
#include "editor/plugins/wwise_project_info.h"
#include <godot_cpp/classes/editor_file_system.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/script.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

using namespace godot;

class WwiseBrowser : public EditorPlugin
{
	GDCLASS(WwiseBrowser, EditorPlugin);

protected:
	static void _bind_methods();

private:
	AkWwiseTreeDataSource* data_source{ nullptr };
	AkWwiseTree* tree{ nullptr };
	String filter;
	WwiseObjectType wwise_object_type;
	String output;

	bool ensure_wwise_connection();
	bool generate_soundbanks();
	void generate_ids(const String& class_type, const Array& p_parent);
	void process_children(const Array& p_parent, int level);
	void populate_tree();
	String sanitize_name(const String& p_name);
	bool has_aux_busses(const Array& p_bus_array);

public:
	virtual void _enter_tree() override;
	virtual void _exit_tree() override;
	void _on_refresh_project_button_up();
	void _on_generate_soundbanks_button_up();
	void _on_generate_ids_button_up();
};
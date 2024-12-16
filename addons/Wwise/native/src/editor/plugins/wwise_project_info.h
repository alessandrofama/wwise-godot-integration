#pragma once

#include "core/types/wwise_acoustic_texture.h"
#include "core/types/wwise_aux_bus.h"
#include "core/types/wwise_bank.h"
#include "core/types/wwise_base_type.h"
#include "core/types/wwise_event.h"
#include "core/types/wwise_rtpc.h"
#include "core/types/wwise_state.h"
#include "core/types/wwise_state_group.h"
#include "core/types/wwise_switch.h"
#include "core/types/wwise_switch_group.h"
#include "core/types/wwise_trigger.h"
#include "core/utils.h"
#include "editor/ak_editor_settings.h"
#include "editor/plugins/ak_json_builder.h"
#include "editor/wwise_project_data.h"
#include <godot_cpp/classes/editor_file_system.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/worker_thread_pool.hpp>

using namespace godot;

class WwiseProjectInfo : public EditorPlugin
{
	GDCLASS(WwiseProjectInfo, EditorPlugin);

protected:
	static void _bind_methods();

private:
	static WwiseProjectInfo* singleton;

	Ref<WwiseProjectData> project_data;

	bool running{ false };
	int64_t worker_group_id{ 0 };

	static void save_project_data(const Ref<WwiseProjectData>& data);
	void update_wwise_object(WwiseObjectType type, const String& name, const String& guid);
	void update_assets_worker(int index);
	void update_all_wwise_assets();
	void _on_populate_completed();
	void _on_updating_assets_completed();

public:
	WwiseProjectInfo();
	~WwiseProjectInfo();
	Array objects_to_update;
	virtual void _enter_tree();
	virtual void _exit_tree();
	virtual void _process(double p_delta);

	static WwiseProjectInfo* get_singleton();

	void set_project_data(const Ref<WwiseProjectData>& p_project_data);
	Ref<WwiseProjectData> get_project_data() const;

	Ref<WwiseProjectData> get_data();
	void create_wwise_project_data();
	void refresh();
};
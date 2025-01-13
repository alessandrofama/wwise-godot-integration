#pragma once

#include "editor/wwise_project_database.h"
#include "editor/plugins/wwise_project_info.h"
#include "editor/wwise_tree_object.h"
#include "editor/wwise_tree_object_event.h"
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/worker_thread_pool.hpp>
#include <vector>

using namespace godot;

class AkJSONBuilder : public EditorPlugin
{
	GDCLASS(AkJSONBuilder, EditorPlugin)

protected:
	static void _bind_methods();

private:
	const static WwisePlatformDataStructure* current_platform_data;
	static CAkLock ak_lock;
	struct TaskGroup
	{
		int id;
		bool completed;
	};

	static std::vector<TaskGroup> active_task_groups;
	static bool all_tasks_completed;
	static bool running;

	static Array build_folder_hierarchy_recursive(PackedStringArray p_folders, Array p_work_units);
	static Array build_folder_hierarchy(const String& p_path, Array p_work_units);
	static bool is_duplicate(const Array& p_work_unit, const String& p_name);

	template <typename MetadataType, typename ObjectType = WwiseTreeObject>
	static void serialise_metadata(const MetadataType* p_metadata, WwiseObjectType p_type,
			std::function<Array()> p_get_root_func, std::function<void(Ref<ObjectType>)> p_configure_object = nullptr);

	static void process_soundbank_worker(int p_index);
	static void process_event_worker(int p_index);
	static void process_bus_worker(int p_index);
	static void process_aux_bus_worker(int p_index);
	static void process_acoustic_texture_worker(int p_index);
	static void process_state_group_worker(int p_index);
	static void process_switch_group_worker(int p_index);
	static void process_game_parameter_worker(int p_index);
	static void process_trigger_worker(int p_index);

public:
	virtual void _process(double p_delta);
	static void populate();
};
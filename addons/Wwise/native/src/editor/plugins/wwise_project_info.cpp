#include "wwise_project_info.h"

WwiseProjectInfo* WwiseProjectInfo::singleton = nullptr;

void WwiseProjectInfo::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_project_data", "project_data"), &WwiseProjectInfo::set_project_data);
	ClassDB::bind_method(D_METHOD("get_project_data"), &WwiseProjectInfo::get_project_data);
	ClassDB::bind_method(D_METHOD("refresh"), &WwiseProjectInfo::refresh);
	ClassDB::bind_method(D_METHOD("_on_populate_completed"), &WwiseProjectInfo::_on_populate_completed);
	ClassDB::bind_method(D_METHOD("update_assets_worker", "index"), &WwiseProjectInfo::update_assets_worker);
	ClassDB::bind_method(D_METHOD("_on_updating_assets_completed"), &WwiseProjectInfo::_on_updating_assets_completed);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "project_data", PROPERTY_HINT_RESOURCE_TYPE, "WwiseProjectData"),
			"set_project_data", "get_project_data");
	ADD_SIGNAL(MethodInfo("ws_populate_completed"));
	ADD_SIGNAL(MethodInfo("ws_refresh_completed"));
	ADD_SIGNAL(MethodInfo("ws_updating_assets_completed"));
}

void WwiseProjectInfo::_enter_tree()
{
	WwiseProjectDatabase::get_singleton()->connect(
			"ws_soundbank_directory_updated", callable_mp(this, &WwiseProjectInfo::refresh));
	connect("ws_populate_completed", callable_mp(this, &WwiseProjectInfo::_on_populate_completed), 1);
	connect("ws_updating_assets_completed", callable_mp(this, &WwiseProjectInfo::_on_updating_assets_completed), 1);

	const String project_data_path = AkEditorSettings::get_project_data_path();

	if (!FileAccess::file_exists(project_data_path))
	{
		create_wwise_project_data();
	}
}

void WwiseProjectInfo::_exit_tree()
{
	WwiseProjectDatabase::get_singleton()->disconnect(
			"ws_soundbank_directory_updated", callable_mp(this, &WwiseProjectInfo::refresh));
	disconnect("ws_populate_completed", callable_mp(this, &WwiseProjectInfo::_on_populate_completed));
	disconnect("ws_updating_assets_completed", callable_mp(this, &WwiseProjectInfo::_on_updating_assets_completed));
}

void WwiseProjectInfo::_process(double p_delta)
{
	if (!running)
		return;

	bool completed = true;

	if (WorkerThreadPool::get_singleton()->is_group_task_completed(worker_group_id))
	{
		WorkerThreadPool::get_singleton()->wait_for_group_task_completion(worker_group_id);
	}
	else
	{
		completed = false;
	}

	if (completed)
	{
		running = false;
		emit_signal("ws_updating_assets_completed");
	}
}

WwiseProjectInfo* WwiseProjectInfo::get_singleton() { return singleton; }

void WwiseProjectInfo::set_project_data(const Ref<WwiseProjectData>& p_project_data) { project_data = p_project_data; }

Ref<WwiseProjectData> WwiseProjectInfo::get_project_data() const { return project_data; }

void WwiseProjectInfo::save_project_data(const Ref<WwiseProjectData>& data)
{
	const String path = AkEditorSettings::get_project_data_path();
	make_dir_recursive(path.get_base_dir());
	const auto error = ResourceSaver::get_singleton()->save(data, path);
	if (error != Error::OK)
	{
		UtilityFunctions::push_error("WwiseGodot: Failed to save WwiseProjectData.tres.");
	}
}

Ref<WwiseProjectData> WwiseProjectInfo::get_data()
{
	const String project_data_path = AkEditorSettings::get_project_data_path();

	if (project_data.is_null())
	{
		if (FileAccess::file_exists(project_data_path))
		{
			project_data = ResourceLoader::get_singleton()->load(
					project_data_path, String(), ResourceLoader::CacheMode::CACHE_MODE_REPLACE_DEEP);
		}
		else
		{
			create_wwise_project_data();
		}
	}

	return project_data;
}

void WwiseProjectInfo::create_wwise_project_data()
{
	if (project_data.is_null())
		project_data.instantiate();

	save_project_data(project_data);

	project_data = ResourceLoader::get_singleton()->load(
			AkEditorSettings::get_project_data_path(), String(), ResourceLoader::CacheMode::CACHE_MODE_REPLACE_DEEP);
}

void WwiseProjectInfo::refresh()
{
	objects_to_update.clear();
	Ref<WwiseProjectData> data = get_data();
	data->reset();
	AkJSONBuilder::populate();
}

void WwiseProjectInfo::update_assets_worker(int index)
{
	Ref<WwiseTreeObject> tree_object = objects_to_update[index];

	update_wwise_object((WwiseObjectType)tree_object->get_type(), tree_object->get_name(), tree_object->get_guid());
}

void WwiseProjectInfo::update_wwise_object(WwiseObjectType type, const String& name, const String& guid)
{
	switch (type)
	{
		case WwiseObjectType::Event:
			WwiseBaseType::update_wwise_object<WwiseEvent>(type, name, guid);
			break;
		case WwiseObjectType::AuxBus:
			WwiseBaseType::update_wwise_object<WwiseAuxBus>(type, name, guid);
			break;
		case WwiseObjectType::Soundbank:
			WwiseBaseType::update_wwise_object<WwiseBank>(type, name, guid);
			break;
		case WwiseObjectType::AcousticTexture:
			WwiseBaseType::update_wwise_object<WwiseAcousticTexture>(type, name, guid);
			break;
		case WwiseObjectType::GameParameter:
			WwiseBaseType::update_wwise_object<WwiseRTPC>(type, name, guid);
			break;
		case WwiseObjectType::StateGroup:
			WwiseBaseType::update_wwise_object<WwiseStateGroup>(type, name, guid);
			break;
		case WwiseObjectType::State:
			WwiseBaseType::update_wwise_object<WwiseState>(type, name, guid);
			break;
		case WwiseObjectType::SwitchGroup:
			WwiseBaseType::update_wwise_object<WwiseSwitchGroup>(type, name, guid);
			break;
		case WwiseObjectType::Switch:
			WwiseBaseType::update_wwise_object<WwiseSwitch>(type, name, guid);
			break;
		case WwiseObjectType::Trigger:
			WwiseBaseType::update_wwise_object<WwiseTrigger>(type, name, guid);
			break;
		default:
			break;
	}
}

void WwiseProjectInfo::update_all_wwise_assets()
{
	Ref<WwiseProjectData> data = get_data();
	if (!data.is_valid())
	{
		UtilityFunctions::printerr("WwiseGodot: Trying to update Godot Assets, but WwiseProjectData.tres is null!");
		return;
	}

	Callable task_callable = Callable(this, "update_assets_worker");
	worker_group_id = WorkerThreadPool::get_singleton()->add_group_task(
			task_callable, objects_to_update.size(), -1, false, "WwiseGodot: Updating Godot Assets");

	running = true;
}

void WwiseProjectInfo::_on_populate_completed()
{
	project_data->sort();
	save_project_data(project_data);

	const String project_data_path = AkEditorSettings::get_project_data_path();
	project_data = ResourceLoader::get_singleton()->load(
			project_data_path, String(), ResourceLoader::CacheMode::CACHE_MODE_REPLACE_DEEP);
	EditorInterface::get_singleton()->get_resource_filesystem()->update_file(project_data_path);

	update_all_wwise_assets();
}

void WwiseProjectInfo::_on_updating_assets_completed()
{
	EditorInterface::get_singleton()->get_resource_filesystem()->scan();
	emit_signal("ws_refresh_completed");
	UtilityFunctions::print("WwiseGodot: Project has been refreshed successfully.");
}

WwiseProjectInfo::WwiseProjectInfo()
{
	if (singleton != nullptr)
		return;
	singleton = this;
}

WwiseProjectInfo::~WwiseProjectInfo()
{
	if (singleton != this)
		return;
	singleton = nullptr;
}
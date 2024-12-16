#include "ak_json_builder.h"

const WwisePlatformDataStructure* AkJSONBuilder::current_platform_data = nullptr;
CAkLock AkJSONBuilder::ak_lock;
std::vector<AkJSONBuilder::TaskGroup> AkJSONBuilder::active_task_groups;
bool AkJSONBuilder::all_tasks_completed;
bool AkJSONBuilder::running;

void AkJSONBuilder::_bind_methods() {}

Array AkJSONBuilder::build_folder_hierarchy_recursive(PackedStringArray p_folders, Array p_work_units)
{
	if (p_folders.size() == 0)
	{
		return p_work_units;
	}

	String folder_name = p_folders[0];
	if (folder_name.length() == 0)
	{
		return p_work_units;
	}

	for (int i = 0; i < p_work_units.size(); i++)
	{
		Ref<WwiseTreeObject> unit = p_work_units[i];
		ERR_FAIL_COND_V(unit.is_null(), Array());
		if (unit->get_name() == folder_name)
		{
			p_folders = skip(p_folders, 1);
			return build_folder_hierarchy_recursive(p_folders, unit->get_children());
		}
	}

	Array current_work_units = p_work_units;

	for (int i = 0; i < p_folders.size(); i++)
	{
		Ref<WwiseTreeObject> new_work_unit;
		new_work_unit.instantiate();
		new_work_unit->set_name(p_folders[i]);
		new_work_unit->set_children(Array());
		current_work_units.push_back(new_work_unit);
		Ref<WwiseTreeObject> tmp = current_work_units[current_work_units.size() - 1];
		current_work_units = tmp->get_children();
	}

	return current_work_units;
}

Array AkJSONBuilder::build_folder_hierarchy(const String& p_path, Array p_work_units)
{
	PackedStringArray folders = p_path.split("\\");
	if (folders.size() < 2)
	{
		return p_work_units;
	}

	folders = skip(folders, 2);
	folders = take(folders, folders.size() - 1);

	return build_folder_hierarchy_recursive(folders, p_work_units);
}

bool AkJSONBuilder::is_duplicate(const Array& p_work_unit, const String& p_name)
{
	for (int64_t i = 0; i < p_work_unit.size(); i++)
	{
		Ref<WwiseTreeObject> info = p_work_unit[i];
		if (info.is_valid())
		{
			if (info->get_name() == p_name)
			{
				return true;
			}

			if (is_duplicate(info->get_children(), p_name))
			{
				return true;
			}
		}
	}
	return false;
}

template <typename MetadataType, typename ObjectType>
void AkJSONBuilder::serialise_metadata(const MetadataType* p_metadata, WwiseObjectType p_type,
		std::function<Array()> p_get_root_func, std::function<void(Ref<ObjectType>)> p_configure_object)
{
	if (!p_metadata)
		return;

	String name;
	String path;
	String guid;

	if constexpr (std::is_same<MetadataType, WwiseMetadataSoundBank>::value)
	{
		name = p_metadata->ShortName.String.c_str();
	}
	else
	{
		name = p_metadata->Name.String.c_str();
	}

	path = p_metadata->ObjectPath.String.c_str();
	guid = p_metadata->GUID.Guid.ToString().c_str();

	Ref<ObjectType> object;
	object.instantiate();
	object->set_name(name);
	object->set_path(path);
	object->set_guid(guid);
	object->set_type((int)p_type);

	if (p_configure_object)
	{
		p_configure_object(object);
	}

	{
		AkAutoLock<CAkLock> scoped_lock(ak_lock);
		Array work_unit = build_folder_hierarchy(path, p_get_root_func());
		if (!is_duplicate(work_unit, name))
		{
			work_unit.push_back(object);
			WwiseProjectInfo::get_singleton()->objects_to_update.push_back(object);
		}
	}
}
void AkJSONBuilder::process_soundbank_worker(int p_index)
{
	if (!current_platform_data)
		return;

	const auto& soundbank_map = current_platform_data->SoundBanks;

	if (p_index < 0 || p_index >= static_cast<int>(soundbank_map.Size()))
		return;

	auto it = soundbank_map.begin();
	std::advance(it, p_index);

	const auto& soundbank_value = it->second;

	const auto& wwise_item = soundbank_value.GetSoundBank();
	if (wwise_item->Type != EMetadataSoundBankType::User || wwise_item->IsInitBank())
		return;

	serialise_metadata(wwise_item, WwiseObjectType::Soundbank,
			[]() { return WwiseProjectInfo::get_singleton()->get_data()->get_bank_root(); });
}

void AkJSONBuilder::process_event_worker(int p_index)
{
	if (!current_platform_data)
		return;

	const auto& event_map = current_platform_data->Events;

	if (p_index < 0 || p_index >= static_cast<int>(event_map.Size()))
		return;

	auto it = event_map.begin();
	std::advance(it, p_index);

	const auto& wwise_item = it->second.GetEvent();
	serialise_metadata<WwiseMetadataEvent, WwiseTreeObjectEvent>(
			wwise_item, WwiseObjectType::Event,
			[]() { return WwiseProjectInfo::get_singleton()->get_data()->get_event_root(); },
			[wwise_item](Ref<WwiseTreeObjectEvent> event_object)
			{
				event_object->set_max_attenuation(wwise_item->MaxAttenuation);
				event_object->set_min_duration(wwise_item->DurationMin);
				event_object->set_max_duration(wwise_item->DurationMax);
			});
}

void AkJSONBuilder::process_bus_worker(int p_index)
{
	if (!current_platform_data)
		return;

	const auto& bus_map = current_platform_data->Busses;
	if (p_index < 0 || p_index >= static_cast<int>(bus_map.Size()))
		return;

	auto it = bus_map.begin();
	std::advance(it, p_index);

	const auto& wwise_item = it->second.GetBus();
	serialise_metadata(wwise_item, WwiseObjectType::Bus,
			[]() { return WwiseProjectInfo::get_singleton()->get_data()->get_bus_root(); });
}

void AkJSONBuilder::process_aux_bus_worker(int p_index)
{
	if (!current_platform_data)
		return;

	const auto& aux_bus_map = current_platform_data->AuxBusses;
	if (p_index < 0 || p_index >= static_cast<int>(aux_bus_map.Size()))
		return;

	auto it = aux_bus_map.begin();
	std::advance(it, p_index);

	const auto& wwise_item = it->second.GetAuxBus();
	serialise_metadata(wwise_item, WwiseObjectType::AuxBus,
			[]() { return WwiseProjectInfo::get_singleton()->get_data()->get_bus_root(); });
}

void AkJSONBuilder::process_acoustic_texture_worker(int p_index)
{
	if (!current_platform_data)
		return;

	const auto& acoustic_texture_map = current_platform_data->AcousticTextures;
	if (p_index < 0 || p_index >= static_cast<int>(acoustic_texture_map.Size()))
		return;

	auto it = acoustic_texture_map.begin();
	std::advance(it, p_index);

	const auto& wwise_item = it->second.GetAcousticTexture();
	serialise_metadata(wwise_item, WwiseObjectType::AcousticTexture,
			[]() { return WwiseProjectInfo::get_singleton()->get_data()->get_acoustic_texture_root(); });
}

void AkJSONBuilder::process_state_group_worker(int p_index)
{
	if (!current_platform_data)
		return;

	const auto& state_group_map = current_platform_data->StateGroups;
	if (p_index < 0 || p_index >= static_cast<int>(state_group_map.Size()))
		return;

	auto it = state_group_map.begin();
	std::advance(it, p_index);

	const auto& wwise_item = it->second.GetStateGroup();
	serialise_metadata(wwise_item, WwiseObjectType::StateGroup,
			[]() { return WwiseProjectInfo::get_singleton()->get_data()->get_state_root(); });

	for (const auto& state : wwise_item->States)
	{
		serialise_metadata(&state, WwiseObjectType::State,
				[]() { return WwiseProjectInfo::get_singleton()->get_data()->get_state_root(); });
	}
}

void AkJSONBuilder::process_switch_group_worker(int p_index)
{
	if (!current_platform_data)
		return;

	const auto& switch_group_map = current_platform_data->SwitchGroups;
	if (p_index < 0 || p_index >= static_cast<int>(switch_group_map.Size()))
		return;

	auto it = switch_group_map.begin();
	std::advance(it, p_index);

	const auto& wwise_item = it->second.GetSwitchGroup();
	serialise_metadata(wwise_item, WwiseObjectType::SwitchGroup,
			[]() { return WwiseProjectInfo::get_singleton()->get_data()->get_switch_root(); });

	for (const auto& _switch : wwise_item->Switches)
	{
		serialise_metadata(&_switch, WwiseObjectType::Switch,
				[]() { return WwiseProjectInfo::get_singleton()->get_data()->get_switch_root(); });
	}
}

void AkJSONBuilder::process_game_parameter_worker(int p_index)
{
	if (!current_platform_data)
		return;

	const auto& game_parameter_map = current_platform_data->GameParameters;
	if (p_index < 0 || p_index >= static_cast<int>(game_parameter_map.Size()))
		return;

	auto it = game_parameter_map.begin();
	std::advance(it, p_index);

	const auto& wwise_item = it->second.GetGameParameter();
	serialise_metadata(wwise_item, WwiseObjectType::GameParameter,
			[]() { return WwiseProjectInfo::get_singleton()->get_data()->get_game_parameter_root(); });
}

void AkJSONBuilder::process_trigger_worker(int p_index)
{
	if (!current_platform_data)
		return;

	const auto& trigger_map = current_platform_data->Triggers;
	if (p_index < 0 || p_index >= static_cast<int>(trigger_map.Size()))
		return;

	auto it = trigger_map.begin();
	std::advance(it, p_index);

	const auto& wwise_item = it->second.GetTrigger();
	serialise_metadata(wwise_item, WwiseObjectType::Trigger,
			[]() { return WwiseProjectInfo::get_singleton()->get_data()->get_trigger_root(); });
}

void AkJSONBuilder::populate()
{
	const auto* PlatformData = DataStructure.Platforms.Find(CurrentPlatform);
	if (!PlatformData)
	{
		UtilityFunctions::printerr("WwiseGodot: AkJSONBuilder: Platform Data not found!");
		return;
	}

	current_platform_data = PlatformData;

	active_task_groups.clear();

	struct TaskConfig
	{
		Callable task_callable;
		unsigned int size;
		String description;
	};

	TaskConfig tasks[] = { { callable_mp_static(AkJSONBuilder::process_soundbank_worker),
								   current_platform_data->SoundBanks.Size(), "WwiseGodot: Processing SoundBanks" },
		{ callable_mp_static(AkJSONBuilder::process_event_worker), current_platform_data->Events.Size(),
				"WwiseGodot: Processing Events" },
		{ callable_mp_static(AkJSONBuilder::process_bus_worker), current_platform_data->Busses.Size(),
				"WwiseGodot: Processing Busses" },
		{ callable_mp_static(AkJSONBuilder::process_aux_bus_worker), current_platform_data->AuxBusses.Size(),
				"WwiseGodot: Processing Aux Busses" },
		{ callable_mp_static(AkJSONBuilder::process_acoustic_texture_worker),
				current_platform_data->AcousticTextures.Size(), "WwiseGodot: Processing Acoustic Textures" },
		{ callable_mp_static(AkJSONBuilder::process_state_group_worker), current_platform_data->StateGroups.Size(),
				"WwiseGodot: Processing State Groups" },
		{ callable_mp_static(AkJSONBuilder::process_switch_group_worker), current_platform_data->SwitchGroups.Size(),
				"WwiseGodot: Processing Switch Groups" },
		{ callable_mp_static(AkJSONBuilder::process_game_parameter_worker),
				current_platform_data->GameParameters.Size(), "WwiseGodot: Processing Game Parameters" },
		{ callable_mp_static(AkJSONBuilder::process_trigger_worker), current_platform_data->Triggers.Size(),
				"WwiseGodot: Processing Triggers" } };

	for (const auto& task : tasks)
	{
		if (task.size > 0)
		{
			int group_id = WorkerThreadPool::get_singleton()->add_group_task(
					task.task_callable, task.size, -1, false, task.description);
			active_task_groups.push_back({ group_id, false });
		}
	}

	all_tasks_completed = false;
	running = true;
}

void AkJSONBuilder::_process(double p_delta)
{
	if (!running)
		return;

	if (all_tasks_completed)
		return;

	bool all_completed = true;

	for (TaskGroup& group : active_task_groups)
	{
		if (!group.completed)
		{
			if (WorkerThreadPool::get_singleton()->is_group_task_completed(group.id))
			{
				group.completed = true;
				WorkerThreadPool::get_singleton()->wait_for_group_task_completion(group.id);
			}
			else
			{
				all_completed = false;
			}
		}
	}

	if (all_completed)
	{
		all_tasks_completed = true;
		WwiseProjectInfo::get_singleton()->emit_signal("ws_populate_completed");
		current_platform_data = nullptr;
		running = false;
	}
}
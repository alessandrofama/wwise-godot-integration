#include "ak_environment_data.h"

void AkAuxArrayData::set_values(Node* event_node)
{
	Wwise::get_singleton()->set_game_object_aux_send_values(event_node, data, data.size());
}

void AkEnvironmentData::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("compare_by_priority", "a", "b"), &AkEnvironmentData::compare_by_priority);
}

void AkEnvironmentData::add_environment(const AkEnvironment* env)
{
	if (env)
	{
		int64_t index =
				active_environments.bsearch_custom(env, callable_mp(this, &AkEnvironmentData::compare_by_priority));
		active_environments.insert(index, env);
		have_environments_changed = true;
	}
}

void AkEnvironmentData::remove_environment(const AkEnvironment* env)
{
	if (env)
	{
		int64_t index = active_environments.find(env);
		if (index != -1)
		{
			active_environments.remove_at(index);
			have_environments_changed = true;
		}
	}
}

bool AkEnvironmentData::compare_by_priority(const AkEnvironment* a, const AkEnvironment* b)
{
	if (a->get_priority() < b->get_priority())
	{
		return true;
	}
	else if (a->get_priority() == b->get_priority())
	{
		return false;
	}
	else
	{
		return false;
	}
}

void AkEnvironmentData::add_highest_priority_environments()
{
	if (aux_array_data.data.size() < active_environments.size())
	{
		for (int i = 0; i < active_environments.size(); i++)
		{
			const AkEnvironment* env = Object::cast_to<AkEnvironment>(active_environments[i].operator godot::Object*());

			if (env)
			{
				if (i == 0)
				{
					Ref<WwiseAuxBus> aux_bus = env->get_aux_bus();
					if (aux_bus.is_null())
					{
						continue;
					}

					for (int i = 0; i < aux_array_data.data.size(); i++)
					{
						Dictionary current_aux_data = aux_array_data.data[i];
						if (current_aux_data.has("aux_bus_id"))
						{
							if ((uint32_t)current_aux_data["aux_bus_id"] == aux_bus->get_id())
							{
								continue;
							}
						}
					}

					Dictionary aux_data;
					uint32_t id = aux_bus->get_id();
					aux_data["control_value"] = 1.0f;
					aux_data["aux_bus_id"] = id;
					aux_array_data.data.append(aux_data);
				}
			}
		}
	}
}

void AkEnvironmentData::update_aux_send(Node* event, const Vector3& position)
{
	if (!have_environments_changed && position == last_position)
	{
		return;
	}

	aux_array_data.data.clear();
	add_highest_priority_environments();
	aux_array_data.set_values(event);
	last_position = position;
	have_environments_changed = false;
}

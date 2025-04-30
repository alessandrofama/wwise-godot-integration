#include "wwise_project_data.h"

void WwiseProjectData::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_acoustic_texture_root", "acoustic_texture_root"),
			&WwiseProjectData::set_acoustic_texture_root);
	ClassDB::bind_method(D_METHOD("get_acoustic_texture_root"), &WwiseProjectData::get_acoustic_texture_root);

	ClassDB::bind_method(D_METHOD("set_bus_root", "bus_root"), &WwiseProjectData::set_bus_root);
	ClassDB::bind_method(D_METHOD("get_bus_root"), &WwiseProjectData::get_bus_root);

	ClassDB::bind_method(D_METHOD("set_bank_root", "bank_root"), &WwiseProjectData::set_bank_root);
	ClassDB::bind_method(D_METHOD("get_bank_root"), &WwiseProjectData::get_bank_root);

	ClassDB::bind_method(D_METHOD("set_event_root", "event_root"), &WwiseProjectData::set_event_root);
	ClassDB::bind_method(D_METHOD("get_event_root"), &WwiseProjectData::get_event_root);

	ClassDB::bind_method(
			D_METHOD("set_game_parameter_root", "game_parameter_root"), &WwiseProjectData::set_game_parameter_root);
	ClassDB::bind_method(D_METHOD("get_game_parameter_root"), &WwiseProjectData::get_game_parameter_root);

	ClassDB::bind_method(D_METHOD("set_state_root", "state_root"), &WwiseProjectData::set_state_root);
	ClassDB::bind_method(D_METHOD("get_state_root"), &WwiseProjectData::get_state_root);

	ClassDB::bind_method(D_METHOD("set_switch_root", "switch_root"), &WwiseProjectData::set_switch_root);
	ClassDB::bind_method(D_METHOD("get_switch_root"), &WwiseProjectData::get_switch_root);

	ClassDB::bind_method(D_METHOD("set_trigger_root", "trigger_root"), &WwiseProjectData::set_trigger_root);
	ClassDB::bind_method(D_METHOD("get_trigger_root"), &WwiseProjectData::get_trigger_root);

	ClassDB::bind_method(D_METHOD("sort_custom", "a", "b"), &WwiseProjectData::sort_custom);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "acoustic_texture_root"), "set_acoustic_texture_root",
			"get_acoustic_texture_root");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "bus_root"), "set_bus_root", "get_bus_root");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "bank_root"), "set_bank_root", "get_bank_root");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "event_root"), "set_event_root", "get_event_root");
	ADD_PROPERTY(
			PropertyInfo(Variant::ARRAY, "game_parameter_root"), "set_game_parameter_root", "get_game_parameter_root");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "state_root"), "set_state_root", "get_state_root");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "switch_root"), "set_switch_root", "get_switch_root");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "trigger_root"), "set_trigger_root", "get_trigger_root");
}

float WwiseProjectData::get_event_max_attenuation(AkUniqueID p_event_id)
{
	auto event = get_event_info(p_event_id);
	return event.is_valid() ? event->get_max_attenuation() : 0.0f;
}

Ref<WwiseTreeObjectEvent> WwiseProjectData::get_event_info(AkUniqueID p_event_id)
{
	Array root_array = get_event_root();
	if (root_array.is_empty())
	{
		return Ref<WwiseTreeObjectEvent>();
	}

	std::function<Ref<WwiseTreeObjectEvent>(const Ref<WwiseTreeObject>&)> find_event =
			[&](const Ref<WwiseTreeObject>& node) -> Ref<WwiseTreeObjectEvent>
	{
		if (node.is_null())
		{
			return Ref<WwiseTreeObjectEvent>();
		}

		if (node->get_id() == p_event_id)
		{
			Ref<WwiseTreeObjectEvent> event = node;
			if (event.is_valid())
			{
				return event;
			}
		}

		Array children = node->get_children();
		for (int i = 0; i < children.size(); ++i)
		{
			Ref<WwiseTreeObject> child = children[i];
			Ref<WwiseTreeObjectEvent> found = find_event(child);
			if (found.is_valid())
			{
				return found;
			}
		}

		return Ref<WwiseTreeObjectEvent>();
	};

	for (int i = 0; i < root_array.size(); ++i)
	{
		Ref<WwiseTreeObject> root = root_array[i];
		if (root.is_valid())
		{
			Ref<WwiseTreeObjectEvent> result = find_event(root);
			if (result.is_valid())
			{
				return result;
			}
		}
	}

	return Ref<WwiseTreeObjectEvent>();
}

void WwiseProjectData::set_acoustic_texture_root(const Array& p_acoustic_texture)
{
	acoustic_texture_root = p_acoustic_texture;
}

Array WwiseProjectData::get_acoustic_texture_root() const { return acoustic_texture_root; }

void WwiseProjectData::set_bus_root(const Array& p_bus) { bus_root = p_bus; }

Array WwiseProjectData::get_bus_root() const { return bus_root; }

void WwiseProjectData::set_bank_root(const Array& p_bank) { bank_root = p_bank; }

Array WwiseProjectData::get_bank_root() const { return bank_root; }

void WwiseProjectData::set_event_root(const Array& p_event) { event_root = p_event; }

Array WwiseProjectData::get_event_root() const { return event_root; }

void WwiseProjectData::set_game_parameter_root(const Array& p_game_parameter)
{
	game_parameter_root = p_game_parameter;
}

Array WwiseProjectData::get_game_parameter_root() const { return game_parameter_root; }

void WwiseProjectData::set_state_root(const Array& p_state) { state_root = p_state; }

Array WwiseProjectData::get_state_root() const { return state_root; }

void WwiseProjectData::set_switch_root(const Array& p_switch) { switch_root = p_switch; }

Array WwiseProjectData::get_switch_root() const { return switch_root; }

void WwiseProjectData::set_trigger_root(const Array& p_trigger) { trigger_root = p_trigger; }

Array WwiseProjectData::get_trigger_root() const { return trigger_root; }

void WwiseProjectData::reset()
{
	acoustic_texture_root.clear();
	bus_root.clear();
	bank_root.clear();
	event_root.clear();
	game_parameter_root.clear();
	state_root.clear();
	switch_root.clear();
	trigger_root.clear();
}

void WwiseProjectData::sort()
{
	std::function<void(Array)> sort_and_recurse = [this, &sort_and_recurse](Array root_array)
	{
		root_array.sort_custom(callable_mp(this, &WwiseProjectData::sort_custom));

		for (int i = 0; i < root_array.size(); ++i)
		{
			Ref<WwiseTreeObject> item = root_array[i];
			if (item.is_valid())
			{
				Array children = item->get_children();
				if (children.size() > 0)
				{
					sort_and_recurse(children);
				}
			}
		}
	};

	sort_and_recurse(acoustic_texture_root);
	sort_and_recurse(bus_root);
	sort_and_recurse(bank_root);
	sort_and_recurse(event_root);
	sort_and_recurse(game_parameter_root);
	sort_and_recurse(state_root);
	sort_and_recurse(switch_root);
	sort_and_recurse(trigger_root);
}

bool WwiseProjectData::sort_custom(const Ref<WwiseTreeObject>& a, const Ref<WwiseTreeObject>& b)
{
	bool result{};

	if (a->get_type() == b->get_type())
	{
		if (a->get_type() == WwiseObjectType::Workunit)
		{
			if (a->get_name() == "Default Work Unit")
			{
				result = true;
			}
			else if (b->get_name() == "Default Work Unit")
			{
				result = false;
			}
			else
			{
				result = a->get_name() < b->get_name();
			}
		}
		else
		{
			result = a->get_name() < b->get_name();
		}
	}
	else
	{
		if (a->get_type() == WwiseObjectType::PhysicalFolder)
		{
			result = true;
		}
		else if (b->get_type() == WwiseObjectType::PhysicalFolder)
		{
			result = false;
		}
		else if (a->get_type() == WwiseObjectType::Workunit)
		{
			result = true;
		}
		else if (b->get_type() == WwiseObjectType::Workunit)
		{
			result = false;
		}
		else if (a->get_type() == WwiseObjectType::Folder)
		{
			result = true;
		}
		else if (b->get_type() == WwiseObjectType::Folder)
		{
			result = false;
		}
		else
		{
			result = false;
		}
	}

	return result;
}
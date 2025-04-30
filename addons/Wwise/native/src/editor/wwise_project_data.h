#pragma once

#include "editor/wwise_tree_object.h"
#include "editor/wwise_tree_object_event.h"
#include <functional>
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class WwiseProjectData : public Resource
{
	GDCLASS(WwiseProjectData, Resource);

protected:
	static void _bind_methods();

private:
	Array acoustic_texture_root;
	Array bus_root;
	Array bank_root;
	Array event_root;
	Array game_parameter_root;
	Array state_root;
	Array switch_root;
	Array trigger_root;

public:
	float get_event_max_attenuation(AkUniqueID p_event_id);
	Ref<WwiseTreeObjectEvent> get_event_info(AkUniqueID p_event_id);

	void set_acoustic_texture_root(const Array& p_acoustic_texture);
	Array get_acoustic_texture_root() const;

	void set_bus_root(const Array& p_bus);
	Array get_bus_root() const;

	void set_bank_root(const Array& p_bank);
	Array get_bank_root() const;

	void set_event_root(const Array& p_event);
	Array get_event_root() const;

	void set_game_parameter_root(const Array& p_game_parameter);
	Array get_game_parameter_root() const;

	void set_state_root(const Array& p_state);
	Array get_state_root() const;

	void set_switch_root(const Array& p_switch);
	Array get_switch_root() const;

	void set_trigger_root(const Array& p_trigger);
	Array get_trigger_root() const;

	void reset();
	void sort();
	bool sort_custom(const Ref<WwiseTreeObject>& a, const Ref<WwiseTreeObject>& b);
};

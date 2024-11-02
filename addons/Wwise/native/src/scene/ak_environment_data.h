#pragma once

#include "core/utils.h"
#include "core/wwise_gdextension.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/array.hpp>
#include "scene/ak_environment.h"

using namespace godot;

class AkEnvironment;

struct AkAuxArrayData
{
	Array data{};
	void set_values(const Node* event_node);

	AkAuxArrayData() { data.clear(); }
};

class AkEnvironmentData : public Object
{
	GDCLASS(AkEnvironmentData, Object);

protected:
	static void _bind_methods();

private:
	Array active_environments{};
	AkAuxArrayData aux_array_data{};
	Vector3 last_position{};
	bool have_environments_changed{};

public:
	void add_environment(const AkEnvironment* env);
	void remove_environment(const AkEnvironment* env);
	bool compare_by_priority(const AkEnvironment* a, const AkEnvironment* b);
	void add_highest_priority_environments();
	void update_aux_send(const Node* event, const Vector3& position);
};

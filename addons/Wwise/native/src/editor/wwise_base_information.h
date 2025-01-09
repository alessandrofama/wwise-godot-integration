#pragma once

#include "editor/short_id_generator.h"
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class WwiseBaseInformation : public Resource
{
	GDCLASS(WwiseBaseInformation, Resource);

protected:
	static void _bind_methods();

private:
	String name;
	String guid;
	uint32_t id{};

public:
	void set_name(const String& p_name);
	String get_name() const;

	void set_guid(const String& p_guid);
	String get_guid() const;

	void set_id(uint32_t p_id);
	uint32_t get_id() const;
};

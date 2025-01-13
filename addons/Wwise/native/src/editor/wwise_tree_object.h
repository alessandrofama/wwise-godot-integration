#pragma once

#include "core/wwise_object_type.h"
#include "editor/wwise_base_information.h"
#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

class WwiseTreeObject : public WwiseBaseInformation
{
	GDCLASS(WwiseTreeObject, WwiseBaseInformation);

protected:
	static void _bind_methods();

private:
	String path;
	Array children;
	int type{ (int)WwiseObjectType::Folder };

public:
	void set_path(const String& p_path);
	String get_path() const;

	void set_children(const Array& p_children);
	Array get_children() const;

	void set_type(int p_type);
	int get_type() const;
};
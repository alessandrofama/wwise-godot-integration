#pragma once

#include "core/utils.h"
#include "core/wwise_gdextension.h"
#include "scene/ak_event.h"
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

class AkEnvironment : public Area3D
{
	GDCLASS(AkEnvironment, Area3D);

protected:
	static void _bind_methods();

private:
	Dictionary aux_bus;
	int priority{};

public:
	AkEnvironment();

	virtual void _enter_tree() override;

	void _on_area_entered(const Area3D* area);
	void _on_area_exited(const Area3D* area);

	void set_aux_bus(const Dictionary& aux_bus);
	Dictionary get_aux_bus() const;

	void set_priority(int priority);
	int get_priority() const;
};
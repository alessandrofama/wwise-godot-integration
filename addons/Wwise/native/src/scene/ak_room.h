#pragma once

#include "core/types/wwise_aux_bus.h"
#include "core/utils.h"
#include "core/wwise_gdextension.h"
#include "scene/ak_event.h"
#include "scene/ak_geometry.h"
#include "scene/ak_listener.h"
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/node_path.hpp>

using namespace godot;

class AkRoom : public Area3D
{
	GDCLASS(AkRoom, Area3D);

protected:
	static void _bind_methods();

private:
	Ref<WwiseAuxBus> aux_bus;
	float reverb_level{ 1.0f };
	float transmission_loss{ 1.0f };
	NodePath associated_geometry{};
	bool keep_registered{ true };

public:
	virtual void _ready() override;
	virtual void _exit_tree() override;

	void _on_area_entered(const Area3D* area);
	void _on_area_exited(const Area3D* area);

	void set_aux_bus(const Ref<WwiseAuxBus>& aux_bus);
	Ref<WwiseAuxBus> get_aux_bus() const;

	void set_reverb_level(float reverb_level);
	float get_reverb_level() const;

	void set_transmission_loss(float transmission_loss);
	float get_transmission_loss() const;

	void set_associated_geometry(const NodePath& associated_geometry);
	NodePath get_associated_geometry() const;

	void set_keep_registered(bool keep_registered);
	bool get_keep_registered() const;
};
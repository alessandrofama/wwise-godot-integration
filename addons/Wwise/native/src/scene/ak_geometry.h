#pragma once

#include "core/utils.h"
#include "core/wwise_gdextension.h"
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/mesh_data_tool.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/aabb.hpp>
#include <godot_cpp/variant/node_path.hpp>

using namespace godot;

class AkGeometry : public Node3D
{
	GDCLASS(AkGeometry, Node3D);

protected:
	static void _bind_methods();
	void _notification(int p_what, bool reversed = false);

private:
	bool is_static{ true };
	bool enable_diffraction{};
	bool enable_diffraction_on_boundary_edges{};
	Dictionary acoustic_texture{};
	float transmission_loss_value{ 1.0f };

	AABB aabb{};
	PackedInt32Array indices{};

	Array vertices{};
	Array triangles{};
	MeshInstance3D* mesh_instance{};
	Object* geometry_instance{};

public:
	AkGeometry();
	virtual void _enter_tree() override;
	virtual void _exit_tree() override;
	bool set_geometry(const MeshInstance3D* mesh_instance);
	Ref<MeshDataTool> create_mesh_data_tool(const Ref<Mesh>& mesh);

	void set_is_static(bool is_static);
	bool get_is_static() const;

	void set_enable_diffraction(bool enable_diffraction);
	bool get_enable_diffraction() const;

	void set_enable_diffraction_on_boundary_edges(bool enable_diffraction_on_boundary_edges);
	bool get_enable_diffraction_on_boundary_edges() const;

	void set_acoustic_texture(const Dictionary& acoustic_texture);
	Dictionary get_acoustic_texture() const;

	void set_transmission_loss_value(float transmission_loss_value);
	float get_transmission_loss_value() const;
};
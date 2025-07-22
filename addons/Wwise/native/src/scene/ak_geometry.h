#pragma once

#include "core/types/wwise_acoustic_texture.h"
#include "core/utils.h"
#include "core/wwise_gdextension.h"
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/capsule_mesh.hpp>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/concave_polygon_shape3d.hpp>
#include <godot_cpp/classes/cylinder_mesh.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>
#include <godot_cpp/classes/mesh_data_tool.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/variant/aabb.hpp>
#include <godot_cpp/variant/node_path.hpp>

using namespace godot;

class AkGeometry : public Node3D
{
	GDCLASS(AkGeometry, Node3D);

protected:
	static void _bind_methods();
	void _notification(int p_what);

private:
	bool is_static{ true };
	bool enable_diffraction{ true };
	bool enable_diffraction_on_boundary_edges{};
	bool bypass_portal_subtraction{};
	bool is_solid{};
	Ref<WwiseAcousticTexture> acoustic_texture;
	float transmission_loss_value{ 1.0f };

	Object* geometry_instance_object{ nullptr };
	bool is_wwise_geometry_set{ false };

	bool sync_wwise_geometry();
	bool prepare_geometry_data(Node* parent_node, PackedVector3Array& out_vertices, PackedInt32Array& out_triangles);
	bool prepare_geometry_from_mesh(
			MeshInstance3D* mesh_parent, PackedVector3Array& out_vertices, PackedInt32Array& out_triangles);
	bool prepare_geometry_from_collision_object(CollisionObject3D* collision_object_parent,
			PackedVector3Array& out_vertices, PackedInt32Array& out_triangles);
	bool call_wwise_set_geometry(const PackedVector3Array& p_vertices, const PackedInt32Array& p_triangles);
	bool call_wwise_set_geometry_instance(const Transform3D& p_transform);
	void call_wwise_remove_geometry_instance();
	void call_wwise_remove_geometry_set();
	void remove_wwise_geometry();
	Ref<MeshDataTool> create_mesh_data_tool(const Ref<Mesh>& mesh);

public:
	virtual PackedStringArray _get_configuration_warnings() const override;

	AkGeometry();
	~AkGeometry();

	void set_is_static(bool p_is_static);
	bool get_is_static() const;

	void set_enable_diffraction(bool p_enable_diffraction);
	bool get_enable_diffraction() const;

	void set_enable_diffraction_on_boundary_edges(bool p_enable_diffraction_on_boundary_edges);
	bool get_enable_diffraction_on_boundary_edges() const;

	void set_bypass_portal_subtraction(bool p_bypass);
	bool get_bypass_portal_subtraction() const;

	void set_is_solid(bool p_is_solid);
	bool get_is_solid() const;

	void set_acoustic_texture(const Ref<WwiseAcousticTexture>& p_acoustic_texture);
	Ref<WwiseAcousticTexture> get_acoustic_texture() const;

	void set_transmission_loss_value(float p_transmission_loss_value);
	float get_transmission_loss_value() const;
};
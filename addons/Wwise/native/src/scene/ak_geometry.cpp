#include "ak_geometry.h"

void AkGeometry::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_is_static", "is_static"), &AkGeometry::set_is_static);
	ClassDB::bind_method(D_METHOD("get_is_static"), &AkGeometry::get_is_static);
	ClassDB::bind_method(D_METHOD("set_enable_diffraction", "enable_diffraction"), &AkGeometry::set_enable_diffraction);
	ClassDB::bind_method(D_METHOD("get_enable_diffraction"), &AkGeometry::get_enable_diffraction);
	ClassDB::bind_method(D_METHOD("set_enable_diffraction_on_boundary_edges", "enable_diffraction_on_boundary_edges"),
			&AkGeometry::set_enable_diffraction_on_boundary_edges);
	ClassDB::bind_method(D_METHOD("get_enable_diffraction_on_boundary_edges"),
			&AkGeometry::get_enable_diffraction_on_boundary_edges);
	ClassDB::bind_method(D_METHOD("set_is_solid", "is_solid"), &AkGeometry::set_is_solid);
	ClassDB::bind_method(D_METHOD("get_is_solid"), &AkGeometry::get_is_solid);
	ClassDB::bind_method(
			D_METHOD("set_bypass_portal_subtraction", "bypass"), &AkGeometry::set_bypass_portal_subtraction);
	ClassDB::bind_method(D_METHOD("get_bypass_portal_subtraction"), &AkGeometry::get_bypass_portal_subtraction);
	ClassDB::bind_method(D_METHOD("set_acoustic_texture", "acoustic_texture"), &AkGeometry::set_acoustic_texture);
	ClassDB::bind_method(D_METHOD("get_acoustic_texture"), &AkGeometry::get_acoustic_texture);
	ClassDB::bind_method(D_METHOD("set_transmission_loss_value", "transmission_loss_value"),
			&AkGeometry::set_transmission_loss_value);
	ClassDB::bind_method(D_METHOD("get_transmission_loss_value"), &AkGeometry::get_transmission_loss_value);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_static", PROPERTY_HINT_NONE), "set_is_static", "get_is_static");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_diffraction", PROPERTY_HINT_NONE), "set_enable_diffraction",
			"get_enable_diffraction");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_diffraction_on_boundary_edges", PROPERTY_HINT_NONE),
			"set_enable_diffraction_on_boundary_edges", "get_enable_diffraction_on_boundary_edges");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_solid"), "set_is_solid", "get_is_solid");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "bypass_portal_subtraction"), "set_bypass_portal_subtraction",
			"get_bypass_portal_subtraction");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "acoustic_texture", PROPERTY_HINT_RESOURCE_TYPE, "WwiseAcousticTexture"),
			"set_acoustic_texture", "get_acoustic_texture");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "transmission_loss_value", PROPERTY_HINT_NONE),
			"set_transmission_loss_value", "get_transmission_loss_value");
}

void AkGeometry::_notification(int p_what)
{
	RETURN_IF_EDITOR;

	switch (p_what)
	{
		case NOTIFICATION_READY:
		{
			set_notify_transform(!is_static);
			sync_wwise_geometry();
			break;
		}
		case NOTIFICATION_EXIT_TREE:
		{
			remove_wwise_geometry();
			break;
		}
		case NOTIFICATION_TRANSFORM_CHANGED:
		{
			remove_wwise_geometry();
			sync_wwise_geometry();
			break;
		}
	}
}

bool AkGeometry::sync_wwise_geometry()
{
	Node* parent_node = get_parent();
	if (!parent_node)
	{
		UtilityFunctions::push_error(vformat("WwiseGodot: AkGeometry '%s' has no parent.", get_path()));
		return false;
	}

	Node3D* parent_3d = Object::cast_to<Node3D>(parent_node);
	if (!parent_3d)
	{
		UtilityFunctions::push_error(vformat(
				"WwiseGodot: AkGeometry '%s' parent must be a Node3D.", get_path()));
		return false;
	}

	PackedVector3Array vertices{};
	PackedInt32Array triangles{};
	bool geometry_prepared = prepare_geometry_data(parent_node, vertices, triangles);

	if (!geometry_prepared)
	{
		remove_wwise_geometry();
		return false;
	}

	bool set_ok = call_wwise_set_geometry(vertices, triangles);
	if (!set_ok)
	{
		call_wwise_remove_geometry_instance();
		return false;
	}

	bool instance_ok = call_wwise_set_geometry_instance(parent_3d->get_global_transform());

	return instance_ok;
}

bool AkGeometry::prepare_geometry_data(
		Node* parent_node, PackedVector3Array& out_vertices, PackedInt32Array& out_triangles)
{
	MeshInstance3D* mesh_parent = Object::cast_to<MeshInstance3D>(parent_node);
	CollisionObject3D* collision_object_parent = Object::cast_to<CollisionObject3D>(parent_node);

	if (mesh_parent)
	{
		return prepare_geometry_from_mesh(mesh_parent, out_vertices, out_triangles);
	}
	else if (collision_object_parent)
	{
		return prepare_geometry_from_collision_object(collision_object_parent, out_vertices, out_triangles);
	}
	else
	{
		UtilityFunctions::push_error(vformat(
				"WwiseGodot: AkGeometry '%s' parent is neither MeshInstance3D nor CollisionObject3D.", get_path()));
		return false;
	}
}

bool AkGeometry::prepare_geometry_from_mesh(
		MeshInstance3D* mesh_parent, PackedVector3Array& out_vertices, PackedInt32Array& out_triangles)
{
	Ref<Mesh> mesh = mesh_parent->get_mesh();
	if (mesh.is_null())
	{
		UtilityFunctions::push_warning(
				vformat("WwiseGodot: AkGeometry '%s' parent MeshInstance3D has no mesh assigned.", get_path()));
		return false;
	}

	Ref<MeshDataTool> mdt = create_mesh_data_tool(mesh);
	if (mdt.is_null())
	{
		UtilityFunctions::push_warning(
				vformat("WwiseGodot: AkGeometry '%s' could not create MeshDataTool.", get_path()));
		return false;
	}

	int vertex_count = mdt->get_vertex_count();
	int face_count = mdt->get_face_count();

	if (vertex_count == 0 || face_count == 0)
	{
		UtilityFunctions::push_warning(
				vformat("WwiseGodot: AkGeometry '%s' mesh has no vertices or faces.", get_path()));
		return false;
	}

	out_vertices.clear();
	out_vertices.resize(vertex_count);
	out_triangles.clear();
	out_triangles.resize(face_count * 3);

	for (int i = 0; i < vertex_count; ++i)
	{
		Vector3 godot_vertex = mdt->get_vertex(i);
		out_vertices[i] = Vector3(godot_vertex.x, godot_vertex.y, -godot_vertex.z);
	}

	for (int i = 0; i < face_count; ++i)
	{
		int p0 = mdt->get_face_vertex(i, 0);
		int p1 = mdt->get_face_vertex(i, 1);
		int p2 = mdt->get_face_vertex(i, 2);

		out_triangles[i * 3 + 0] = p0;
		out_triangles[i * 3 + 1] = p2;
		out_triangles[i * 3 + 2] = p1;
	}

	return true;
}

bool AkGeometry::prepare_geometry_from_collision_object(
		CollisionObject3D* collision_object_parent, PackedVector3Array& out_vertices, PackedInt32Array& out_triangles)
{
	out_vertices.clear();
	out_triangles.clear();
	int current_vertex_offset = 0;
	bool has_valid_geometry = false;

	for (int i = 0; i < collision_object_parent->get_child_count(); ++i)
	{
		CollisionShape3D* cs = Object::cast_to<CollisionShape3D>(collision_object_parent->get_child(i));

		if (!cs || cs->is_disabled())
		{
			continue;
		}

		Ref<Shape3D> shape = cs->get_shape();
		if (shape.is_null())
		{
			continue;
		}

		Ref<Mesh> temp_mesh;

		if (Object::cast_to<BoxShape3D>(shape.ptr()))
		{
			Ref<BoxMesh> bm;
			bm.instantiate();
			bm->set_size(Object::cast_to<BoxShape3D>(shape.ptr())->get_size());
			temp_mesh = bm;
		}
		else if (Object::cast_to<CylinderShape3D>(shape.ptr()))
		{
			Ref<CylinderShape3D> cyl_shape = shape;
			Ref<CylinderMesh> cm;
			cm.instantiate();
			cm->set_radial_segments(16);
			cm->set_rings(1);
			cm->set_bottom_radius(cyl_shape->get_radius());
			cm->set_top_radius(cyl_shape->get_radius());
			cm->set_height(cyl_shape->get_height());
			temp_mesh = cm;
		}
		else if (Object::cast_to<CapsuleShape3D>(shape.ptr()))
		{
			Ref<CapsuleShape3D> cap_shape = shape;
			Ref<CapsuleMesh> cpm;
			cpm.instantiate();
			cpm->set_radial_segments(16);
			cpm->set_rings(8);
			cpm->set_radius(cap_shape->get_radius());
			cpm->set_height(cap_shape->get_height());
			temp_mesh = cpm;
		}
		else if (Object::cast_to<SphereShape3D>(shape.ptr()))
		{
			Ref<SphereShape3D> sph_shape = shape;
			Ref<SphereMesh> sm;
			sm.instantiate();
			sm->set_radial_segments(16);
			sm->set_rings(8);
			sm->set_radius(sph_shape->get_radius());
			sm->set_height(sph_shape->get_radius() * 2.0f);
			temp_mesh = sm;
		}
		else if (Object::cast_to<ConcavePolygonShape3D>(shape.ptr()))
		{
			Ref<ConcavePolygonShape3D> concave = shape;
			PackedVector3Array faces = concave->get_faces();
			if (faces.size() == 0 || faces.size() % 3 != 0)
			{
				UtilityFunctions::push_warning(vformat(
						"WwiseGodot: AkGeometry '%s' found ConcavePolygonShape3D with invalid face data.", get_path()));
				continue;
			}

			Ref<ArrayMesh> arr_mesh;
			arr_mesh.instantiate();
			Array surface_array;
			surface_array.resize(Mesh::ArrayType::ARRAY_MAX);
			surface_array[Mesh::ArrayType::ARRAY_VERTEX] = faces;
			arr_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, surface_array);
			temp_mesh = arr_mesh;
		}
		else
		{
			UtilityFunctions::push_warning(
					vformat("WwiseGodot: AkGeometry '%s' encountered unsupported shape type '%s'.", get_path(),
							shape->get_class()));
			continue;
		}

		if (temp_mesh.is_valid())
		{
			Ref<MeshDataTool> mdt = create_mesh_data_tool(temp_mesh);
			if (mdt.is_valid())
			{
				int vertex_count = mdt->get_vertex_count();
				int face_count = mdt->get_face_count();

				if (vertex_count > 0 && face_count > 0)
				{
					Transform3D shape_local_transform = cs->get_transform();

					for (int v = 0; v < vertex_count; ++v)
					{
						Vector3 local_vertex = mdt->get_vertex(v);
						Vector3 vertex_in_area_space = shape_local_transform.xform(local_vertex);
						out_vertices.push_back(
								Vector3(vertex_in_area_space.x, vertex_in_area_space.y, -vertex_in_area_space.z));
					}

					for (int f = 0; f < face_count; ++f)
					{
						int p0 = mdt->get_face_vertex(f, 0);
						int p1 = mdt->get_face_vertex(f, 1);
						int p2 = mdt->get_face_vertex(f, 2);

						out_triangles.push_back(p0 + current_vertex_offset);
						out_triangles.push_back(p2 + current_vertex_offset);
						out_triangles.push_back(p1 + current_vertex_offset);
					}
					current_vertex_offset += vertex_count;
					has_valid_geometry = true;
				}
			}
			else
			{
				UtilityFunctions::push_warning(
						vformat("WwiseGodot: AkGeometry '%s' failed to create MeshDataTool for shape '%s'.", get_path(),
								cs->get_name()));
			}
		}
	}

	if (!has_valid_geometry)
	{
		UtilityFunctions::push_warning(
				vformat("WwiseGodot: AkGeometry '%s' found no valid CollisionShapes under CollisionObject3D parent.",
						get_path()));
		return false;
	}

	return true;
}

bool AkGeometry::call_wwise_set_geometry(const PackedVector3Array& p_vertices, const PackedInt32Array& p_triangles)
{
	Wwise* soundengine = Wwise::get_singleton();

	bool result = soundengine->set_geometry(p_vertices, p_triangles, acoustic_texture, transmission_loss_value, this,
			enable_diffraction, enable_diffraction_on_boundary_edges);

	is_wwise_geometry_set = result;

	if (!result)
	{
		UtilityFunctions::push_error(
				vformat("WwiseGodot: AkGeometry '%s' failed to set Wwise geometry set.", get_path()));
	}

	return result;
}

bool AkGeometry::call_wwise_set_geometry_instance(const Transform3D& p_transform)
{
	Wwise* soundengine = Wwise::get_singleton();

	if (!is_wwise_geometry_set || !geometry_instance_object)
	{
		return false;
	}

	bool use_for_reflect_diffract = enable_diffraction;

	bool result = soundengine->set_geometry_instance(
			this, p_transform, use_for_reflect_diffract, is_solid, bypass_portal_subtraction, geometry_instance_object);

	if (!result)
	{
		UtilityFunctions::push_error(
				vformat("WwiseGodot: AkGeometry '%s' failed to set Wwise geometry instance.", get_path()));
	}

	return result;
}

void AkGeometry::call_wwise_remove_geometry_instance()
{
	if (!geometry_instance_object)
	{
		return;
	}

	Wwise* soundengine = Wwise::get_singleton();
	if (!soundengine->remove_geometry_instance(geometry_instance_object))
	{
		UtilityFunctions::push_error(
				vformat("WwiseGodot: AkGeometry '%s' failed to remove Wwise geometry instance.", get_path()));
	}
}

void AkGeometry::call_wwise_remove_geometry_set()
{
	if (!is_wwise_geometry_set)
	{
		return;
	}

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine->remove_geometry(this))
	{
		is_wwise_geometry_set = false;
	}
	else
	{
		UtilityFunctions::push_error(
				vformat("WwiseGodot: AkGeometry '%s' failed to remove Wwise geometry set.", get_path()));
	}
}

void AkGeometry::remove_wwise_geometry()
{
	call_wwise_remove_geometry_instance();
	call_wwise_remove_geometry_set();
}

Ref<MeshDataTool> AkGeometry::create_mesh_data_tool(const Ref<Mesh>& mesh)
{
	if (mesh.is_null())
	{
		UtilityFunctions::push_error(
				"WwiseGodot: AkGeometry '%s' create_mesh_data_tool called with null mesh.", get_path());
		return Ref<MeshDataTool>();
	}

	if (mesh->get_surface_count() == 0)
	{
		return Ref<MeshDataTool>();
	}

	Array surface_arrays = mesh->surface_get_arrays(0);
	if (surface_arrays.is_empty())
	{
		UtilityFunctions::push_warning("WwiseGodot: AkGeometry '%s' Mesh surface 0 has no array data.", get_path());
		return Ref<MeshDataTool>();
	}

	Ref<ArrayMesh> array_mesh;
	array_mesh.instantiate();
	array_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, surface_arrays);

	Ref<MeshDataTool> mesh_data_tool;
	mesh_data_tool.instantiate();
	godot::Error err = mesh_data_tool->create_from_surface(array_mesh, 0);

	if (err != OK)
	{
		UtilityFunctions::push_error(
				vformat("WwiseGodot: AkGeometry '%s' MeshDataTool::create_from_surface failed with error code: %d",
						get_path(), err));
		return Ref<MeshDataTool>();
	}

	return mesh_data_tool;
}

PackedStringArray AkGeometry::_get_configuration_warnings() const
{
	PackedStringArray result;

	Node* parent_node = get_parent();
	MeshInstance3D* mesh_parent = Object::cast_to<MeshInstance3D>(parent_node);
	CollisionObject3D* collision_object_parent = Object::cast_to<CollisionObject3D>(parent_node);

	if (!mesh_parent && !collision_object_parent)
	{
		result.push_back("AkGeometry must be a child of either a MeshInstance3D or an CollisionObject3D node (Area3D, "
						 "PhysicsBody3D).");
	}

	return result;
}

AkGeometry::AkGeometry()
{
	RETURN_IF_EDITOR;
	geometry_instance_object = memnew(Object);
}

AkGeometry::~AkGeometry()
{
	RETURN_IF_EDITOR;

	if (geometry_instance_object)
	{
		memdelete(geometry_instance_object);
		geometry_instance_object = nullptr;
	}
}

void AkGeometry::set_is_static(bool p_is_static)
{
	is_static = p_is_static;
	set_notify_transform(!is_static);
}

bool AkGeometry::get_is_static() const { return is_static; }

void AkGeometry::set_enable_diffraction(bool p_enable_diffraction)
{
	if (enable_diffraction != p_enable_diffraction)
	{
		enable_diffraction = p_enable_diffraction;

		RETURN_IF_EDITOR;

		if (is_inside_tree())
		{
			remove_wwise_geometry();
			sync_wwise_geometry();
		}
	}
}

bool AkGeometry::get_enable_diffraction() const { return enable_diffraction; }

void AkGeometry::set_enable_diffraction_on_boundary_edges(bool p_enable_diffraction_on_boundary_edges)
{
	if (enable_diffraction_on_boundary_edges != p_enable_diffraction_on_boundary_edges)
	{
		enable_diffraction_on_boundary_edges = p_enable_diffraction_on_boundary_edges;

		RETURN_IF_EDITOR;

		if (is_inside_tree())
		{
			remove_wwise_geometry();
			sync_wwise_geometry();
		}
	}
}

bool AkGeometry::get_enable_diffraction_on_boundary_edges() const { return enable_diffraction_on_boundary_edges; }

void AkGeometry::set_bypass_portal_subtraction(bool p_bypass)
{
	if (bypass_portal_subtraction != p_bypass)
	{
		bypass_portal_subtraction = p_bypass;

		RETURN_IF_EDITOR;

		if (is_inside_tree())
		{
			remove_wwise_geometry();
			sync_wwise_geometry();
		}
	}
}

bool AkGeometry::get_bypass_portal_subtraction() const { return bypass_portal_subtraction; }

void AkGeometry::set_is_solid(bool p_is_solid)
{
	if (is_solid != p_is_solid)
	{
		is_solid = p_is_solid;

		RETURN_IF_EDITOR;

		if (is_inside_tree())
		{
			remove_wwise_geometry();
			sync_wwise_geometry();
		}
	}
}

bool AkGeometry::get_is_solid() const { return is_solid; }

void AkGeometry::set_acoustic_texture(const Ref<WwiseAcousticTexture>& p_acoustic_texture)
{
	if (acoustic_texture != p_acoustic_texture)
	{
		acoustic_texture = p_acoustic_texture;
		notify_property_list_changed();

		RETURN_IF_EDITOR;

		if (is_inside_tree())
		{
			remove_wwise_geometry();
			sync_wwise_geometry();
		}
	}
}

Ref<WwiseAcousticTexture> AkGeometry::get_acoustic_texture() const { return acoustic_texture; }

void AkGeometry::set_transmission_loss_value(float p_transmission_loss_value)
{
	if (transmission_loss_value != p_transmission_loss_value)
	{
		transmission_loss_value = p_transmission_loss_value;

		RETURN_IF_EDITOR;

		if (is_inside_tree())
		{
			remove_wwise_geometry();
			sync_wwise_geometry();
		}
	}
}

float AkGeometry::get_transmission_loss_value() const { return transmission_loss_value; }
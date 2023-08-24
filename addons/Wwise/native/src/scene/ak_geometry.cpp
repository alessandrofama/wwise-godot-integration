#include "ak_geometry.h"

using namespace godot;

void AkGeometry::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_geometry", "mesh_instance"), &AkGeometry::set_geometry);
	ClassDB::bind_method(D_METHOD("create_mesh_data_tool", "mesh"), &AkGeometry::create_mesh_data_tool);

	ClassDB::bind_method(D_METHOD("set_is_static", "is_static"), &AkGeometry::set_is_static);
	ClassDB::bind_method(D_METHOD("get_is_static"), &AkGeometry::get_is_static);
	ClassDB::bind_method(D_METHOD("set_enable_diffraction", "enable_diffraction"), &AkGeometry::set_enable_diffraction);
	ClassDB::bind_method(D_METHOD("get_enable_diffraction"), &AkGeometry::get_enable_diffraction);
	ClassDB::bind_method(D_METHOD("set_enable_diffraction_on_boundary_edges", "enable_diffraction_on_boundary_edges"),
			&AkGeometry::set_enable_diffraction_on_boundary_edges);
	ClassDB::bind_method(D_METHOD("get_enable_diffraction_on_boundary_edges"),
			&AkGeometry::get_enable_diffraction_on_boundary_edges);
	ClassDB::bind_method(D_METHOD("set_acoustic_texture", "acoustic_texture"), &AkGeometry::set_acoustic_texture);
	ClassDB::bind_method(D_METHOD("get_acoustic_texture"), &AkGeometry::get_acoustic_texture);
	ClassDB::bind_method(D_METHOD("set_transmission_loss_value", "transmission_loss_value"),
			&AkGeometry::set_transmission_loss_value);
	ClassDB::bind_method(D_METHOD("get_transmission_loss_value"), &AkGeometry::get_transmission_loss_value);
	ClassDB::bind_method(D_METHOD("set_associated_room", "associated_room"), &AkGeometry::set_associated_room);
	ClassDB::bind_method(D_METHOD("get_associated_room"), &AkGeometry::get_associated_room);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_static", PROPERTY_HINT_NONE), "set_is_static", "get_is_static");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_diffraction", PROPERTY_HINT_NONE), "set_enable_diffraction",
			"get_enable_diffraction");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_diffraction_on_boundary_edges", PROPERTY_HINT_NONE),
			"set_enable_diffraction_on_boundary_edges", "get_enable_diffraction_on_boundary_edges");
	// todo(alex): Add proper hint for acoustic texture
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "acoustic_texture", PROPERTY_HINT_NONE), "set_acoustic_texture",
			"get_acoustic_texture");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "associated_room", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "AkRoom"),
			"set_associated_room", "get_associated_room");
}

void AkGeometry::_notification(int p_what, bool reversed)
{
	RETURN_IF_EDITOR;

	if (p_what == NOTIFICATION_TRANSFORM_CHANGED)
	{
		Wwise* soundengine = Wwise::get_singleton();

		if (!soundengine)
			return;

		if (soundengine->remove_geometry_instance(geometry_instance) && soundengine->remove_geometry(this))
		{
			MeshInstance3D* mesh_instance = Object::cast_to<MeshInstance3D>(get_parent());
			set_geometry(mesh_instance);
		}
	}
}

void AkGeometry::add_indices(int a, int b, int c, int d)
{
	indices.append(a);
	indices.append(c);
	indices.append(d);
	indices.append(a);
	indices.append(b);
	indices.append(c);
}

void AkGeometry::_enter_tree()
{
	RETURN_IF_EDITOR;

	geometry_instance = memnew(Object);

	mesh_instance = Object::cast_to<MeshInstance3D>(get_parent());

	if (!mesh_instance)
	{
		return;
	}

	set_geometry(mesh_instance);

	if (!is_static)
	{
		set_notify_transform(true);
	}
}

void AkGeometry::_exit_tree()
{
	RETURN_IF_EDITOR;

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		soundengine->remove_geometry_instance(geometry_instance);
		soundengine->remove_geometry(this);
	}

	memdelete(geometry_instance);
}

bool AkGeometry::set_geometry(const MeshInstance3D* mesh_instance)
{
	Ref<MeshDataTool> mdt = create_mesh_data_tool(mesh_instance->get_mesh());

	for (int i = 0; i < mdt->get_vertex_count(); ++i)
	{
		Vector3 vertex = to_global(mdt->get_vertex(i));
		vertices.append(vertex);
	}

	for (int i = 0; i < mdt->get_face_count(); ++i)
	{
		int point_0 = mdt->get_face_vertex(i, 0);
		int point_1 = mdt->get_face_vertex(i, 1);
		int point_2 = mdt->get_face_vertex(i, 2);
		triangles.append(point_0);
		triangles.append(point_1);
		triangles.append(point_2);
	}

	if (!associated_room.is_empty())
	{
		room_node = get_node<Area3D>(associated_room);
	}

	Wwise* soundengine = Wwise::get_singleton();

	bool geometry_result{};
	bool instance_result{};
	if (soundengine)
	{
		geometry_result = soundengine->set_geometry(vertices, triangles, acoustic_texture, transmission_loss_value,
				this, enable_diffraction, enable_diffraction_on_boundary_edges);
		instance_result = soundengine->set_geometry_instance(this, get_transform(), geometry_instance, room_node);
	}

	vertices.clear();
	triangles.clear();

	return geometry_result && instance_result;
}

Ref<MeshDataTool> AkGeometry::create_mesh_data_tool(const Ref<Mesh>& mesh)
{
	Ref<ArrayMesh> array_mesh;

	if (mesh->get_class() != "ArrayMesh")
	{
		Array surface_arrays = mesh->surface_get_arrays(0);
		array_mesh.instantiate();
		array_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, surface_arrays);
	}

	Ref<MeshDataTool> mesh_data_tool;
	mesh_data_tool.instantiate();
	mesh_data_tool->create_from_surface(array_mesh, 0);
	return mesh_data_tool;
}

void AkGeometry::set_is_static(bool is_static) { this->is_static = is_static; }

bool AkGeometry::get_is_static() const { return is_static; }

void AkGeometry::set_enable_diffraction(bool enable_diffraction) { this->enable_diffraction = enable_diffraction; }

bool AkGeometry::get_enable_diffraction() const { return enable_diffraction; }

void AkGeometry::set_enable_diffraction_on_boundary_edges(bool enable_diffraction_on_boundary_edges)
{
	this->enable_diffraction_on_boundary_edges = enable_diffraction_on_boundary_edges;
}

bool AkGeometry::get_enable_diffraction_on_boundary_edges() const { return enable_diffraction_on_boundary_edges; }

void AkGeometry::set_acoustic_texture(const Ref<Resource>& acoustic_texture)
{
	this->acoustic_texture = acoustic_texture;
}

Ref<Resource> AkGeometry::get_acoustic_texture() const { return acoustic_texture; }

void AkGeometry::set_transmission_loss_value(float transmission_loss_value)
{
	this->transmission_loss_value = transmission_loss_value;
}

float AkGeometry::get_transmission_loss_value() const { return transmission_loss_value; }

void AkGeometry::set_associated_room(const NodePath& associated_room) { this->associated_room = associated_room; }

NodePath AkGeometry::get_associated_room() const { return associated_room; }
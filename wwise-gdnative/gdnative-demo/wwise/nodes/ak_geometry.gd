extends Spatial
class_name AkGeometry

var vertices = []
var triangles = []
var mesh_instance

export(bool) var is_static:bool = true
export(bool) var enable_diffraction:bool = false
export(bool) var enable_diffraction_on_boundary_edges:bool = false
export(Resource) var acoustic_texture:Resource
export(float) var occlusion_value:float = 1.0
export(NodePath) var room:NodePath
var room_node

func create_mesh_data_tool(mesh:Mesh) -> MeshDataTool:
	var array_mesh = mesh
	if !array_mesh is ArrayMesh:
		var surface_arrays = mesh.surface_get_arrays(0)
		array_mesh = ArrayMesh.new()
		array_mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, surface_arrays)
	var mesh_data_tool = MeshDataTool.new()
	mesh_data_tool.create_from_surface(array_mesh, 0)
	return mesh_data_tool
	
func set_geometry(mesh_inst:MeshInstance) -> bool:
	var mdt = create_mesh_data_tool(mesh_inst.mesh)
		
	for i in range(mdt.get_vertex_count()):
		var vertex = to_global(mdt.get_vertex(i))
		vertices.append(vertex)
	
	for i in range(mdt.get_face_count()):
		var point_0 = mdt.get_face_vertex(i, 0)
		var point_1 = mdt.get_face_vertex(i, 1)
		var point_2 = mdt.get_face_vertex(i, 2)
		triangles.append(point_0)
		triangles.append(point_1)
		triangles.append(point_2)
	
	if not room.is_empty():
		room_node = get_node(room)
	
	var result = Wwise.set_geometry(vertices, triangles, acoustic_texture, occlusion_value, self, enable_diffraction, enable_diffraction_on_boundary_edges, room_node)
	
	vertices.clear()
	triangles.clear()
	
	return result
	
func _enter_tree():
	mesh_instance = get_parent()
	#warning-ignore:return_value_discarded
	set_geometry(mesh_instance)
	if not is_static:
		set_notify_transform(true)

func _exit_tree():
	Wwise.remove_geometry(self)
	
func _notification(notification):
	if(notification == NOTIFICATION_TRANSFORM_CHANGED):
		if Wwise.remove_geometry(self):
			#warning-ignore:return_value_discarded
			set_geometry(mesh_instance)

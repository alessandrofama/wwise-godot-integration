extends Spatial

var vertices = []
var triangles = []

var mesh_instance

func create_mesh_data_tool(mesh:Mesh) -> MeshDataTool:
	var array_mesh = mesh
	if !array_mesh is ArrayMesh:
		var surface_arrays = mesh.surface_get_arrays(0)
		array_mesh = ArrayMesh.new()
		array_mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, surface_arrays)
	var mesh_data_tool = MeshDataTool.new()
	mesh_data_tool.create_from_surface(array_mesh, 0)
	return mesh_data_tool
	
func set_geometry(mesh_instance:MeshInstance) -> void:
	var mdt = create_mesh_data_tool(mesh_instance.mesh)
		
	for i in range(mdt.get_vertex_count()):
		var vertex = to_global(mdt.get_vertex(i))
		vertices.append(vertex)
		
	for i in range(mdt.get_face_count()):
		var a = mdt.get_face_vertex(i, 0)
		var b = mdt.get_face_vertex(i, 1)
		var c = mdt.get_face_vertex(i, 2)
		triangles.append(a)
		triangles.append(b)
		triangles.append(c)
		
	Wwise.set_geometry(vertices, triangles, self)
	vertices.clear()
	triangles.clear()

func _ready():
	mesh_instance = get_parent()
	set_geometry(mesh_instance)

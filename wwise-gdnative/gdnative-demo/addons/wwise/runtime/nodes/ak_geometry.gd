extends Spatial

var vertices:Array = []
var triangles:Array= []
var mesh_instance:MeshInstance

export(bool) var is_static:bool = true
export(bool) var enable_diffraction:bool = false
export(bool) var enable_diffraction_on_boundary_edges:bool = false
export(Resource) var acoustic_texture:Resource
export(float) var transmission_loss_value:float = 1.0
export(NodePath) var associated_room:NodePath
export(bool) var enable_triangles:bool = true
var room_node:Node
var aabb:AABB
var sizeOffset = 1.025
export(bool) var use_simplified_mesh:bool = false
var indices = PoolIntArray()

func _enter_tree() -> void:
	mesh_instance = get_parent()
	
	if not mesh_instance:
		return
	
	if use_simplified_mesh:
		aabb = mesh_instance.get_aabb()
		aabb = aabb.abs()
		var simplified_mesh = create_simplified_mesh(mesh_instance.global_transform.origin, 
				mesh_instance.rotation, 
				(aabb.size * mesh_instance.get_global_transform().basis.get_scale()))
		
		#warning-ignore:return_value_discarded
		set_geometry(simplified_mesh)
	else:
		#warning-ignore:return_value_discarded
		set_geometry(mesh_instance)
	
	if not is_static:
		set_notify_transform(true)

func _exit_tree() -> void:
		Wwise.remove_geometry(self)
	
func _notification(notification:int) -> void:
	if(notification == NOTIFICATION_TRANSFORM_CHANGED):
		if Wwise.remove_geometry(self):
			if use_simplified_mesh:
				mesh_instance = get_parent()
				aabb = mesh_instance.get_aabb()
				aabb = aabb.abs()
				var simplified_mesh = create_simplified_mesh(mesh_instance.global_transform.origin, 
					mesh_instance.rotation, 
					(aabb.size * mesh_instance.get_global_transform().basis.get_scale()))
				#warning-ignore:return_value_discarded
				set_geometry(simplified_mesh)
			else:
				#warning-ignore:return_value_discarded
				set_geometry(mesh_instance)
	
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
	
	if not associated_room.is_empty():
		room_node = get_node(associated_room)
	
	var result = Wwise.set_geometry(vertices, triangles, acoustic_texture, transmission_loss_value, self, enable_diffraction, enable_diffraction_on_boundary_edges, room_node, enable_triangles)
	
	vertices.clear()
	triangles.clear()
	
	return result
	
func create_mesh_data_tool(mesh:Mesh) -> MeshDataTool:
	var array_mesh = mesh
	if !array_mesh is ArrayMesh:
		var surface_arrays = mesh.surface_get_arrays(0)
		array_mesh = ArrayMesh.new()
		array_mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, surface_arrays)
	var mesh_data_tool = MeshDataTool.new()
	mesh_data_tool.create_from_surface(array_mesh, 0)
	return mesh_data_tool

func create_simplified_mesh(position, rot, size) -> MeshInstance:
	var length = (aabb.size.x  * 0.5) 
	var height = (aabb.size.y * 0.5) 
	var width = (aabb.size.z  * 0.5) 
	
	var a = Vector3(-length, height, -width)
	var b = Vector3(length, height, -width)
	var c = Vector3(length, -height, -width)
	var d = Vector3(-length, -height, -width)
	var e = Vector3(length, height, width)
	var f = Vector3(-length, height, width)
	var g = Vector3(-length, -height, width)
	var h = Vector3(length, -height, width)
	
	var verts = PoolVector3Array( [
	a, b, c, d,
	e, f, g, h,
	f, a, d, g,
	b, e, h, c,
	f, e, b, a,
	d, c, h, g,
] )
	
	indices = PoolIntArray()
	
	add_indices(  0,  1,  2,  3 )
	add_indices(  4,  5,  6,  7 )
	add_indices(  8,  9, 10, 11 )
	add_indices( 12, 13, 14, 15 )
	add_indices( 16, 17, 18, 19 )
	add_indices( 20, 21, 22, 23 )

	var arrays = []
	arrays.resize(Mesh.ARRAY_MAX)

	arrays[Mesh.ARRAY_VERTEX] = verts
	arrays[Mesh.ARRAY_INDEX] = indices

	var mesh = ArrayMesh.new()
	mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, arrays)

	var simplified_mesh_instance = MeshInstance.new()
	simplified_mesh_instance.translation = position
	simplified_mesh_instance.rotation = rot
	simplified_mesh_instance.scale = size
	simplified_mesh_instance.mesh = mesh
	return simplified_mesh_instance
	
func add_indices(a, b, c, d):
	indices += PoolIntArray([a,c,d,  a,b,c])

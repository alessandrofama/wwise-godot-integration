extends EditorSpatialGizmoPlugin

const ResonanceAudioRoom = preload("res://addons/resonance_audio/resonance_audio_room.gd")

var verts = PoolVector3Array()
var indices = PoolIntArray()

func _init():
	create_material("main", Color(0.7, 0.3, 0, 0.07))

func redraw(gizmo):
	gizmo.clear()
	var spatial = gizmo.get_spatial_node() as ResonanceAudioRoom
	
	if has_gizmo(spatial):
		var mesh:ArrayMesh = get_mesh(spatial)
		gizmo.add_mesh(mesh, false, null, get_material("main", gizmo))

func get_mesh(spatial:ResonanceAudioRoom):
	var length = (1 * spatial.size.x) * 0.5
	var height = (1 * spatial.size.y) * 0.5
	var width = (1 * spatial.size.y) * 0.5
	
	var a = Vector3(-length, height, -width)
	var b = Vector3(length, height, -width)
	var c = Vector3(length, -height, -width)
	var d = Vector3(-length, -height, -width)
	var e = Vector3(length, height, width)
	var f = Vector3(-length, height, width)
	var g = Vector3(-length, -height, width)
	var h = Vector3(length, -height, width)
	

	verts = PoolVector3Array( [
	a, b, c, d,
	e, f, g, h,
	f, a, d, g,
	b, e, h, c,
	f, e, b, a,
	d, c, h, g,
] )

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
	return mesh
	
func add_indices(a, b, c, d):
	indices += PoolIntArray([a,c,d,  a,b,c])
	
func has_gizmo(spatial):
	return spatial is ResonanceAudioRoom
	
func get_name():
	return "ResonanceAudioRoom"

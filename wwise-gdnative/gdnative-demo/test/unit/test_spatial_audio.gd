extends "res://addons/gut/test.gd"

class TestSpatialAudio:
	extends "res://addons/gut/test.gd"
	
	var mesh_instance: MeshInstance
	
	func before_all():
		mesh_instance = MeshInstance.new()
		mesh_instance.name = "Mesh Instance"
		get_tree().get_root().add_child(mesh_instance)
		var mesh = load("res://meshes/default_cube.obj")
		mesh_instance.set_mesh(mesh)

	func test_assert_set_geometry():
		var ak_geometry:AkGeometry
		ak_geometry = AkGeometry.new()
		ak_geometry.name = "AkGeometry"
		mesh_instance.add_child(ak_geometry)
		#warning-ignore:return_value_discarded
		ak_geometry.set_owner(mesh_instance)
		assert_true(ak_geometry.set_geometry(mesh_instance), "Set Geometry should by true")
		Wwise.remove_geometry(ak_geometry)
	
	func test_assert_remove_geometry():
		var ak_geometry:AkGeometry
		ak_geometry = AkGeometry.new()
		ak_geometry.name = "AkGeometry"
		mesh_instance.add_child(ak_geometry)
		ak_geometry.set_owner(mesh_instance)
		#warning-ignore:return_value_discarded
		ak_geometry.set_geometry(mesh_instance)
		assert_true(Wwise.remove_geometry(ak_geometry), "Remove Geometry should be true")
		
	func test_assert_set_room():
		var node: Node = Node.new()
		node.name = "Test"
		assert_true(Wwise.set_room(node, AK.AUX_BUSSES.LARGEVERB, node.get_name()), 
					"Set Room should be true")
		Wwise.remove_room(node)
	
	func test_assert_remove_room():
		var node: Node = Node.new()
		node.name = "Test"
		Wwise.set_room(node, AK.AUX_BUSSES.LARGEVERB, node.get_name())		
		assert_true(Wwise.remove_room(node), "Remove Room should be true")
		

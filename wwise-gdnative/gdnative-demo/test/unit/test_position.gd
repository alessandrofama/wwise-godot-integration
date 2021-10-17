extends "res://addons/gut/test.gd"

class TestPosition:
	extends "res://addons/gut/test.gd"
	
	var node2d: Node2D = Node2D.new()
	var spatial: Spatial = Spatial.new()
	
	func before_all():
		Wwise.load_bank_id(AK.BANKS.INIT)
		Wwise.load_bank_id(AK.BANKS.TESTBANK)
		spatial.name = "Spatial"
		node2d.name = "Node2D"
		
	func test_assert_set_3d_position():
		Wwise.register_game_obj(spatial, spatial.get_name())
		yield(yield_for(0.1), YIELD)
		spatial.transform.origin = Vector3(8, 13, 35)
		assert_true(Wwise.set_3d_position(spatial, spatial.transform), "Setting 3D Position should be true")
		Wwise.unregister_game_obj(spatial)
	
	func test_assert_set_2d_position():
		Wwise.register_game_obj(node2d, node2d.get_name())
		yield(yield_for(0.1), YIELD)
		node2d.transform.origin = Vector2(5, 17)
		node2d.z_index = -33
		assert_true(Wwise.set_2d_position(node2d, node2d.transform, node2d.z_index), "Setting 2D Position should be true")
		Wwise.unregister_game_obj(node2d)
		
	func test_set_multiple_positions_3d():
		Wwise.register_game_obj(spatial, spatial.get_name())
		yield(yield_for(0.1), YIELD)
		spatial.transform.origin = Vector3(8, 13, 35)
		var positions:Array = []
		positions.push_back(spatial.transform)
		var num_positions = 1
		assert_true(Wwise.set_multiple_positions_3d(spatial, positions, num_positions, AkUtils.MultiPositionType.MultiSources), "Set Multiple Position should be true")
		Wwise.unregister_game_obj(spatial)
		
	func test_set_multiple_positions_2d():
		Wwise.register_game_obj(node2d, node2d.get_name())
		yield(yield_for(0.1), YIELD)
		node2d.transform.origin = Vector2(4, 2)
		var positions:Array = []
		positions.push_back(node2d.transform)
		var z_depths:Array = []
		z_depths.push_back(-7)
		var num_positions = 1
		assert_true(Wwise.set_multiple_positions_2d(node2d, positions, z_depths, num_positions, AkUtils.MultiPositionType.MultiSources), "Set Multiple Position should be true")
		Wwise.unregister_game_obj(node2d)
		
	func test_set_gameobj_radius():
		Wwise.register_game_obj(spatial, spatial.get_name())
		yield(yield_for(0.1), YIELD)
		assert_true(Wwise.set_game_obj_radius(spatial, 12, 6), "Set GameObject Radius should be true")
		Wwise.unregister_game_obj(spatial)
		
	func after_all():
		Wwise.unload_bank_id(AK.BANKS.TESTBANK)	
		Wwise.unload_bank_id(AK.BANKS.INIT)

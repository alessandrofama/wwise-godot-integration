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
		
	func after_all():
		Wwise.unload_bank_id(AK.BANKS.TESTBANK)	
		Wwise.unload_bank_id(AK.BANKS.INIT)

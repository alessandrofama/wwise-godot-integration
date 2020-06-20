extends "res://addons/gut/test.gd"

class TestGameObject:
	extends "res://addons/gut/test.gd"
	

	
	func before_all():
		Wwise.load_bank_id(AK.BANKS.INIT)
		Wwise.load_bank_id(AK.BANKS.TESTBANK)
		
	func test_assert_register_game_obj():
		var node: Node = Node.new()
		node.name = "Test"
		assert_true(Wwise.register_game_obj(node, node.get_name()), "Register Game Obj should be true")
		Wwise.unregister_game_obj(node)
		
	func test_assert_unregister_game_obj():
		var node: Node = Node.new()
		node.name = "Test"
		Wwise.register_game_obj(node, node.get_name())
		assert_true(Wwise.unregister_game_obj(node), "Unregister Game Obj should be true")
		
	func test_assert_set_game_obj_output_bus_volume():
		var node: Node = Node.new()
		node.name = "Test"
		Wwise.register_game_obj(node, node.get_name())
		var listener:Node = Node.new()
		Wwise.register_listener(listener)
		assert_true(Wwise.set_game_obj_output_bus_volume(node.get_instance_id(), 
					listener.get_instance_id(), 1), "Set Game Obj Outputs Bus volume should be true")
		Wwise.unregister_game_obj(node)
		
	func test_assert_set_game_obj_aux_send_values():
		var node: Node = Node.new()
		node.name = "Test"
		Wwise.register_game_obj(node, node.get_name())
		var aux_array = []
		var aux_data = {"aux_bus_id": AK.AUX_BUSSES.LARGEVERB, "control_value":  1.0}
		aux_array.append(aux_data)
		assert_true(Wwise.set_game_obj_aux_send_values(node.get_instance_id(), aux_array, 1), 
					"Set Game Obj Aux Send Values should be true")
		Wwise.unregister_game_obj(node)
		
	func test_assert_set_obstruction_occlusion():
		var node: Node = Node.new()
		node.name = "Test"
		Wwise.register_game_obj(node, node.get_name())
		var listener:Node = Node.new()
		Wwise.register_listener(listener)
		assert_true(Wwise.set_obj_obstruction_and_occlusion(node.get_instance_id(), 
					listener.get_instance_id(), 0.5, 0.7), "Set Obstruction and Occlusion should be true")
		Wwise.unregister_game_obj(node)
	
	func after_all():
		Wwise.unload_bank_id(AK.BANKS.TESTBANK)	
		Wwise.unload_bank_id(AK.BANKS.INIT)

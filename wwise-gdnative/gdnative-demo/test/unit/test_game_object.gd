extends "res://addons/gut/test.gd"

class TestGameObject:
	extends "res://addons/gut/test.gd"
	
	var node: Node = Node.new()
	
	func before_all():
		Wwise.load_bank_id(AK.BANKS.INIT)
		Wwise.load_bank_id(AK.BANKS.TESTBANK)
		node.name = "Test"
		
	func test_assert_register_game_obj():
		assert_true(Wwise.register_game_obj(node, node.get_name()), "Register Game Obj should be true")
		Wwise.unregister_game_obj(node)
		
	func test_assert_unregister_game_obj():
		Wwise.register_game_obj(node, node.get_name())
		assert_true(Wwise.unregister_game_obj(node), "Unregister Game Obj should be true")
	
	func after_all():
		Wwise.unload_bank_id(AK.BANKS.TESTBANK)	
		Wwise.unload_bank_id(AK.BANKS.INIT)

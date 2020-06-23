extends "res://addons/gut/test.gd"

class TestListener:
	extends "res://addons/gut/test.gd"
	
	var node: Node = Node.new()
	
	func before_all():
		Wwise.load_bank_id(AK.BANKS.INIT)
		Wwise.load_bank_id(AK.BANKS.TESTBANK)
		
	func test_assert_register_listener():
		assert_true(Wwise.register_listener(node), "Register Listener should be true")
	
	func test_assert_register_spatial_listener():
		assert_true(Wwise.register_spatial_listener(node), "Register Spatial Listener should be true")
	
	func after_all():
		Wwise.unload_bank_id(AK.BANKS.TESTBANK)	
		Wwise.unload_bank_id(AK.BANKS.INIT)

extends "res://addons/gut/test.gd"

class TestBank:
	extends "res://addons/gut/test.gd"
	
	func test_assert_load_bank_string():
		# loading banks
		assert_true(Wwise.load_bank("Init"), "Loading bank should be true")
		assert_true(Wwise.load_bank("TestBank"), "Loading bank should be true")
		
		Wwise.unload_bank("TestBank")
		Wwise.unload_bank("Init")

		
	func test_assert_load_bank_id():
		assert_true(Wwise.load_bank_id(AK.BANKS.INIT), "Loading bank should be true")
		assert_true(Wwise.load_bank_id(AK.BANKS.TESTBANK), "Loading bank should be true")
		
		Wwise.unload_bank_id(AK.BANKS.TESTBANK)
		Wwise.unload_bank_id(AK.BANKS.INIT)

	
	func test_assert_unload_bank_string():
		Wwise.load_bank("Init")
		Wwise.load_bank("TestBank")
		
		assert_true(Wwise.unload_bank("TestBank"), "Unloading bank should be true")		
		assert_true(Wwise.unload_bank("Init"), "Unloading bank should be true")
	
	func test_assert_unload_bank_id():
		Wwise.load_bank_id(AK.BANKS.INIT)
		Wwise.load_bank_id(AK.BANKS.TESTBANK)
		
		assert_true(Wwise.unload_bank_id(AK.BANKS.TESTBANK), "Unloading bank should be true")
		assert_true(Wwise.unload_bank_id(AK.BANKS.INIT), "Unloading bank should be true")

		

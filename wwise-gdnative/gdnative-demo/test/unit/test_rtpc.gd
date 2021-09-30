extends "res://addons/gut/test.gd"

class TestRTPC:
	extends "res://addons/gut/test.gd"
	
	var node: Node = Node.new()
	var sprite: Sprite = Sprite.new()
	var playing_id:int
	
	func before_all():
		Wwise.load_bank_id(AK.BANKS.INIT)
		Wwise.load_bank_id(AK.BANKS.TESTBANK)
		node.name = "Test"
		Wwise.register_game_obj(node, node.get_name())
		Wwise.set_rtpc("Enemies", 3, node)
		
	func test_assert_set_rtpc_string():
		assert_true(Wwise.set_rtpc("MusicVolume", -33.0, node), "Set RTPC should be true")
		
	func test_assert_get_rtpc_string():
		yield(yield_for(1), YIELD)
		playing_id = Wwise.post_event_id(AK.EVENTS.MUSIC, node)
		var desired_value: float = 3
		assert_eq(Wwise.get_rtpc("Enemies", node), desired_value, "RTPC value should be: " + str(desired_value))
		Wwise.stop_event(playing_id, 0, AkUtils.AkCurveInterpolation.LINEAR)		
		
	func test_assert_set_rtpc_id():
		assert_true(Wwise.set_rtpc_id(AK.GAME_PARAMETERS.MUSICVOLUME, -33, node), "Set RTPC ID should be true")
		
	func test_assert_get_rtpc_id():
		yield(yield_for(1), YIELD)
		playing_id = Wwise.post_event_id(AK.EVENTS.MUSIC, node)
		var desired_value: float = 3
		assert_eq(Wwise.get_rtpc_id(AK.GAME_PARAMETERS.ENEMIES, node), desired_value, "RTPC value should be: " + str(desired_value))
		Wwise.stop_event(playing_id, 0, AkUtils.AkCurveInterpolation.LINEAR)
	
	func after_all():
		Wwise.unregister_game_obj(node)		
		Wwise.unload_bank_id(AK.BANKS.TESTBANK)	
		Wwise.unload_bank_id(AK.BANKS.INIT)

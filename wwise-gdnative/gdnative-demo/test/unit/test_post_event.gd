extends "res://addons/gut/test.gd"

class TestPostEvent:
	extends "res://addons/gut/test.gd"
	
	var node: Node = Node.new()
	
	func before_all():
		Wwise.load_bank_id(AK.BANKS.INIT)
		Wwise.load_bank_id(AK.BANKS.TESTBANK)
		node.name = "Test"
		Wwise.register_game_obj(node, node.get_name())
		
	func test_assert_post_event_string():
		yield(yield_for(0.1), YIELD)
		var desired_value = Wwise.post_event("Play_chimes_with_marker", node)
		assert_true(desired_value > 0, "Playing ID should be greater than 0")
		Wwise.stop_event(desired_value, 0, AkUtils.AkInterpolationMode.LINEAR)
		
	func test_assert_post_event_id():
		yield(yield_for(0.1), YIELD)
		var desired_value = Wwise.post_event_id(AK.EVENTS.PLAY_CHIMES_WITH_MARKER, node)
		assert_true(desired_value > 0, "Playing ID should be greater than 0")
		Wwise.stop_event(desired_value, 0, AkUtils.AkInterpolationMode.LINEAR)

	func test_asert_post_event_callback_string():
		yield(yield_for(0.1), YIELD)
		var desired_value = Wwise.post_event_callback("Play_chimes_with_marker", AkUtils.AkCallbackType.AK_Marker, node)
		assert_true(desired_value > 0, "Playing ID should be greater than 0")
		Wwise.stop_event(desired_value, 0, AkUtils.AkInterpolationMode.LINEAR)

	func test_assert_post_event_callback_id():
		yield(yield_for(0.1), YIELD)
		var desired_value = Wwise.post_event_id_callback(AK.EVENTS.PLAY_CHIMES_WITH_MARKER, AkUtils.AkCallbackType.AK_Marker, node)
		assert_true(desired_value > 0, "Playing ID should be greater than 0")
		Wwise.stop_event(desired_value, 0, AkUtils.AkInterpolationMode.LINEAR)
		
	func test_assert_stop_event():
		var playing_id = Wwise.post_event_id(AK.EVENTS.PLAY_CHIMES_WITH_MARKER, node)
		yield(yield_for(0.1), YIELD)
		assert_true(Wwise.stop_event(playing_id, 0, AkUtils.AkInterpolationMode.LINEAR))
	
	func after_all():
		Wwise.unregister_game_obj(node)		
		Wwise.unload_bank_id(AK.BANKS.TESTBANK)	
		Wwise.unload_bank_id(AK.BANKS.INIT)

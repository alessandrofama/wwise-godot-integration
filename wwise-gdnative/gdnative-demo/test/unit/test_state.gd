extends "res://addons/gut/test.gd"

class TestState:
	extends "res://addons/gut/test.gd"
	
	var node: Node = Node.new()
	
	func before_all():
		Wwise.load_bank_id(AK.BANKS.INIT)
		Wwise.load_bank_id(AK.BANKS.TESTBANK)
		node.name = "Test"
		
	func test_assert_set_state_string():
		Wwise.register_game_obj(node, node.get_name())
		yield(yield_for(0.1), YIELD)
		assert_true(Wwise.set_state("MusicState", "Intense"), "Set State should be true")
		var playing_id = Wwise.post_event_id(AK.EVENTS.MUSIC, node)
		Wwise.stop_event(playing_id, 0, AkUtils.AkCurveInterpolation.LINEAR)
		Wwise.unregister_game_obj(node)
		
	func test_assert_set_state_id():
		Wwise.register_game_obj(node, node.get_name())
		yield(yield_for(0.1), YIELD)
		assert_true(Wwise.set_state_id(AK.STATES.MUSICSTATE.GROUP, AK.STATES.MUSICSTATE.STATE.INTENSE), "Set State should be true")
		var playing_id = Wwise.post_event_id(AK.EVENTS.FOOTSTEPS, node)
		Wwise.stop_event(playing_id, 0, AkUtils.AkCurveInterpolation.LINEAR)
		Wwise.unregister_game_obj(node)
	
	func after_all():
		Wwise.unload_bank_id(AK.BANKS.TESTBANK)	
		Wwise.unload_bank_id(AK.BANKS.INIT)

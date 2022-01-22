extends "res://addons/gut/test.gd"

class TestPostEvent:
	extends "res://addons/gut/test.gd"
	
	var node: Node = Node.new()
	
	func before_all():
		Wwise.load_bank_id(AK.BANKS.INIT)
		Wwise.load_bank_id(AK.BANKS.TESTBANK)
		node.name = "Test"
		Wwise.register_game_obj(node, node.get_name())
		
#	func test_assert_post_event_string():
#		yield(yield_for(0.1), YIELD)
#		var desired_value = Wwise.post_event("Play_chimes_with_marker", node)
#		assert_true(desired_value > 0, "Playing ID should be greater than 0")
#		Wwise.stop_event(desired_value, 0, AkUtils.AkCurveInterpolation.LINEAR)
		
	func test_assert_post_event_id():
		yield(yield_for(0.1), YIELD)
		var desired_value = Wwise.post_event_id(AK.EVENTS.PLAY_CHIMES_WITH_MARKER, node)
		assert_true(desired_value > 0, "Playing ID should be greater than 0")
		Wwise.stop_event(desired_value, 0, AkUtils.AkCurveInterpolation.LINEAR)

#	func test_assert_post_event_callback_string():
#		yield(yield_for(0.1), YIELD)
#		var desired_value = Wwise.post_event_callback("Play_chimes_with_marker", AkUtils.AkCallbackType.AK_Marker, node)
#		assert_true(desired_value > 0, "Playing ID should be greater than 0")
#		Wwise.stop_event(desired_value, 0, AkUtils.AkCurveInterpolation.LINEAR)

#	func test_assert_post_event_callback_id():
#		yield(yield_for(0.1), YIELD)
#		var desired_value = Wwise.post_event_id_callback(AK.EVENTS.PLAY_CHIMES_WITH_MARKER, AkUtils.AkCallbackType.AK_Marker, self, null)
#		assert_true(desired_value > 0, "Playing ID should be greater than 0")
#		Wwise.stop_event(desired_value, 0, AkUtils.AkCurveInterpolation.LINEAR)
		
	func test_assert_stop_event():
		var playing_id = Wwise.post_event_id(AK.EVENTS.PLAY_CHIMES_WITH_MARKER, node)
		yield(yield_for(0.1), YIELD)
		assert_true(Wwise.stop_event(playing_id, 0, AkUtils.AkCurveInterpolation.LINEAR))
		
	func test_assert_post_trigger_string():
		var playing_id = Wwise.post_event_id(AK.EVENTS.MUSIC, node)
		yield(yield_for(0.1), YIELD)
		assert_true(Wwise.post_trigger("MusicTrigger", node), "Post Trigger should be true")
		Wwise.stop_event(playing_id, 0, AkUtils.AkCurveInterpolation.LINEAR)
		
	func test_assert_post_trigger_id():
		var playing_id = Wwise.post_event_id(AK.EVENTS.MUSIC, node)
		yield(yield_for(0.1), YIELD)
		assert_true(Wwise.post_trigger_id(AK.TRIGGERS.MUSICTRIGGER, node), "Post Trigger ID should be true")
		Wwise.stop_event(playing_id, 0, AkUtils.AkCurveInterpolation.LINEAR)
		
	func test_assert_external_source_string():
		var desired_value = Wwise.post_external_source("External_Source_Event", node, "External_Source", "ExternalSources/External_Source_Demo.wem", AkUtils.AkCodecID.AKCODECID_PCM)
		yield(yield_for(0.1), YIELD)
		assert_true(desired_value > 0, "External Source Playing ID should be greater than 0")
		Wwise.stop_event(desired_value, 0, AkUtils.AkCurveInterpolation.LINEAR)

	func test_assert_external_source_id():
		var desired_value = Wwise.post_external_source_id(AK.EVENTS.EXTERNAL_SOURCE_EVENT, node, AK.EXTERNAL_SOURCES.EXTERNAL_SOURCE, "ExternalSources/External_Source_Demo.wem", AkUtils.AkCodecID.AKCODECID_PCM)
		yield(yield_for(0.1), YIELD)
		assert_true(desired_value > 0, "External Source Playing ID should be greater than 0")
		Wwise.stop_event(desired_value, 0, AkUtils.AkCurveInterpolation.LINEAR)
		
#	func test_assert_get_source_play_position():
#		var playing_id = Wwise.post_event_id_callback(AK.EVENTS.MUSIC, AkUtils.AkCallbackType.AK_EnableGetSourcePlayPosition, self, null)
#		yield(yield_for(0.1), YIELD)
#		var position = Wwise.get_source_play_position(playing_id, true)
#		yield(yield_for(0.1), YIELD)
#		assert_true(position > 0, "Get Source Play Position should be greater than 0")
#		Wwise.stop_event(playing_id, 0, AkUtils.AkCurveInterpolation.LINEAR)
		
#	func test_assert_get_playing_segment_info():
#		var playing_id = Wwise.post_event_id_callback(AK.EVENTS.MUSIC, AkUtils.AkCallbackType.AK_EnableGetMusicPlayPosition, self, null)
#		yield(yield_for(0.1), YIELD)
#		var segment_info:Dictionary = Wwise.get_playing_segment_info(playing_id, true)
#		yield(yield_for(0.1), YIELD)
#		assert_true(segment_info.iCurrentPosition > 0, "Current Position of Segment Info should be greater than 0")
#		Wwise.stop_event(playing_id, 0, AkUtils.AkCurveInterpolation.LINEAR)
		
	func after_all():
		Wwise.unregister_game_obj(node)		
		Wwise.unload_bank_id(AK.BANKS.TESTBANK)	
		Wwise.unload_bank_id(AK.BANKS.INIT)

class_name TestWwise
extends GdUnitTestSuite

var bank_data

func create_node_3d() -> Node3D:
	var node = Node3D.new()
	add_child(node)
	node.owner = self
	return node

func free_node_3d(node):
	node.queue_free()

func create_node_2d() -> Node2D:
	var node = Node2D.new()
	add_child(node)
	node.owner = self
	return node

func free_node_2d(node):
	node.queue_free()

func get_sample_geometry_info():
	var node = create_node_3d()
	Wwise.register_game_obj(node, "GeometryNode")
	var mesh_instance = MeshInstance3D.new()
	mesh_instance.mesh = BoxMesh.new()
	var surface_arrays = mesh_instance.mesh.surface_get_arrays(0)
	var array_mesh:ArrayMesh = ArrayMesh.new()
	array_mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, surface_arrays)
	var mdt = MeshDataTool.new()
	mdt.create_from_surface(array_mesh, 0)
	var vertices = []
	var triangles = []

	for i in range(mdt.get_vertex_count()):
		var vertex = mdt.get_vertex(i)
		vertices.append(Vector3(vertex.x, vertex.y, -vertex.z))

	for i in range(mdt.get_face_count()):
		var point_0 = mdt.get_face_vertex(i, 0)
		var point_1 = mdt.get_face_vertex(i, 1)
		var point_2 = mdt.get_face_vertex(i, 2)
		triangles.append(point_0)
		triangles.append(point_2)
		triangles.append(point_1)

	mesh_instance.free()
	return {"vertices":vertices, "triangles":triangles}

func bank_callback(_data):
	bank_data = _data


# Tests
func test_load_bank() -> void:
	var result = Wwise.load_bank("UserDefinedBank")
	assert_bool(result).is_true()
	Wwise.unload_bank("UserDefinedBank")


func test_unload_bank() -> void:
	Wwise.load_bank("UserDefinedBank")
	var result = Wwise.unload_bank("UserDefinedBank")
	assert_bool(result).is_true()


func test_load_bank_async() -> void:
	var result = Wwise.load_bank_async("UserDefinedBank", bank_callback)
	await get_tree().create_timer(0.1).timeout
	Wwise.unload_bank("UserDefinedBank")
	assert_bool(result).is_true()
	assert_bool(bank_data.has("result")).is_true()
	assert_bool(bank_data["result"] == 1).is_true()


func test_unload_bank_async() -> void:
	Wwise.load_bank("UserDefinedBank")
	var result = Wwise.unload_bank_async("UserDefinedBank", bank_callback)
	await get_tree().create_timer(0.1).timeout
	assert_bool(result).is_true()
	assert_bool(bank_data.has("result")).is_true()
	assert_bool(bank_data["result"] == 1).is_true()


func test_register_listener() -> void:
	var listener = Node3D.new()
	var result = Wwise.register_listener(listener)
	listener.queue_free()
	assert_bool(result).is_true()


func test_register_game_obj() -> void:
	var game_obj = Node.new()
	var result = Wwise.register_game_obj(game_obj, "game_obj")
	Wwise.unregister_game_obj(game_obj)
	game_obj.queue_free()
	assert_bool(result).is_true()


func test_unregister_game_obj() -> void:
	var game_obj = Node.new()
	Wwise.register_game_obj(game_obj, "game obj")
	var result = Wwise.unregister_game_obj(game_obj)
	game_obj.queue_free()
	assert_bool(result).is_true()


func test_set_distance_probe() -> void:
	var listener = create_node_3d()
	Wwise.register_listener(listener)
	var probe = create_node_3d()
	var result = Wwise.set_distance_probe(listener, probe)
	free_node_3d(listener)
	free_node_3d(probe)
	assert_bool(result).is_true()


func test_reset_distance_probe() -> void:
	var listener = create_node_3d()
	Wwise.register_listener(listener)
	var probe = create_node_3d()
	Wwise.set_distance_probe(listener, probe)
	var result = Wwise.reset_distance_probe(listener)
	free_node_3d(listener)
	free_node_3d(probe)
	assert_bool(result).is_true()


func test_set_listeners() -> void:
	var listener = Node3D.new()
	Wwise.register_listener(listener)
	var game_obj = Node.new()
	Wwise.register_game_obj(game_obj, "emitter")
	var result = Wwise.set_listeners(game_obj, listener)
	listener.queue_free()
	game_obj.queue_free()
	assert_bool(result).is_true()


func test_set_3d_position() -> void:
	var node = create_node_3d()
	Wwise.register_game_obj(node, "game_obj")
	var result = Wwise.set_3d_position(node, node.get_global_transform())
	free_node_3d(node)
	assert_bool(result).is_true()


func test_set_2d_position() -> void:
	var node = create_node_2d()
	Wwise.register_game_obj(node, "game_obj")
	var result = Wwise.set_2d_position(node, node.get_global_transform(), 1.0)
	free_node_2d(node)
	assert_bool(result).is_true()


func test_set_multiple_positions_3d() -> void:
	var node = create_node_3d()
	var node_2 = create_node_3d()
	Wwise.register_game_obj(node, "game_obj1")
	Wwise.register_game_obj(node_2, "game_obj2")
	var positions: Array[Transform3D]
	positions.push_back(node.get_global_transform())
	positions.push_back(node_2.get_global_transform())
	var result = Wwise.set_multiple_positions_3d(node, positions, 2, AkUtils.TYPE_MULTI_SOURCES)
	free_node_3d(node)
	free_node_3d(node_2)
	assert_bool(result).is_true()


func test_set_multiple_positions_2d() -> void:
	var node = create_node_2d()
	var node_2 = create_node_2d()
	Wwise.register_game_obj(node, "game_obj1")
	Wwise.register_game_obj(node_2, "game_obj2")
	var positions: Array[Transform2D]
	positions.push_back(node.get_global_transform())
	positions.push_back(node_2.get_global_transform())
	var result = Wwise.set_multiple_positions_2d(node, positions, [2.0, 5.0], 2, AkUtils.TYPE_MULTI_SOURCES)
	free_node_2d(node)
	free_node_2d(node_2)
	assert_bool(result).is_true()


func test_set_game_object_radius() -> void:
	var node = create_node_3d()
	Wwise.register_game_obj(node, "game_obj")
	var result = Wwise.set_game_object_radius(node, 5.0, 3.0)
	free_node_3d(node)
	assert_bool(result).is_true()


func test_post_event() -> void:
	Wwise.load_bank("UserDefinedBank")
	var node = create_node_3d()
	var result = Wwise.post_event("Play_UserDefinedBankSFX", node)
	await await_millis(100)
	free_node_3d(node)
	Wwise.unload_bank("UserDefinedBank")
	assert_int(result).is_greater(0)


func test_post_event_callback() -> void:
	Wwise.load_bank("UserDefinedBank")
	var node = create_node_3d()
	var result = Wwise.post_event_callback("Play_UserDefinedBankSFX", AkUtils.AK_DURATION, node, func(_data): pass)
	await await_millis(100)
	free_node_3d(node)
	Wwise.unload_bank("UserDefinedBank")
	assert_int(result).is_greater(0)


func test_post_event_id() -> void:
	Wwise.load_bank("UserDefinedBank")
	var node = create_node_3d()
	var result = Wwise.post_event_id(AK.EVENTS.PLAY_USERDEFINEDBANKSFX, node)
	await await_millis(100)
	free_node_3d(node)
	Wwise.unload_bank("UserDefinedBank")
	assert_int(result).is_greater(0)


func test_post_event_id_stream() -> void:
	var node = create_node_3d()
	var event:WwiseEvent = WwiseEvent.new()
	event.set_name("Streaming_Event")
	event.set_id(AK.EVENTS.STREAMING_EVENT)
	event.emit_signal("ws_post_resource_init")
	var result = event.post(node)
	free_node_3d(node)
	assert_int(result).is_greater(0)


func test_post_event_id_callback() -> void:
	Wwise.load_bank("UserDefinedBank")
	var node = create_node_3d()
	var result = Wwise.post_event_id_callback(AK.EVENTS.PLAY_USERDEFINEDBANKSFX, AkUtils.AK_DURATION, node, func(_data): pass)
	await await_millis(100)
	free_node_3d(node)
	Wwise.unload_bank("UserDefinedBank")
	assert_int(result).is_greater(0)


func test_post_midi_on_event_id() -> void:
	Wwise.load_bank("UserDefinedBank")
	var node = create_node_3d()
	var array:Array[AkMidiPost]
	var mpost = AkMidiPost.new()
	mpost.by_type = AkMidiPost.MIDI_EVENT_TYPE_NOTE_ON
	mpost.by_note = 43
	array.push_back(mpost)
	var result = Wwise.post_midi_on_event_id(AK.EVENTS.PLAY_USERDEFINEDBANKSFX, node, array, false)
	await await_millis(100)
	free_node_3d(node)
	Wwise.unload_bank("UserDefinedBank")
	assert_int(result).is_greater(0)


func test_stop_midi_on_event_id() -> void:
	Wwise.load_bank("UserDefinedBank")
	var node = create_node_3d()
	var array:Array[AkMidiPost]
	var mpost = AkMidiPost.new()
	mpost.by_type = AkMidiPost.MIDI_EVENT_TYPE_NOTE_ON
	mpost.by_note = 43
	array.push_back(mpost)
	Wwise.post_midi_on_event_id(AK.EVENTS.PLAY_USERDEFINEDBANKSFX, node, array, false)
	var result = Wwise.stop_midi_on_event_id(AK.EVENTS.PLAY_USERDEFINEDBANKSFX, node)
	await await_millis(100)
	free_node_3d(node)
	Wwise.unload_bank("UserDefinedBank")
	assert_bool(result).is_true()


func test_set_switch() -> void:
	var node = create_node_3d()
	var result = Wwise.set_switch("AkSwitch", "HighSwitch", node)
	free_node_3d(node)
	assert_bool(result).is_true()


func test_set_switch_id() -> void:
	var node = create_node_3d()
	var result = Wwise.set_switch_id(AK.SWITCHES.AKSWITCH.GROUP, AK.SWITCHES.AKSWITCH.SWITCH.HIGHSWITCH, node)
	free_node_3d(node)
	assert_bool(result).is_true()


func test_set_state() -> void:
	var result = Wwise.set_state("AkState", "High")
	assert_bool(result).is_true()


func test_set_state_id() -> void:
	var result = Wwise.set_state_id(AK.STATES.AKSTATE.GROUP, AK.STATES.AKSTATE.STATE.LOW)
	assert_bool(result).is_true()


func test_get_rtpc_value() -> void:
	var node = create_node_3d()
	Wwise.set_rtpc_value("RTPC", 300, node)
	await get_tree().create_timer(0.1).timeout
	var value = Wwise.get_rtpc_value("RTPC", node)
	free_node_3d(node)
	assert_float(value).is_equal_approx(300, 10)


func test_get_rtpc_value_id() -> void:
	var node = create_node_3d()
	Wwise.set_rtpc_value_id(AK.GAME_PARAMETERS.RTPC, 200, node)
	await get_tree().create_timer(0.1).timeout
	var value = Wwise.get_rtpc_value_id(AK.GAME_PARAMETERS.RTPC, node)
	free_node_3d(node)
	assert_float(value).is_equal_approx(200, 10)


func test_set_rtpc_value() -> void:
	var node = create_node_3d()
	var result = Wwise.set_rtpc_value("RTPC", 300, node)
	free_node_3d(node)
	assert_bool(result).is_true()


func test_set_rtpc_value_id() -> void:
	var node = create_node_3d()
	var result = Wwise.set_rtpc_value_id(AK.GAME_PARAMETERS.RTPC, 200, node)
	free_node_3d(node)
	assert_bool(result).is_true()


func test_post_trigger() -> void:
	var node = create_node_3d()
	var result = Wwise.post_trigger("Trigger", node)
	free_node_3d(node)
	assert_bool(result).is_true()


func test_post_trigger_id() -> void:
	var node = create_node_3d()
	var result = Wwise.post_trigger_id(AK.TRIGGERS.TRIGGER, node)
	free_node_3d(node)
	assert_bool(result).is_true()


func test_post_external_source_id() -> void:
	var node = create_node_3d()
	var event:WwiseEvent = WwiseEvent.new()
	event.set_name("Play_One")
	event.set_id(AK.EVENTS.PLAY_ONE)
	event.emit_signal("ws_post_resource_init")
	var playing_id = Wwise.post_external_source_id(
		event.id,
		node,
		1064933119,
		"01.wem",
		AkUtils.AK_CODECID_ADPCM
	)
	Wwise.stop_event(playing_id, 0, AkUtils.AK_CURVE_LINEAR)
	free_node_3d(node)
	assert_int(playing_id).is_greater(0)
	


func test_post_external_sources() -> void:
	var node = create_node_3d()
	var event:WwiseEvent = WwiseEvent.new()
	event.set_name("Play_One")
	event.set_id(AK.EVENTS.PLAY_ONE)
	event.emit_signal("ws_post_resource_init")
	var array:Array[WwiseExternalSourceInfo]
	array.resize(1)
	array[0] = WwiseExternalSourceInfo.new()
	array[0].external_src_cookie = Wwise.get_id_from_string("One")
	array[0].id_codec = AkUtils.AK_CODECID_ADPCM
	array[0].sz_file = "01.wem"
	var playing_id = Wwise.post_external_sources(
		event.id,
		node,
		array
	)
	Wwise.stop_event(playing_id, 0, AkUtils.AK_CURVE_LINEAR)
	free_node_3d(node)
	assert_int(playing_id).is_greater(0)


func test_get_source_play_position() -> void:
	var node = create_node_3d()
	var event:WwiseEvent = WwiseEvent.new()
	event.set_name("InteractiveMusicEvent")
	event.set_id(AK.EVENTS.INTERACTIVEMUSICEVENT)
	event.emit_signal("ws_post_resource_init")
	var playing_id = Wwise.post_event_callback("InteractiveMusicEvent", AkUtils.AK_ENABLE_GET_SOURCE_PLAY_POSITION, node, func(_data): pass)
	await get_tree().create_timer(1.2).timeout
	var pos:float = Wwise.get_source_play_position(playing_id, true)
	free_node_3d(node)
	assert_float(pos).is_greater(1000)


func test_get_playing_segment_info() -> void:
	var node = create_node_3d()
	var event:WwiseEvent = WwiseEvent.new()
	event.set_name("InteractiveMusicEvent")
	event.set_id(AK.EVENTS.INTERACTIVEMUSICEVENT)
	event.emit_signal("ws_post_resource_init")
	var playing_id = Wwise.post_event_callback("InteractiveMusicEvent", AkUtils.AK_ENABLE_GET_MUSIC_PLAY_POSITION, node, func(_data): pass)
	await get_tree().create_timer(1.2).timeout
	var info = Wwise.get_playing_segment_info(playing_id, true)
	free_node_3d(node)
	assert_bool(info.has("iCurrentPosition")).is_true()
	assert_bool(info.get("iCurrentPosition") > 1000).is_true()


func test_set_game_object_output_bus_volume() -> void:
	var game_obj = create_node_3d()
	var listener = create_node_3d()
	Wwise.register_game_obj(game_obj, "game_obj")
	Wwise.register_listener(listener)
	var result = Wwise.set_game_object_output_bus_volume(game_obj, listener, 0.8)
	free_node_3d(game_obj)
	free_node_3d(listener)
	assert_bool(result).is_true()


func test_set_game_object_aux_send_values() -> void:
	var game_obj = create_node_3d()
	Wwise.register_game_obj(game_obj, "game_obj")
	var aux_send_values = [
		{"aux_bus_id": AK.AUX_BUSSES.LOWER_PITCH_BUS, "control_value": 0.7},
	]
	var result = Wwise.set_game_object_aux_send_values(game_obj, aux_send_values, aux_send_values.size())
	free_node_3d(game_obj)
	assert_bool(result).is_true()


func test_set_object_obstruction_and_occlusion() -> void:
	var game_obj = create_node_3d()
	var listener = create_node_3d()
	Wwise.register_game_obj(game_obj, "game_obj")
	Wwise.register_listener(listener)
	var result = Wwise.set_object_obstruction_and_occlusion(game_obj, listener, 0.3, 0.5)
	free_node_3d(game_obj)
	free_node_3d(listener)
	assert_bool(result).is_true()


func test_set_geometry() -> void:
	var node = create_node_3d()
	Wwise.register_game_obj(node, "GeometryNode")
	var geometry_info = get_sample_geometry_info()
	var result = Wwise.set_geometry(geometry_info["vertices"], geometry_info["triangles"], WwiseAcousticTexture.new(), 1.0, node, true, true)
	Wwise.remove_geometry(node)
	free_node_3d(node)
	assert_bool(result).is_true()


func test_remove_geometry() -> void:
	var node = create_node_3d()
	Wwise.register_game_obj(node, "GeometryNode")
	var geometry_info = get_sample_geometry_info()
	Wwise.set_geometry(geometry_info["vertices"], geometry_info["triangles"], WwiseAcousticTexture.new(), 1.0, node, true, true)
	var result = Wwise.remove_geometry(node)
	free_node_3d(node)
	assert_bool(result).is_true()


func test_set_geometry_instance() -> void:
	var node = create_node_3d()
	var geometry_instance = Object.new()
	Wwise.register_game_obj(node, "GeometryNode")
	var geometry_info = get_sample_geometry_info()
	Wwise.set_geometry(geometry_info["vertices"], geometry_info["triangles"], WwiseAcousticTexture.new(), 1.0, node, true, true)
	var result = Wwise.set_geometry_instance(node, Transform3D(), geometry_instance)
	free_node_3d(node)
	geometry_instance.free()
	assert_bool(result).is_true()


func test_remove_geometry_instance() -> void:
	var node = create_node_3d()
	var geometry_instance = Object.new()
	Wwise.register_game_obj(node, "GeometryNode")
	var geometry_info = get_sample_geometry_info()
	Wwise.set_geometry(geometry_info["vertices"], geometry_info["triangles"], WwiseAcousticTexture.new(), 1.0, node, true, true)
	Wwise.set_geometry_instance(node, Transform3D(), geometry_instance)
	var result = Wwise.remove_geometry_instance(geometry_instance)
	Wwise.remove_geometry(node)
	free_node_3d(node)
	geometry_instance.free()
	assert_bool(result).is_true()


func test_register_spatial_listener():
	var node = create_node_3d()
	Wwise.register_listener(node)
	var result = Wwise.register_spatial_listener(node)
	assert_bool(result).is_true()


func test_set_room() -> void:
	var game_obj = create_node_3d()
	Wwise.register_game_obj(game_obj, "test_room")

	var ak_aux_bus_id = AK.AUX_BUSSES.LOWER_PITCH_BUS
	var reverb_level = 0.8
	var transmission_loss = 0.5
	var front_vector = Vector3(1, 0, 0)
	var up_vector = Vector3(0, 1, 0)
	var keep_registered = true

	var result = Wwise.set_room(
		game_obj,
		ak_aux_bus_id,
		reverb_level,
		transmission_loss,
		front_vector,
		up_vector,
		keep_registered,
		null
	)

	free_node_3d(game_obj)
	assert_bool(result).is_true()


func test_remove_room() -> void:
	var game_obj = create_node_3d()
	Wwise.register_game_obj(game_obj, "test_room")

	var ak_aux_bus_id = AK.AUX_BUSSES.LOWER_PITCH_BUS
	var reverb_level = 0.8
	var transmission_loss = 0.5
	var front_vector = Vector3(1, 0, 0)
	var up_vector = Vector3(0, 1, 0)
	var keep_registered = true

	Wwise.set_room(
		game_obj,
		ak_aux_bus_id,
		reverb_level,
		transmission_loss,
		front_vector,
		up_vector,
		keep_registered,
		null
	)

	var result = Wwise.remove_room(game_obj)
	free_node_3d(game_obj)
	assert_bool(result).is_true()


func test_set_portal() -> void:
	var node = create_node_3d()
	node.transform.origin = Vector3(5, 23, -53)
	var extent = Vector3(4, 2, 2)
	var enabled = true
	var front_room = Node.new()
	var back_room = Node.new()
	var result = Wwise.set_portal(node, node.transform, extent, front_room, back_room, enabled)
	Wwise.remove_portal(node)
	free_node_3d(node)
	front_room.queue_free()
	back_room.queue_free()
	assert_bool(result).is_true()


func test_remove_portal() -> void:
	var node = create_node_3d()
	node.transform.origin = Vector3(5, 23, -53)
	var extent = Vector3(4, 2, 2)
	var enabled = true
	var front_room = Node.new()
	var back_room = Node.new()
	Wwise.set_portal(node, node.transform, extent, front_room, back_room, enabled)
	var result = Wwise.remove_portal(node)
	free_node_3d(node)
	front_room.queue_free()
	back_room.queue_free()
	assert_bool(result).is_true()


func test_set_portal_obstruction_and_occlusion() -> void:
	var node = create_node_3d()
	node.transform.origin = Vector3(5, 23, -53)
	var extent = Vector3(4, 2, 2)
	var enabled = true
	var front_room = Node.new()
	var back_room = Node.new()
	Wwise.set_portal(node, node.transform, extent, front_room, back_room, enabled)
	var result = Wwise.set_portal_obstruction_and_occlusion(node, 1.0, 0.4)
	Wwise.remove_portal(node)
	free_node_3d(node)
	front_room.queue_free()
	back_room.queue_free()
	assert_bool(result).is_true()


func test_set_game_object_to_portal_obstruction() -> void:
	var node = create_node_3d()
	var game_obj = create_node_3d()
	Wwise.register_game_obj(game_obj, "game_obj")
	node.transform.origin = Vector3(5, 23, -53)
	var extent = Vector3(4, 2, 2)
	var enabled = true
	var front_room = Node.new()
	var back_room = Node.new()
	Wwise.set_portal(node, node.transform, extent, front_room, back_room, enabled)
	var result = Wwise.set_game_object_to_portal_obstruction(game_obj, node, 0.5)
	Wwise.remove_portal(node)
	free_node_3d(node)
	front_room.queue_free()
	back_room.queue_free()
	assert_bool(result).is_true()


func test_set_portal_to_portal_obstruction() -> void:
	var node = create_node_3d()
	node.transform.origin = Vector3(5, 23, -53)
	var extent = Vector3(4, 2, 2)
	var enabled = true
	var front_room = Node.new()
	var back_room = Node.new()
	Wwise.set_portal(node, node.transform, extent, front_room, back_room, enabled)

	var node2 = create_node_3d()
	node2.transform.origin = Vector3(5, 23, -53)
	var extent2 = Vector3(4, 2, 2)
	var enabled2 = true
	var front_room2 = Node.new()
	var back_room2 = Node.new()
	Wwise.set_portal(node2, node2.transform, extent2, front_room2, back_room2, enabled2)

	var result = Wwise.set_portal_to_portal_obstruction(node, node2, 0.5)

	Wwise.remove_portal(node)
	free_node_3d(node)
	front_room.queue_free()
	back_room.queue_free()

	Wwise.remove_portal(node2)
	free_node_3d(node2)
	front_room2.queue_free()
	back_room2.queue_free()

	assert_bool(result).is_true()


func test_set_game_object_in_room() -> void:
	var game_obj = create_node_3d()
	var room = create_node_3d()
	Wwise.register_game_obj(game_obj, "test_game_obj")
	Wwise.register_game_obj(room, "test_room")

	var result = Wwise.set_game_object_in_room(game_obj, room)

	free_node_3d(game_obj)
	free_node_3d(room)

	assert_bool(result).is_true()


func test_remove_game_object_from_room() -> void:
	var game_obj = create_node_3d()
	var room = create_node_3d()
	Wwise.register_game_obj(game_obj, "test_game_obj")
	Wwise.register_game_obj(room, "test_room")

	Wwise.set_game_object_in_room(game_obj, room)
	var result = Wwise.remove_game_object_from_room(game_obj)

	free_node_3d(game_obj)
	free_node_3d(room)

	assert_bool(result).is_true()


func test_set_early_reflections_aux_send() -> void:
	var game_obj = create_node_3d()
	var aux_bus_id = AK.AUX_BUSSES.LOWER_PITCH_BUS
	var result = Wwise.set_early_reflections_aux_send(game_obj, aux_bus_id)
	free_node_3d(game_obj)
	assert_bool(result).is_true()


func test_set_early_reflections_volume() -> void:
	var game_obj = create_node_3d()
	var volume = 0.75
	var result = Wwise.set_early_reflections_volume(game_obj, volume)
	free_node_3d(game_obj)
	assert_bool(result).is_true()


func test_get_id_from_string() -> void:
	var id = Wwise.get_id_from_string("Looping_Event")
	assert_int(id).is_equal(AK.EVENTS.LOOPING_EVENT)

extends "res://addons/Wwise/tests/ak_test.gd"

const AK = preload("res://addons/Wwise/tests/GeneratedSoundBanks/wwise_ids_tests.gd")
var mesh_instance: MeshInstance3D


func before_all():
	mesh_instance = MeshInstance3D.new()
	mesh_instance.name = "Mesh Instance"
	var mesh = BoxMesh.new()
	mesh_instance.set_mesh(mesh)


func after_all():
	pass


func _ready():
	pass


func run(randomize = false):
	tests = get_method_names()
	return await run_tests(tests, randomize)


func register_3d_game_obj():
	var node = Node3D.new()
	add_child(node)
	node.owner = self
	Wwise.register_game_obj(node, "game_obj")
	return node


func unregister_3d_game_obj(node):
	Wwise.unregister_game_obj(node)
	node.queue_free()


func register_2d_game_obj():
	var node = Node2D.new()
	Wwise.register_game_obj(node, "game_obj")
	return node


func unregister_2d_game_obj(node):
	Wwise.unregister_game_obj(node)
	node.queue_free()


func test_set_base_path() -> bool:
	var result = Wwise.set_base_path("res://wwise/GeneratedSoundBanks")
	return ak_assert(result)


func test_register_listener() -> bool:
	var listener = Node3D.new()
	var result = Wwise.register_listener(listener)
	listener.queue_free()
	return ak_assert(result)


func test_register_game_obj() -> bool:
	var game_obj = Node.new()
	var result = Wwise.register_game_obj(game_obj, "game obj")
	Wwise.unregister_game_obj(game_obj)
	game_obj.queue_free()
	return ak_assert(result)


func test_unregister_game_obj() -> bool:
	var game_obj = Node.new()
	Wwise.register_game_obj(game_obj, "game obj")
	var result = Wwise.unregister_game_obj(game_obj)
	game_obj.queue_free()
	return ak_assert(result)


func test_set_listeners() -> bool:
	var listener = Node3D.new()
	Wwise.register_listener(listener)
	var game_obj = Node.new()
	Wwise.register_game_obj(game_obj, "emitter")
	var result = Wwise.set_listeners(game_obj, listener)
	listener.queue_free()
	game_obj.queue_free()
	return ak_assert(result)


func test_set_3d_position() -> bool:
	var node = register_3d_game_obj()
	var result = Wwise.set_3d_position(node, node.get_global_transform())
	unregister_3d_game_obj(node)
	return ak_assert(result)


func test_set_2d_position() -> bool:
	var node = register_2d_game_obj()
	var result = Wwise.set_2d_position(node, node.get_global_transform(), 1.0)
	unregister_2d_game_obj(node)
	return ak_assert(result)


func test_set_multiple_positions_3d() -> bool:
	var node = register_3d_game_obj()
	var node_2 = register_3d_game_obj()
	var positions: Array[Transform3D]
	positions.push_back(node.get_global_transform())
	positions.push_back(node_2.get_global_transform())
	var result = Wwise.set_multiple_positions_3d(node, positions, 2, AkUtils.TYPE_MULTI_SOURCES)
	unregister_3d_game_obj(node)
	unregister_3d_game_obj(node_2)
	return ak_assert(result)


func test_set_multiple_positions_2d() -> bool:
	var node = register_2d_game_obj()
	var node_2 = register_2d_game_obj()
	var positions: Array[Transform2D]
	positions.push_back(node.get_global_transform())
	positions.push_back(node_2.get_global_transform())
	var result = Wwise.set_multiple_positions_2d(
		node, positions, [2.0, 5.0], 2, AkUtils.TYPE_MULTI_SOURCES
	)
	unregister_2d_game_obj(node)
	unregister_2d_game_obj(node_2)
	return ak_assert(result)


func test_set_game_object_radius() -> bool:
	var node = register_3d_game_obj()
	var result = Wwise.set_game_object_radius(node, 5.0, 3.0)
	unregister_3d_game_obj(node)
	return ak_assert(result)


func test_post_event() -> bool:
	var node = register_3d_game_obj()
	var result = Wwise.post_event("MUSIC", node)
	unregister_3d_game_obj(node)
	return ak_assert(result)


func test_post_event_callback() -> bool:
	var node = register_3d_game_obj()
	var cookie_wrapper = CookieWrapper.new()
	cookie_wrapper.cookie = func(data): pass
	var result = Wwise.post_event_callback("MUSIC", AkUtils.AK_DURATION, node, cookie_wrapper)
	unregister_3d_game_obj(node)
	cookie_wrapper.call_deferred("free")
	return ak_assert(result)


func test_post_event_id() -> bool:
	var node = register_3d_game_obj()
	var result = Wwise.post_event_id(AK.EVENTS.MUSIC, node)
	unregister_3d_game_obj(node)
	return ak_assert(result)


func test_post_event_id_callback() -> bool:
	var node = register_3d_game_obj()
	var cookie_wrapper = CookieWrapper.new()
	cookie_wrapper.cookie = func(data): pass
	var result = Wwise.post_event_id_callback(
		AK.EVENTS.MUSIC, AkUtils.AK_DURATION, node, cookie_wrapper
	)
	unregister_3d_game_obj(node)
	cookie_wrapper.call_deferred("free")
	return ak_assert(result)


func test_set_switch() -> bool:
	var node = register_3d_game_obj()
	var result = Wwise.set_switch("FootstepsSwitch", "Wood", node)
	unregister_3d_game_obj(node)
	return ak_assert(result)


func test_set_switch_id() -> bool:
	var node = register_3d_game_obj()
	var result = Wwise.set_switch_id(
		AK.SWITCHES.FOOTSTEPSSWITCH.GROUP, AK.SWITCHES.FOOTSTEPSSWITCH.SWITCH.WOOD, node
	)
	unregister_3d_game_obj(node)
	return ak_assert(result)


func test_set_state() -> bool:
	var result = Wwise.set_state("MusicState", "Intense")
	return ak_assert(result)


func test_set_state_id() -> bool:
	var result = Wwise.set_state_id(AK.STATES.MUSICSTATE.GROUP, AK.STATES.MUSICSTATE.STATE.INTENSE)
	return ak_assert(result)


func test_get_rtpc_value() -> bool:
	var node = register_3d_game_obj()
	Wwise.set_rtpc_value("Enemies", 3.0, node)
	await get_tree().process_frame
	var value = Wwise.get_rtpc_value("Enemies", node)
	unregister_3d_game_obj(node)
	return ak_assert(value == 3.0)


func test_get_rtpc_value_id() -> bool:
	var node = register_3d_game_obj()
	Wwise.set_rtpc_value_id(AK.GAME_PARAMETERS.ENEMIES, 6.0, node)
	await get_tree().create_timer(1.0).timeout
	var value = Wwise.get_rtpc_value_id(AK.GAME_PARAMETERS.ENEMIES, node)
	unregister_3d_game_obj(node)
	return ak_assert(value == 6.0)


func test_post_trigger() -> bool:
	var node = register_3d_game_obj()
	var result = Wwise.post_trigger("MusicTrigger", node)
	unregister_3d_game_obj(node)
	return ak_assert(result)


func test_post_trigger_id() -> bool:
	var node = register_3d_game_obj()
	var result = Wwise.post_trigger_id(AK.TRIGGERS.MUSICTRIGGER, node)
	unregister_3d_game_obj(node)
	return ak_assert(result)


func test_post_external_source() -> bool:
	var node = register_3d_game_obj()
	var playing_id = Wwise.post_external_source(
		"External_Source_Event",
		node,
		"External_Source",
		"ExternalSources/External_Source_Demo.wem",
		AkUtils.AK_CODECID_ADPCM
	)
	Wwise.stop_event(playing_id, 0, AkUtils.AK_CURVE_LINEAR)
	unregister_3d_game_obj(node)
	return ak_assert(playing_id > 0)


func test_post_external_source_id() -> bool:
	var node = register_3d_game_obj()
	var playing_id = Wwise.post_external_source_id(
		AK.EVENTS.EXTERNAL_SOURCE_EVENT,
		node,
		AK.EXTERNAL_SOURCES.EXTERNAL_SOURCE,
		"ExternalSources/External_Source_Demo.wem",
		AkUtils.AK_CODECID_ADPCM
	)
	Wwise.stop_event(playing_id, 0, AkUtils.AK_CURVE_LINEAR)
	unregister_3d_game_obj(node)
	return ak_assert(playing_id > 0)


func test_set_geometry() -> bool:
	var ak_geometry = AkGeometry.new()
	ak_geometry.name = "AkGeometry"
	mesh_instance.add_child(ak_geometry)
	#warning-ignore:return_value_discarded
	ak_geometry.set_owner(mesh_instance)
	add_child(mesh_instance)
	var result = ak_geometry.set_geometry(mesh_instance)
	Wwise.remove_geometry(ak_geometry)
	mesh_instance.remove_child(ak_geometry)
	return ak_assert(result)


func test_remove_geometry() -> bool:
	var ak_geometry = AkGeometry.new()
	ak_geometry.name = "AkGeometry"
	mesh_instance.add_child(ak_geometry)
	#warning-ignore:return_value_discarded
	ak_geometry.set_owner(mesh_instance)
	ak_geometry.set_geometry(mesh_instance)
	await get_tree().process_frame
	var result = Wwise.remove_geometry(ak_geometry)
	remove_child(mesh_instance)
	return ak_assert(result)


func test_register_spatial_listener() -> bool:
	var game_obj = register_3d_game_obj()
	var result = Wwise.register_spatial_listener(game_obj)
	unregister_3d_game_obj(game_obj)
	return ak_assert(result)


func test_set_portal() -> bool:
	var spatial = Node3D.new()
	spatial.transform.origin = Vector3(5, 23, -53)
	var extent = Vector3(4, 2, 2)
	var enabled = true
	var front_room = Node.new()
	var back_room = Node.new()
	add_child(spatial)
	var result = Wwise.set_portal(
		spatial, spatial.transform, extent, front_room, back_room, enabled
	)
	Wwise.remove_portal(spatial)
	remove_child(spatial)
	return ak_assert(result)


func test_remove_portal() -> bool:
	var spatial = Node3D.new()
	spatial.transform.origin = Vector3(5, 23, -53)
	var extent = Vector3(4, 2, 2)
	var enabled = true
	var front_room = Node.new()
	var back_room = Node.new()
	add_child(spatial)
	Wwise.set_portal(spatial, spatial.transform, extent, front_room, back_room, enabled)
	var result = Wwise.remove_portal(spatial)
	remove_child(spatial)
	return ak_assert(result)


func test_set_portal_obstruction_and_occlusion() -> bool:
	var spatial = Node3D.new()
	spatial.transform.origin = Vector3(5, 23, -53)
	var extent = Vector3(4, 2, 2)
	var enabled = true
	var front_room = Node.new()
	var back_room = Node.new()
	add_child(spatial)
	Wwise.set_portal(spatial, spatial.transform, extent, front_room, back_room, enabled)
	var result = Wwise.set_portal_obstruction_and_occlusion(spatial, 1.0, 1.0)
	Wwise.remove_portal(spatial)
	remove_child(spatial)
	return ak_assert(result)


func test_set_game_object_to_portal_obstruction() -> bool:
	var game_obj = register_3d_game_obj()
	var spatial = Node3D.new()
	spatial.transform.origin = Vector3(5, 23, -53)
	var extent = Vector3(4, 2, 2)
	var enabled = true
	var front_room = Node.new()
	var back_room = Node.new()
	add_child(spatial)
	Wwise.set_portal(spatial, spatial.transform, extent, front_room, back_room, enabled)
	var result = Wwise.set_game_object_to_portal_obstruction(game_obj, spatial, 1.0)
	Wwise.remove_portal(spatial)
	remove_child(spatial)
	unregister_3d_game_obj(game_obj)
	return ak_assert(result)


func test_set_portal_to_portal_obstruction() -> bool:
	var spatial = Node3D.new()
	spatial.transform.origin = Vector3(5, 23, -53)
	var extent = Vector3(4, 2, 2)
	var enabled = true
	var front_room = Node.new()
	var back_room = Node.new()
	add_child(spatial)
	Wwise.set_portal(spatial, spatial.transform, extent, front_room, back_room, enabled)

	var spatial2 = Node3D.new()
	spatial2.transform.origin = Vector3(5, 23, -53)
	var extent2 = Vector3(4, 2, 2)
	var enabled2 = true
	var front_room2 = Node.new()
	var back_room2 = Node.new()
	add_child(spatial2)
	Wwise.set_portal(spatial2, spatial2.transform, extent2, front_room2, back_room2, enabled2)

	var result = Wwise.set_portal_to_portal_obstruction(spatial, spatial2, 1.0)
	Wwise.remove_portal(spatial)
	Wwise.remove_portal(spatial2)
	remove_child(spatial)
	remove_child(spatial2)

	return ak_assert(result)


func test_set_early_reflections_aux_send() -> bool:
	var game_obj = register_3d_game_obj()
	var result = Wwise.set_early_reflections_aux_send(game_obj, AK.AUX_BUSSES.LARGEVERB)
	unregister_3d_game_obj(game_obj)
	return ak_assert(result)


func test_set_early_reflections_volume() -> bool:
	var game_obj = register_3d_game_obj()
	var result = Wwise.set_early_reflections_volume(game_obj, 0.5)
	unregister_3d_game_obj(game_obj)
	return ak_assert(result)

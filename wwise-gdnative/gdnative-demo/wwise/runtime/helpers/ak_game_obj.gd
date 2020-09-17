extends Spatial

var signal_prefix:String = "_on_"

func register_game_object(object:Object, gameObjectName:String) -> void:
	Wwise.register_game_obj(object, gameObjectName)

func get_listener() -> Spatial: 
	return get_tree().get_root().find_node("AkListener", true, false) as Spatial
	# haaacky and slow, needs a better solution ^_^"
	
# Instantiating a RayCast node, setting it as child of the event's node. 
# It is used in compute_occlusion to check if there is any object between Event 
# and Listener.
func set_up_raycast(event:Spatial) -> RayCast:
	var ray:RayCast = RayCast.new()
	ray.enabled = true
	ray.set_name("ray")
	event.add_child(ray)
	ray.set_owner(event)
	return ray

# Without using Spatial Audio, Wwise uses Physics.Raycast in the Unity integration
# to check if there are any Objects between Event and Listener.
# Then calls AK::SoundEngine::SetObjectObstructionAndOcclusion to set the obstruction
# and occlusion values based on the number of objects hit. Here we are casting a ray
# in the direction of the Listener and adding the OCCLUSION_ADDEND value to the occlusion
# variable for each object between Event and Listener. 
func compute_occlusion(listener_transform:Transform, source_transform:Transform, colliding_objects:Array, ray:RayCast) -> float:
	colliding_objects.clear()
	var occlusion_value:float = 0.0

	ray.set_cast_to(listener_transform.origin - source_transform.origin)
	while ray.is_colliding():
		var obj = ray.get_collider()
		if colliding_objects.find(obj) == -1:
			colliding_objects.append(obj)
		ray.add_exception(obj)
		ray.force_raycast_update()
		if colliding_objects.size() > 0:
			occlusion_value += AkUtils.OCCLUSION_ADDEND
	for obj in colliding_objects:
		ray.remove_exception(obj)
	return occlusion_value

# Here we are actually calling AK::SoundEngine::SetObjectObstructionAndOcclusion.
# Since it doesn't need to be every frame, we do that based on the value defined
# in OCCLUSION_DETECTION_INTERVAL (milliseconds)
func set_obstruction_and_occlusion(event:Spatial, listener:Spatial, colliding_objects:Array, ray:RayCast, next_occlusion_update:int) -> void:
	if OS.get_ticks_msec() >= next_occlusion_update:
		next_occlusion_update = OS.get_ticks_msec() + AkUtils.OCCLUSION_DETECTION_INTERVAL
		var current_occlusion:float = compute_occlusion(listener.get_global_transform(), self.get_global_transform(), colliding_objects, ray)
		Wwise.set_obj_obstruction_and_occlusion(event.get_instance_id(), listener.get_instance_id(), 0, current_occlusion)
		
func connect_signals(callback_receiver:NodePath, callback_type) -> void:
	if callback_receiver.is_empty():
		print("Please select a callback node")
	else:
		var callback_node:Node = get_node(callback_receiver)
		match(callback_type):
			AkUtils.AkCallbackType.AK_EndOfEvent:
				Wwise.connect(AkUtils.Signals.END_OF_EVENT, callback_node, signal_prefix + AkUtils.Signals.END_OF_EVENT)
					
			AkUtils.AkCallbackType.AK_EndOfDynamicSequenceItem:
				Wwise.connect(AkUtils.Signals.END_OF_DYNAMIC_SEQUENCE_ITEM, callback_node, signal_prefix + AkUtils.Signals.END_OF_DYNAMIC_SEQUENCE_ITEM)
				
			AkUtils.AkCallbackType.AK_Marker:
				Wwise.connect(AkUtils.Signals.AUDIO_MARKER, callback_node, signal_prefix + AkUtils.Signals.AUDIO_MARKER)
				
			AkUtils.AkCallbackType.AK_Duration:
				Wwise.connect(AkUtils.Signals.AUDIO_DURATION, callback_node, signal_prefix + AkUtils.Signals.AUDIO_DURATION)
				
			AkUtils.AkCallbackType.AK_SpeakerVolumeMatrix:
				Wwise.connect(AkUtils.Signals.SPEAKER_VOLUME_MATRIX, callback_node, signal_prefix + AkUtils.Signals.SPEAKER_VOLUME_MATRIX)
				
			AkUtils.AkCallbackType.AK_Starvation:
				Wwise.connect(AkUtils.Signals.AUDIO_STARVATION, callback_node, signal_prefix + AkUtils.Signals.AUDIO_STARVATION)
				
			AkUtils.AkCallbackType.AK_MusicPlaylistSelect:
				Wwise.connect(AkUtils.Signals.MUSIC_PLAYLIST_SELECT, callback_node, signal_prefix + AkUtils.Signals.MUSIC_PLAYLIST_SELECT)
				
			AkUtils.AkCallbackType.AK_MusicPlayStarted:
				Wwise.connect(AkUtils.Signals.MUSIC_PLAY_STARTED, callback_node, signal_prefix + AkUtils.Signals.MUSIC_PLAY_STARTED)
					
			AkUtils.AkCallbackType.AK_MusicSyncBeat:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_BEAT, callback_node, signal_prefix + AkUtils.Signals.MUSIC_SYNC_BEAT)
					
			AkUtils.AkCallbackType.AK_MusicSyncBar:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_BAR, callback_node, signal_prefix + AkUtils.Signals.MUSIC_SYNC_BAR)
					
			AkUtils.AkCallbackType.AK_MusicSyncEntry:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_ENTRY, callback_node, signal_prefix + AkUtils.Signals.MUSIC_SYNC_ENTRY)
					
			AkUtils.AkCallbackType.AK_MusicSyncExit:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_EXIT, callback_node, signal_prefix + AkUtils.Signals.MUSIC_SYNC_EXIT)
					
			AkUtils.AkCallbackType.AK_MusicSyncGrid:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_GRID, callback_node, signal_prefix + AkUtils.Signals.MUSIC_SYNC_GRID)
				
			AkUtils.AkCallbackType.AK_MusicSyncUser:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_USER_CUE, callback_node, signal_prefix + AkUtils.Signals.MUSIC_SYNC_USER_CUE)
					
			AkUtils.AkCallbackType.AK_MusicSyncPoint:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_POINT, callback_node, signal_prefix + AkUtils.Signals.MUSIC_SYNC_POINT)
					
			AkUtils.AkCallbackType.AK_MusicSyncAll:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_ALL, callback_node, signal_prefix + AkUtils.Signals.MUSIC_SYNC_ALL)
					
			AkUtils.AkCallbackType.AK_MIDIEvent:
				Wwise.connect(AkUtils.Signals.MIDI_EVENT, callback_node, signal_prefix + AkUtils.Signals.MIDI_EVENT)
				
			AkUtils.AkCallbackType.AK_CallbackBits:
				Wwise.connect(AkUtils.Signals.CALLBACK_BITS, callback_node, signal_prefix + AkUtils.Signals.CALLBACK_BITS)
				
			AkUtils.AkCallbackType.AK_EnableGetSourcePlayPosition:
				Wwise.connect(AkUtils.Signals.ENABLE_GET_SOURCE_PLAY_POSITION, callback_node, signal_prefix + AkUtils.Signals.ENABLE_GET_SOURCE_PLAY_POSITION)
				
			AkUtils.AkCallbackType.AK_EnableGetMusicPlayPosition:
				Wwise.connect(AkUtils.Signals.ENABLE_GET_MUSIC_PLAY_POSITION, callback_node, signal_prefix + AkUtils.Signals.ENABLE_GET_MUSIC_PLAY_POSITION)
				
			AkUtils.AkCallbackType.AK_EnableGetSourceStreamBuffering:
				Wwise.connect(AkUtils.Signals.ENABLE_GET_SOURCE_STREAM_BUFFERING, callback_node, signal_prefix + AkUtils.Signals.ENABLE_GET_SOURCE_STREAM_BUFFERING)

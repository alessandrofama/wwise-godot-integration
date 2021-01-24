extends Spatial

var signal_prefix:String = "_on_"

export(AK.EVENTS._enum) var event:int = AK.EVENTS._enum.values()[0]
export(AkUtils.GameEvent) var trigger_on:int = AkUtils.GameEvent.NONE
export(AkUtils.GameEvent) var stop_on:int = AkUtils.GameEvent.NONE
export(int) var stop_fade_time:int = 0
export(AkUtils.AkCurveInterpolation) var stop_interpolation_curve:int = AkUtils.AkCurveInterpolation.LINEAR
export(bool) var is_environment_aware:bool = false;
export(bool) var is_spatial:bool = false
export(bool) var use_callback:bool = false

export(int, FLAGS, 
"END_OF_EVENT",
"END_OF_DYNAMIC_SEQUENCE_ITEM",              
"MARKER",                
"DURATION",                 
"SPEAKER_VOLUME_MATRIX",              
"STARVATION",                 
"MUSIC_PLAYLIST_SELECT",
"MUSIC_PLAY_STARTED",
"MUSIC_SYNC_BEAT",
"MUSIC_SYNC_BAR",
"MUSIC_SYNC_ENTRY",             
"MUSIC_SYNC_EXIT",        
"MUSIC_SYNC_GRID",         
"MUSIC_SYNC_USER_CUE",           
"MUSIC_SYNC_POINT",          
"MUSIC_SYNC_ALL",         
"MIDI_EVENT",         
"CALLBACK_BITS",     
"ENABLE_GET_SOURCE_PLAY_POSITION",
"ENABLE_GET_MUSIC_PLAY_POSITION",
"ENABLE_GET_SOURCE_STREAM_BUFFERING"                       
) var callback_flag = 0

signal end_of_event(data)
signal end_of_dynamic_sequence_item(data)
signal marker(data)
signal duration(data)
signal speaker_volume_matrix(data)
signal starvation(data)
signal music_playlist_select(data)
signal music_play_started(data)
signal music_sync_beat(data)
signal music_sync_bar(data)
signal music_sync_entry(data)
signal music_sync_exit(data)
signal music_sync_grid(data)
signal music_sync_user_cue(data)
signal music_sync_point(data)
signal music_sync_all(data)
signal midi_event(data)
signal callback_bits(data)

func register_game_object(object:Object, gameObjectName:String) -> void:
	Wwise.register_game_obj(object, gameObjectName)

func get_listener() -> Spatial: 
	return get_tree().get_root().find_node("AkListener", true, false) as Spatial
	# haaacky and slow, needs a better solution ^_^"
	
# Instantiating a RayCast node, setting it as child of the event's node. 
# It is used in compute_occlusion to check if there is any object between Event 
# and Listener.
func set_up_raycast(_event:Spatial) -> RayCast:
	var ray:RayCast = RayCast.new()
	ray.enabled = true
	ray.set_name("ray")
	_event.add_child(ray)
	ray.set_owner(_event)
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
func set_obstruction_and_occlusion(_event:Spatial, listener:Spatial, colliding_objects:Array, ray:RayCast, next_occlusion_update:int) -> void:
	if OS.get_ticks_msec() >= next_occlusion_update:
		next_occlusion_update = OS.get_ticks_msec() + AkUtils.OCCLUSION_DETECTION_INTERVAL
		var current_occlusion:float = compute_occlusion(listener.get_global_transform(), self.get_global_transform(), colliding_objects, ray)
		Wwise.set_obj_obstruction_and_occlusion(_event.get_instance_id(), listener.get_instance_id(), current_occlusion, current_occlusion)
		
func connect_signals(callback:int) -> void:
		match(callback):
			AkUtils.AkCallbackType.AK_EndOfEvent:
				Wwise.connect(AkUtils.Signals.END_OF_EVENT, self, signal_prefix + AkUtils.Signals.END_OF_EVENT)
					
			AkUtils.AkCallbackType.AK_EndOfDynamicSequenceItem:
				Wwise.connect(AkUtils.Signals.END_OF_DYNAMIC_SEQUENCE_ITEM, self, signal_prefix + AkUtils.Signals.END_OF_DYNAMIC_SEQUENCE_ITEM)
				
			AkUtils.AkCallbackType.AK_Marker:
				Wwise.connect(AkUtils.Signals.AUDIO_MARKER, self, signal_prefix + AkUtils.Signals.AUDIO_MARKER)
				
			AkUtils.AkCallbackType.AK_Duration:
				Wwise.connect(AkUtils.Signals.AUDIO_DURATION, self, signal_prefix + AkUtils.Signals.AUDIO_DURATION)
				
			AkUtils.AkCallbackType.AK_SpeakerVolumeMatrix:
				Wwise.connect(AkUtils.Signals.SPEAKER_VOLUME_MATRIX, self, signal_prefix + AkUtils.Signals.SPEAKER_VOLUME_MATRIX)
				
			AkUtils.AkCallbackType.AK_Starvation:
				Wwise.connect(AkUtils.Signals.AUDIO_STARVATION, self, signal_prefix + AkUtils.Signals.AUDIO_STARVATION)
				
			AkUtils.AkCallbackType.AK_MusicPlaylistSelect:
				Wwise.connect(AkUtils.Signals.MUSIC_PLAYLIST_SELECT, self, signal_prefix + AkUtils.Signals.MUSIC_PLAYLIST_SELECT)
				
			AkUtils.AkCallbackType.AK_MusicPlayStarted:
				Wwise.connect(AkUtils.Signals.MUSIC_PLAY_STARTED, self, signal_prefix + AkUtils.Signals.MUSIC_PLAY_STARTED)
					
			AkUtils.AkCallbackType.AK_MusicSyncBeat:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_BEAT, self, signal_prefix + AkUtils.Signals.MUSIC_SYNC_BEAT)
					
			AkUtils.AkCallbackType.AK_MusicSyncBar:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_BAR, self, signal_prefix + AkUtils.Signals.MUSIC_SYNC_BAR)
					
			AkUtils.AkCallbackType.AK_MusicSyncEntry:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_ENTRY, self, signal_prefix + AkUtils.Signals.MUSIC_SYNC_ENTRY)
					
			AkUtils.AkCallbackType.AK_MusicSyncExit:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_EXIT, self, signal_prefix + AkUtils.Signals.MUSIC_SYNC_EXIT)
					
			AkUtils.AkCallbackType.AK_MusicSyncGrid:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_GRID, self, signal_prefix + AkUtils.Signals.MUSIC_SYNC_GRID)
				
			AkUtils.AkCallbackType.AK_MusicSyncUserCue:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_USER_CUE, self, signal_prefix + AkUtils.Signals.MUSIC_SYNC_USER_CUE)
					
			AkUtils.AkCallbackType.AK_MusicSyncPoint:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_POINT, self, signal_prefix + AkUtils.Signals.MUSIC_SYNC_POINT)
					
			AkUtils.AkCallbackType.AK_MusicSyncAll:
				Wwise.connect(AkUtils.Signals.MUSIC_SYNC_ALL, self, signal_prefix + AkUtils.Signals.MUSIC_SYNC_ALL)
					
			AkUtils.AkCallbackType.AK_MIDIEvent:
				Wwise.connect(AkUtils.Signals.MIDI_EVENT, self, signal_prefix + AkUtils.Signals.MIDI_EVENT)
				
			AkUtils.AkCallbackType.AK_CallbackBits:
				Wwise.connect(AkUtils.Signals.CALLBACK_BITS, self, signal_prefix + AkUtils.Signals.CALLBACK_BITS)
				
			AkUtils.AkCallbackType.AK_EnableGetSourcePlayPosition:
				Wwise.connect(AkUtils.Signals.ENABLE_GET_SOURCE_PLAY_POSITION, self, signal_prefix + AkUtils.Signals.ENABLE_GET_SOURCE_PLAY_POSITION)
				
			AkUtils.AkCallbackType.AK_EnableGetMusicPlayPosition:
				Wwise.connect(AkUtils.Signals.ENABLE_GET_MUSIC_PLAY_POSITION, self, signal_prefix + AkUtils.Signals.ENABLE_GET_MUSIC_PLAY_POSITION)
				
			AkUtils.AkCallbackType.AK_EnableGetSourceStreamBuffering:
				Wwise.connect(AkUtils.Signals.ENABLE_GET_SOURCE_STREAM_BUFFERING, self, signal_prefix + AkUtils.Signals.ENABLE_GET_SOURCE_STREAM_BUFFERING)

func _on_end_of_event(data):
	emit_signal("end_of_event", data)

func _on_end_of_dynamic_sequence_item(data):
	emit_signal("end_of_dynamic_sequence_item", data)
	
func _on_audio_marker(data):
	emit_signal("marker", data)

func _on_audio_duration(data):
	emit_signal("duration", data)
	
func _on_speaker_volume_matrix(data):
	emit_signal("speaker_volume_matrix", data)

func _on_audio_starvation(data):
	emit_signal("starvation", data)
	
func _on_music_playlist_select(data):
	emit_signal("music_playlist_select", data)

func _on_music_play_started(data):
	emit_signal("music_play_started", data)
	
func _on_music_sync_beat(data):
	emit_signal("music_sync_beat", data)

func _on_music_sync_bar(data):
	emit_signal("music_sync_bar", data)
	
func _on_music_sync_entry(data):
	emit_signal("music_sync_entry", data)

func _on_music_sync_exit(data):
	emit_signal("music_sync_exit", data)
	
func _on_music_sync_grid(data):
	emit_signal("music_sync_grid", data)

func _on_music_sync_user_cue(data):
	emit_signal("music_sync_user_cue", data)
	
func _on_music_sync_point(data):
	emit_signal("music_sync_point", data)

func _on_music_sync_all(data):
	emit_signal("music_sync_all", data)
	
func _on_midi_event(data):
	emit_signal("midi_event", data)

func _on_callback_bits(data):
	emit_signal("callback_bits", data)

extends AkEventHandler
class_name AkEvent

export(AK.EVENTS._enum) var event:int
export(AkUtils.GameEvent) var trigger_on:int = AkUtils.GameEvent.NONE
export(AkUtils.GameEvent) var stop_on:int = AkUtils.GameEvent.NONE
export(int) var stop_fade_time = 0
export (AkUtils.AkInterpolationMode) var interpolation_mode = AkUtils.AkInterpolationMode.LINEAR

var playingID:int
var signal_prefix = "_on_"

export(bool) var use_callback = false
export(AkUtils.AkCallbackType) var callback_type = AkUtils.AkCallbackType.AK_EndOfEvent
export(NodePath) var callback_receiver:NodePath

var listener:Spatial
var next_occlusion_update:float = 0
var ray:RayCast
var colliding_objects:Array = []

func _enter_tree():
	register_game_object(self, self.get_name())
	
func _ready() -> void:
	if use_callback:
		connect_signals()
	if is_environment_aware:
		listener = get_listener()
		set_up_raycast()
	
func set_up_raycast() -> void:
	ray = RayCast.new()
	ray.enabled = true
	ray.set_name("ray")
	add_child(ray)
	ray.set_owner(self)
	set_process(true)

func connect_signals() -> void:
	if callback_receiver.is_empty():
		print("Please select a callback node")
	else:
		var callback_node = get_node(callback_receiver)
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
				
func handle_game_event(gameEvent:int) -> void:
	if trigger_on == gameEvent:
		post_event()
	if stop_on == gameEvent:
		stop_event()
		
func post_event() -> void:
	if not use_callback:
		playingID = Wwise.post_event_id(event, self)
	else:
		playingID = Wwise.post_event_id_callback(event, callback_type, self)
	
func stop_event() -> void:
	Wwise.stop_event(playingID, stop_fade_time, interpolation_mode)
	
func _process(_delta) -> void:
	Wwise.set_3d_position(self, get_global_transform())
	
func _physics_process(_delta) -> void:
	if is_environment_aware:
		set_obstruction_and_occlusion()
	
func set_environment(ak_aux_send_value:Dictionary) -> void:
	Wwise.set_game_obj_aux_send_values(self.get_instance_id(), ak_aux_send_value, 1)
			
func set_obstruction_and_occlusion() -> void:
	if OS.get_ticks_msec() >= next_occlusion_update:
		next_occlusion_update = OS.get_ticks_msec() + AkUtils.OCCLUSION_DETECTION_INTERVAL
		var current_occlusion = compute_occlusion(listener.get_global_transform(), self.get_global_transform())
		Wwise.set_obj_obstruction_and_occlusion(self.get_instance_id(), listener.get_instance_id(), current_occlusion, current_occlusion)			

func compute_occlusion(listener_transform:Transform, source_transform:Transform) -> float:
# No idea how to compute occlusion and obstruction yet
# Casting a ray from the emitter to the listener, adding AkUtils.OCCLUSION_ADDED to occlusion_value per hit
# Not easy to get an array of hits at one time like Unity's Physics.Raycast.

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

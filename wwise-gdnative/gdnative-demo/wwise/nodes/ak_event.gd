extends AkEventHandler

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

func _init() -> void:
	register_game_object(self, self.name)
	
func _ready() -> void:
	if use_callback:
		connect_signals()

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
	

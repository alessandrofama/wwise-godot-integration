extends Node

export(AK.EVENTS._enum) var event = AK.EVENTS.MUSIC
export(AkUtils.AkCallbackType) var callback_type = AkUtils.AkCallbackType.AK_MusicSyncBeat

func _ready():
	Wwise.connect(AkUtils.Signals.AUDIO_MARKER, self, "_on_audio_marker")
	Wwise.connect(AkUtils.Signals.END_OF_EVENT, self, "_on_end_of_event")
	Wwise.connect(AkUtils.Signals.AUDIO_DURATION, self, "_on_audio_duration")
	Wwise.connect(AkUtils.Signals.SPEAKER_VOLUME_MATRIX, self, "_on_speaker_volume_matrix")
	Wwise.connect(AkUtils.Signals.AUDIO_STARVATION, self, "_on_audio_starvation")
	Wwise.connect(AkUtils.Signals.MUSIC_PLAYLIST_SELECT, self, "_on_music_playlist_select")
	Wwise.connect(AkUtils.Signals.MUSIC_PLAY_STARTED, self, "_on_music_play_started")
	Wwise.connect(AkUtils.Signals.MUSIC_SYNC_BEAT, self, "_on_sync_beat")
	Wwise.connect(AkUtils.Signals.MUSIC_SYNC_BAR, self, "_on_sync_bar")
	Wwise.connect(AkUtils.Signals.MUSIC_SYNC_ENTRY, self, "_on_sync_entry")
	Wwise.connect(AkUtils.Signals.MUSIC_SYNC_EXIT, self, "_on_sync_exit")
	Wwise.connect(AkUtils.Signals.MUSIC_SYNC_GRID, self, "_on_sync_grid")
	Wwise.connect(AkUtils.Signals.MUSIC_SYNC_USER_CUE, self, "_on_sync_user_cue")
	Wwise.connect(AkUtils.Signals.MUSIC_SYNC_ALL, self, "_on_sync_all")
	Wwise.connect(AkUtils.Signals.MIDI_EVENT, self, "_on_midi_event")
	Wwise.connect(AkUtils.Signals.CALLBACK_BITS, self, "_on_callback_bits")
	Wwise.connect(AkUtils.Signals.ENABLE_GET_SOURCE_PLAY_POSITION, self, "_on_enable_get_source_play_position")
	Wwise.connect(AkUtils.Signals.ENABLE_GET_MUSIC_PLAY_POSITION, self, "_on_enable_get_music_play_position")
	Wwise.connect(AkUtils.Signals.ENABLE_GET_SOURCE_STREAM_BUFFERING, self, "_on_get_source_stream_buffering")

	var registerResult = Wwise.register_game_obj(self, "Marker test")
	print("Registering Marker test: ", registerResult)
	
	Wwise.post_event_id_callback(event, callback_type, self)

func _on_end_of_event(data):
	print("End of Event data: ", data)

func _on_audio_marker(data):
	print("Marker data: ", data)

func _on_audio_duration(data):
	print("Audio Duration data: ", data)

func _on_speaker_volume_matrix(data):
	print("Speaker Volume Matrix data: ", data)

func _on_audio_starvation(data):
	print("Audio Starvation data: ", data)
	
func _on_music_playlist_select(data):
	print("Music Playlist Select data: ", data)
	
func _on_music_play_started(data):
	print("Music Play Started data: ", data)
		
func _on_sync_beat(data):
	print("Sync Beat data: ", data)
	
func _on_sync_bar(data):
	print("Sync Bar data: ", data)
	
func _on_sync_entry(data):
	print("Sync Entry data: ", data)
	
func _on_sync_exit(data):
	print("Sync Exit data: ", data)
	
func _on_sync_grid(data):
	print("Sync Grid data: ", data)
	
func _on_sync_user_cue(data):
	print("Sync User Cue data: ", data)
	
func _on_sync_all(data):
	print("Sync All data: ", data)

func _on_midi_event(data):
	print("Midi Event data: ", data.midiEvent.cc.byValue)

func _on_callback_bits(data):
	print("Callback Bits data: ", data)
	
func _on_enable_get_source_play_position(data):
	print("Enable Get Source Play Position data: ", data)
	
func _on_enable_get_music_play_position(data):
	print("Enable Get Music Play Position data: ", data)
	
func _on_get_source_stream_buffering(data):
	print("Get Source Stream Buffering data: ", data)

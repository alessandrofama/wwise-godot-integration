extends Node2D

var signal_prefix:String = "_on_"

export(AK.EVENTS._enum) var event:int = AK.EVENTS._enum.values()[0]
export(AkUtils.GameEvent) var trigger_on:int = AkUtils.GameEvent.NONE
export(AkUtils.GameEvent) var stop_on:int = AkUtils.GameEvent.NONE
export(int) var stop_fade_time:int = 0
export (AkUtils.AkCurveInterpolation) var interpolation_mode:int = AkUtils.AkCurveInterpolation.LINEAR
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

func register_game_object(object:Object, gameObjectName:String):
	Wwise.register_game_obj(object, gameObjectName)

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

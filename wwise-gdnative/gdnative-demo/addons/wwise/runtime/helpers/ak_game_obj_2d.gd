# warning-ignore-all:UNUSED_SIGNAL
extends Node2D

var signal_prefix:String = "_on_"

export(Dictionary) var event:Dictionary = {"Name": "", "Id": 0}
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
signal audio_marker(data)
signal audio_duration(data)
signal speaker_volume_matrix(data)
signal audio_starvation(data)
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

func callback_emitter(data):
	match(data.callbackType):
		AkUtils.AkCallbackType.AK_EndOfEvent:
			emit_signal(AkUtils.Signals.END_OF_EVENT, data)
				
		AkUtils.AkCallbackType.AK_EndOfDynamicSequenceItem:
			emit_signal(AkUtils.Signals.END_OF_DYNAMIC_SEQUENCE_ITEM, data)
			
		AkUtils.AkCallbackType.AK_Marker:
			emit_signal(AkUtils.Signals.AUDIO_MARKER, data)
			
		AkUtils.AkCallbackType.AK_Duration:
			emit_signal(AkUtils.Signals.AUDIO_DURATION, data)
			
		AkUtils.AkCallbackType.AK_SpeakerVolumeMatrix:
			emit_signal(AkUtils.Signals.SPEAKER_VOLUME_MATRIX, data)
			
		AkUtils.AkCallbackType.AK_Starvation:
			emit_signal(AkUtils.Signals.AUDIO_STARVATION, data)
			
		AkUtils.AkCallbackType.AK_MusicPlaylistSelect:
			emit_signal(AkUtils.Signals.MUSIC_PLAYLIST_SELECT, data)
			
		AkUtils.AkCallbackType.AK_MusicPlayStarted:
			emit_signal(AkUtils.Signals.MUSIC_PLAY_STARTED, data)
				
		AkUtils.AkCallbackType.AK_MusicSyncBeat:
			emit_signal(AkUtils.Signals.MUSIC_SYNC_BEAT, data)
				
		AkUtils.AkCallbackType.AK_MusicSyncBar:
			emit_signal(AkUtils.Signals.MUSIC_SYNC_BAR, data)
				
		AkUtils.AkCallbackType.AK_MusicSyncEntry:
			emit_signal(AkUtils.Signals.MUSIC_SYNC_ENTRY, data)
				
		AkUtils.AkCallbackType.AK_MusicSyncExit:
			emit_signal(AkUtils.Signals.MUSIC_SYNC_EXIT, data)
				
		AkUtils.AkCallbackType.AK_MusicSyncGrid:
			emit_signal(AkUtils.Signals.MUSIC_SYNC_GRID, data)
			
		AkUtils.AkCallbackType.AK_MusicSyncUserCue:
			emit_signal(AkUtils.Signals.MUSIC_SYNC_USER_CUE, data)
				
		AkUtils.AkCallbackType.AK_MusicSyncPoint:
			emit_signal(AkUtils.Signals.MUSIC_SYNC_POINT, data)
				
		AkUtils.AkCallbackType.AK_MusicSyncAll:
			emit_signal(AkUtils.Signals.MUSIC_SYNC_ALL, data)
				
		AkUtils.AkCallbackType.AK_MIDIEvent:
			emit_signal(AkUtils.Signals.MIDI_EVENT, data)
			
		AkUtils.AkCallbackType.AK_CallbackBits:
			emit_signal(AkUtils.Signals.CALLBACK_BITS, data)

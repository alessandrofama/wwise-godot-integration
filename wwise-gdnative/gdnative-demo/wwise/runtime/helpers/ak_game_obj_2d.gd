extends Node2D

var signal_prefix:String = "_on_"

func register_game_object(object:Object, gameObjectName:String):
	Wwise.register_game_obj(object, gameObjectName)

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

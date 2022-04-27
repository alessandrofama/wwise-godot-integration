class_name AkUtils

enum AkCurveInterpolation {
	LOG3			= 0,
	SINE			= 1,
	LOG1			= 2,
	INVSCURVE		= 3,
	LINEAR			= 4,
	SCURVE			= 5,
	EXP1			= 6,
	SINERECIP		= 7,
	EXP3			= 8,
	LASTFADECURVE  	= 8,
}

enum AkCallbackType {
	AK_EndOfEvent					= 0x0001,	
	AK_EndOfDynamicSequenceItem		= 0x0002,	
	AK_Marker						= 0x0004,	
	AK_Duration						= 0x0008,	
	AK_SpeakerVolumeMatrix			= 0x0010,   
	AK_Starvation					= 0x0020,	
	AK_MusicPlaylistSelect			= 0x0040,	
	AK_MusicPlayStarted				= 0x0080,	
	AK_MusicSyncBeat				= 0x0100,	
	AK_MusicSyncBar					= 0x0200,	
	AK_MusicSyncEntry				= 0x0400,	
	AK_MusicSyncExit				= 0x0800,	
	AK_MusicSyncGrid				= 0x1000,	
	AK_MusicSyncUserCue				= 0x2000,	
	AK_MusicSyncPoint				= 0x4000,	
	AK_MusicSyncAll					= 0x7f00,	
	AK_MIDIEvent					= 0x10000,	
	AK_CallbackBits					= 0xfffff,	
	AK_EnableGetSourcePlayPosition	= 0x100000,	
	AK_EnableGetMusicPlayPosition	= 0x200000,	
	AK_EnableGetSourceStreamBuffering = 0x400000
}

class Signals:
	const END_OF_EVENT 					= "end_of_event"
	const END_OF_DYNAMIC_SEQUENCE_ITEM 	= "end_of_dynamic_sequence_item"
	const AUDIO_MARKER 					= "audio_marker"
	const AUDIO_DURATION 				= "audio_duration"
	const SPEAKER_VOLUME_MATRIX 		= "speaker_volume_matrix"
	const AUDIO_STARVATION 				= "audio_starvation"
	const MUSIC_PLAYLIST_SELECT 		= "music_playlist_select"
	const MUSIC_PLAY_STARTED 			= "music_play_started"
	const MUSIC_SYNC_BEAT				= "music_sync_beat"
	const MUSIC_SYNC_BAR 				= "music_sync_bar"
	const MUSIC_SYNC_ENTRY 				= "music_sync_entry"
	const MUSIC_SYNC_EXIT 				= "music_sync_exit"
	const MUSIC_SYNC_GRID 				= "music_sync_grid"
	const MUSIC_SYNC_USER_CUE 			= "music_sync_user_cue"
	const MUSIC_SYNC_POINT 				= "music_sync_point"
	const MUSIC_SYNC_ALL 				= "music_sync_all"
	const MIDI_EVENT 					= "midi_event"
	const CALLBACK_BITS 				= "callback_bits"
	const ENABLE_GET_SOURCE_PLAY_POSITION = "enable_get_source_play_position"
	const ENABLE_GET_MUSIC_PLAY_POSITION = "enable_get_music_play_position"
	const ENABLE_GET_SOURCE_STREAM_BUFFERING = "enable_get_source_stream_buffering"
	const BANK_CALLBACK					= "bank_callback"

enum AkCodecID {
	AKCODECID_BANK				= 0,
	AKCODECID_PCM				= 1,
	AKCODECID_ADPCM				= 2,
	AKCODECID_XMA				= 3,
	AKCODECID_VORBIS			= 4,
	AKCODECID_WIIADPCM			= 5,
	AKCODECID_PCMEX				= 7,
	AKCODECID_EXTERNAL_SOURCE	= 8, 
	AKCODECID_XWMA				= 9,
	AKCODECID_AAC				= 10,
	AKCODECID_FILE_PACKAGE		= 11,
	AKCODECID_ATRAC9			= 12,
	AKCODECID_VAG				= 13,
	AKCODECID_PROFILERCAPTURE	= 14,
	AKCODECID_ANALYSISFILE		= 15,
	AKCODECID_MIDI				= 16,
	AKCODECID_OPUSNX			= 17,
	AKCODECID_CAF				= 18,
	AKCODECID_AKOPUS			= 19
}

enum GameEvent{
	NONE,
	TREE_ENTER,
	TREE_EXIT,
	READY,
	AREA_ENTER,
	AREA_EXIT,
	BODY_ENTER,
	BODY_EXIT
	}
	
const OCCLUSION_DETECTION_INTERVAL = 200
const OCCLUSION_ADDEND = 0.25

enum MultiPositionType {
	SingleSource = 0,
	MultiSources = 1,
	MultiDirections = 2
}

enum AkType {
	EVENT,
	BANK,
	RTPC,
	STATE_GROUP,
	STATE,
	SWITCH_GROUP,
	SWITCH,
	BUS,
	AUX_BUS
}

static func create_icon(path: String) -> ImageTexture:
	var image = Image.new()
	var err = image.load(path)
	if err != OK:
		push_error("Failed load custom nodes icon")
	var icon = ImageTexture.new()
	icon.create_from_image(image, 0)
	return icon


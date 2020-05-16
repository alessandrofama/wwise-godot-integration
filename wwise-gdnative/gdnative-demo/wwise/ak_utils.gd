class_name AkUtils

enum AkInterpolationMode {
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

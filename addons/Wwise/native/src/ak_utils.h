#ifndef AK_UTILS_H
#define AK_UTILS_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/templates/hash_map.hpp>

namespace godot
{
class AkUtils : public RefCounted
{
	GDCLASS(AkUtils, RefCounted);

protected:
	static void _bind_methods();

private:
	static AkUtils* singleton;

public:
	enum GameEvent
	{
		GAMEEVENT_NONE,
		GAMEEVENT_ENTER_TREE,
		GAMEEVENT_READY,
		GAMEEVENT_EXIT_TREE
	};

	enum AkType
	{
		AKTYPE_EVENT,
		AKTYPE_BANK,
		AKTYPE_RTPC,
		AKTYPE_STATE_GROUP,
		AKTYPE_STATE,
		AKTYPE_SWITCH_GROUP,
		AKTYPE_SWITCH,
		AKTYPE_BUS,
		AKTYPE_AUX_BUS
	};

	enum AkCurveInterpolation
	{
		AK_CURVE_LOG3 = 0,
		AK_CURVE_SINE = 1,
		AK_CURVE_LOG1 = 2,
		AK_CURVE_INVSCURVE = 3,
		AK_CURVE_LINEAR = 4,
		AK_CURVE_SCURVE = 5,
		AK_CURVE_EXP1 = 6,
		AK_CURVE_SINERECIP = 7,
		AK_CURVE_EXP3 = 8,
		AK_CURVE_LASTFADECURVE = 8,
		AK_CURVE_CONSTANT = 9
	};

	enum AkCallbackType
	{
		AK_END_OF_EVENT = 0x0001,
		AK_END_OF_DYNAMIC_SEQUENCE_ITEM = 0x0002,
		AK_MARKER = 0x0004,
		AK_DURATION = 0x0008,
		AK_SPEAKER_VOLUME_MATRIX = 0x0010,
		AK_STARVATION = 0x0020,
		AK_MUSIC_PLAYLIST_SELECT = 0x0040,
		AK_MUSIC_PLAY_STARTED = 0x0080,
		AK_MUSIC_SYNC_BEAT = 0x0100,
		AK_MUSIC_SYNC_BAR = 0x0200,
		AK_MUSIC_SYNC_ENTRY = 0x0400,
		AK_MUSIC_SYNC_EXIT = 0x0800,
		AK_MUSIC_SYNC_GRID = 0x1000,
		AK_MUSIC_SYNC_USER_CUE = 0x2000,
		AK_MUSIC_SYNC_POINT = 0x4000,
		AK_MUSIC_SYNC_ALL = 0x7F00,
		AK_MIDI_EVENT = 0x10000,
		AK_CALLBACK_BITS = 0xFFFFF,
		AK_ENABLE_GET_SOURCE_PLAY_POSITION = 0x100000,
		AK_ENABLE_GET_MUSIC_PLAY_POSITION = 0x200000,
		AK_ENABLE_GET_SOURCE_STREAM_BUFFERING = 0x400000
	};

	std::unordered_map<AkCallbackType, StringName> event_callback_signals{
		{ AK_END_OF_EVENT, "end_of_event" },
		{ AK_END_OF_DYNAMIC_SEQUENCE_ITEM, "end_of_dynamic_sequence_item" },
		{ AK_MARKER, "audio_marker" },
		{ AK_DURATION, "duration" },
		{ AK_SPEAKER_VOLUME_MATRIX, "speaker_volume_matrix" },
		{ AK_STARVATION, "audio_starvation" },
		{ AK_MUSIC_PLAYLIST_SELECT, "music_playlist_select" },
		{ AK_MUSIC_PLAY_STARTED, "music_play_started" },
		{ AK_MUSIC_SYNC_BEAT, "music_sync_beat" },
		{ AK_MUSIC_SYNC_BAR, "music_sync_bar" },
		{ AK_MUSIC_SYNC_ENTRY, "music_sync_entry" },
		{ AK_MUSIC_SYNC_EXIT, "music_sync_exit" },
		{ AK_MUSIC_SYNC_GRID, "music_sync_grid" },
		{ AK_MUSIC_SYNC_USER_CUE, "music_sync_user_cue" },
		{ AK_MUSIC_SYNC_POINT, "music_sync_point" },
		{ AK_MUSIC_SYNC_ALL, "music_sync_all" },
		{ AK_MIDI_EVENT, "midi_event" },
		{ AK_CALLBACK_BITS, "callback_bits" },
		{ AK_ENABLE_GET_SOURCE_PLAY_POSITION, "enable_get_source_play_position" },
		{ AK_ENABLE_GET_MUSIC_PLAY_POSITION, "enable_get_music_play_position" },
		{ AK_ENABLE_GET_SOURCE_STREAM_BUFFERING, "enable_get_source_stream_buffering" },
	};

public:
	AkUtils();
	~AkUtils();
	static AkUtils* get_singleton();
};
} //namespace godot

VARIANT_ENUM_CAST(AkUtils::GameEvent);
VARIANT_ENUM_CAST(AkUtils::AkType);
VARIANT_ENUM_CAST(AkUtils::AkCurveInterpolation);
VARIANT_ENUM_CAST(AkUtils::AkCallbackType);

#endif
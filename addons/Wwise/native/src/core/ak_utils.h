#pragma once

#include <godot_cpp/classes/object.hpp>
#include <unordered_map>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class AkUtils : public Object
{
	GDCLASS(AkUtils, Object);

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

	enum MultiPositionType
	{
		TYPE_SINGLE_SOURCE = 0,
		TYPE_MULTI_SOURCES = 1,
		TYPE_MULTI_DIRECTIONS = 2
	};

	enum AkCodecID
	{
		AK_CODECID_BANK = 0,
		AK_CODECID_PCM = 1,
		AK_CODECID_ADPCM = 2,
		AK_CODECID_XMA = 3,
		AK_CODECID_VORBIS = 4,
		AK_CODECID_WIIADPCM = 5,
		AK_CODECID_PCMEX = 7,
		AK_CODECID_EXTERNAL_SOURCE = 8,
		AK_CODECID_XWMA = 9,
		AK_CODECID_AAC = 10,
		AK_CODECID_FILE_PACKAGE = 11,
		AK_CODECID_ATRAC9 = 12,
		AK_CODECID_VAG = 13,
		AK_CODECID_PROFILERCAPTURE = 14,
		AK_CODECID_ANALYSISFILE = 15,
		AK_CODECID_MIDI = 16,
		AK_CODECID_OPUSNX = 17,
		AK_CODECID_CAF = 18,
		AK_CODECID_AKOPUS = 19,
		AK_CODECID_AKOPUS_WEM = 20,
		AK_CODECID_MEMORYMGR_DUMP = 21,
		AK_CODECID_SONY360 = 22,
		AK_CODECID_BANK_EVENT = 30,
		AK_CODECID_BANK_BUS = 31
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
		{ AK_ENABLE_GET_SOURCE_STREAM_BUFFERING, "enable_get_source_stream_buffering" }
	};

public:
	AkUtils();
	~AkUtils();
	static AkUtils* get_singleton();
};

VARIANT_ENUM_CAST(AkUtils::GameEvent);
VARIANT_ENUM_CAST(AkUtils::AkCurveInterpolation);
VARIANT_ENUM_CAST(AkUtils::AkCallbackType);
VARIANT_ENUM_CAST(AkUtils::MultiPositionType);
VARIANT_ENUM_CAST(AkUtils::AkCodecID);
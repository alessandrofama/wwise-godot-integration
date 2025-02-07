#pragma once

#include "core/ak_utils.h"
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class WwiseExternalSourceInfo : public RefCounted
{
	GDCLASS(WwiseExternalSourceInfo, RefCounted);

protected:
	static void _bind_methods()
	{
		ClassDB::bind_method(
				D_METHOD("set_external_src_cookie", "cookie"), &WwiseExternalSourceInfo::set_external_src_cookie);
		ClassDB::bind_method(D_METHOD("get_external_src_cookie"), &WwiseExternalSourceInfo::get_external_src_cookie);

		ClassDB::bind_method(D_METHOD("set_id_codec", "id_codec"), &WwiseExternalSourceInfo::set_id_codec);
		ClassDB::bind_method(D_METHOD("get_id_codec"), &WwiseExternalSourceInfo::get_id_codec);

		ClassDB::bind_method(D_METHOD("set_sz_file", "sz_file"), &WwiseExternalSourceInfo::set_sz_file);
		ClassDB::bind_method(D_METHOD("get_sz_file"), &WwiseExternalSourceInfo::get_sz_file);

		ADD_PROPERTY(PropertyInfo(Variant::INT, "external_src_cookie"), "set_external_src_cookie",
				"get_external_src_cookie");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "id_codec", PROPERTY_HINT_ENUM,
							 "BANK,PCM,ADPCM,XMA,VORBIS,WIIADPCM,PCMEX,EXTERNAL_SOURCE,XWMA,AAC,"
							 "FILE_PACKAGE,ATRAC9,VAG,PROFILERCAPTURE,ANALYSISFILE,MIDI,OPUSNX,CAF,"
							 "AKOPUS,AKOPUS_WEM,MEMORYMGR_DUMP,SONY360,BANK_EVENT,BANK_BUS"),
				"set_id_codec", "get_id_codec");
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "sz_file"), "set_sz_file", "get_sz_file");
	}

private:
	uint32_t external_src_cookie{ 0 };
	AkUtils::AkCodecID id_codec{ AkUtils::AkCodecID::AK_CODECID_ADPCM };
	String sz_file;

public:
	void set_external_src_cookie(uint32_t p_cookie) { external_src_cookie = p_cookie; }
	uint32_t get_external_src_cookie() const { return external_src_cookie; }

	void set_id_codec(AkUtils::AkCodecID p_codec_id) { id_codec = p_codec_id; }
	AkUtils::AkCodecID get_id_codec() const { return id_codec; }

	void set_sz_file(const String& p_file) { sz_file = p_file; }
	String get_sz_file() const { return sz_file; }
};
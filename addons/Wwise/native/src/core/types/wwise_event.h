#pragma once

#ifdef TOOLS_ENABLED
#include "editor/wwise_project_database.h"
#include <godot_cpp/classes/editor_file_system.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#endif

#include "core/types/wwise_base_type.h"
#include "core/wwise_gdextension.h"
#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class WwiseEvent : public WwiseBaseType
{
	GDCLASS(WwiseEvent, WwiseBaseType);

protected:
	static void _bind_methods();
	void _notification(int p_what);

private:
	bool is_in_user_defined_sound_bank{};
	uint32_t bank_id{ AK_INVALID_UNIQUE_ID };
	bool is_auto_bank_loaded{};
	uint32_t playing_id{ AK_INVALID_PLAYING_ID };

	void _on_post_resource_init();
	void load_auto_bank();
	void load_auto_bank_blocking();
	void post_load_auto_bank(uint32_t p_bank_id);
	void unload_auto_bank();
	AkBankTypeEnum get_bank_type() const;

public:
	virtual WwiseObjectType get_object_type() const override { return WwiseObjectType::Event; };

	uint32_t post(Node* p_node);
	uint32_t post_callback(Node* p_node, AkUtils::AkCallbackType p_flags, const Callable& cookie);
	void stop(Node* p_node, int p_transition_duration = 0,
			AkUtils::AkCurveInterpolation p_fade_curve = AkUtils::AkCurveInterpolation::AK_CURVE_LINEAR);
	void execute_action(Node* p_node, AkUtils::AkActionOnEventType p_action_on_event_type, int p_transition_duration,
			AkUtils::AkCurveInterpolation p_fade_curve);
	void post_midi(Node* p_node, TypedArray<AkMidiPost> p_midi_posts);
	void stop_midi(Node* p_node);

	void set_is_in_user_defined_sound_bank(bool p_is_in_user_defined_soundbank);
	bool get_is_in_user_defined_sound_bank() const;

	void set_bank_id(uint32_t p_bank_id);
	uint32_t get_bank_id() const;

	void set_is_auto_bank_loaded(bool p_is_auto_bank_loaded);
	bool get_is_auto_bank_loaded() const;

	void set_playing_id(AkPlayingID p_playing_id);
	AkPlayingID get_playing_id() const;

#ifdef TOOLS_ENABLED
	bool update_is_user_defined_soundbank()
	{
		bool changed{};
		if (WwiseProjectDatabase::project_info_exists)
		{
			const WwiseCRefSoundBank* soundbank_ref = GetSoundBankRefString(get_name().utf8().get_data(), "Event");
			bool is_in_user_defined_soundbank = !IsSoundBankValid((void*)soundbank_ref);
			if (get_is_in_user_defined_sound_bank() != is_in_user_defined_soundbank)
			{
				changed = true;
				set_is_in_user_defined_sound_bank(is_in_user_defined_soundbank);
			}
		}

		return changed;
	}
#endif
};
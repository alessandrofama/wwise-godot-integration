#pragma once

#ifdef TOOLS_ENABLED
#include "editor/wwise_project_database.h"
#endif

#include "core/types/wwise_base_type.h"
#include "core/wwise_gdextension.h"

using namespace godot;

class WwiseAuxBus : public WwiseBaseType
{
	GDCLASS(WwiseAuxBus, WwiseBaseType);

protected:
	static void _bind_methods();
	void _notification(int p_what);

private:
	bool is_in_user_defined_sound_bank{};
	uint32_t bank_id{ AK_INVALID_UNIQUE_ID };
	bool is_auto_bank_loaded{};

	void _on_post_resource_init();
	void load_auto_bank();
	void load_auto_bank_blocking();
	void post_load_auto_bank(uint32_t p_bank_id);
	void unload_auto_bank();
	AkBankTypeEnum get_bank_type() const;

public:
	virtual WwiseObjectType get_object_type() const override { return WwiseObjectType::AuxBus; };

	void set_is_in_user_defined_sound_bank(bool p_is_in_user_defined_soundbank);
	bool get_is_in_user_defined_sound_bank() const;

	void set_bank_id(uint32_t p_bank_id);
	uint32_t get_bank_id() const;

	void set_is_auto_bank_loaded(bool p_is_auto_bank_loaded);
	bool get_is_auto_bank_loaded() const;

#ifdef TOOLS_ENABLED
	bool update_is_user_defined_soundbank()
	{
		bool changed{};
		if (WwiseProjectDatabase::project_info_exists)
		{
			const WwiseCRefSoundBank* soundbank_ref = GetSoundBankRefString(get_name().utf8().get_data(), "Bus");
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
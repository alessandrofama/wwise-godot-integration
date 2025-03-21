#include "wwise_aux_bus.h"

void WwiseAuxBus::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_is_in_user_defined_sound_bank", "is_in_user_defined_soundbank"),
			&WwiseAuxBus::set_is_in_user_defined_sound_bank);
	ClassDB::bind_method(
			D_METHOD("get_is_in_user_defined_sound_bank"), &WwiseAuxBus::get_is_in_user_defined_sound_bank);

	ClassDB::bind_method(D_METHOD("set_bank_id", "bank_id"), &WwiseAuxBus::set_bank_id);
	ClassDB::bind_method(D_METHOD("get_bank_id"), &WwiseAuxBus::get_bank_id);

	ClassDB::bind_method(
			D_METHOD("set_is_auto_bank_loaded", "is_auto_bank_loaded"), &WwiseAuxBus::set_is_auto_bank_loaded);
	ClassDB::bind_method(D_METHOD("get_is_auto_bank_loaded"), &WwiseAuxBus::get_is_auto_bank_loaded);

	ClassDB::bind_method(D_METHOD("_on_post_resource_init"), &WwiseAuxBus::_on_post_resource_init);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_in_user_defined_sound_bank"), "set_is_in_user_defined_sound_bank",
			"get_is_in_user_defined_sound_bank");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "bank_id"), "set_bank_id", "get_bank_id");
	ADD_PROPERTY(
			PropertyInfo(Variant::BOOL, "is_auto_bank_loaded"), "set_is_auto_bank_loaded", "get_is_auto_bank_loaded");
}

void WwiseAuxBus::_notification(int p_what)
{
	RETURN_IF_EDITOR;

	switch (p_what)
	{
		case NOTIFICATION_POSTINITIALIZE:
		{
			connect("ws_post_resource_init", callable_mp(this, &WwiseAuxBus::_on_post_resource_init));
			break;
		}
		case NOTIFICATION_PREDELETE:
		{
			disconnect("ws_post_resource_init", callable_mp(this, &WwiseAuxBus::_on_post_resource_init));
			unload_auto_bank();
			break;
		}
		default:
			break;
	}
}

void WwiseAuxBus::_on_post_resource_init()
{
	if (Wwise::get_singleton()->is_initialized())
	{
		load_auto_bank();
	}
}

void WwiseAuxBus::load_auto_bank()
{
	if (get_is_in_user_defined_sound_bank())
	{
		return;
	}

	load_auto_bank_blocking();
}

void WwiseAuxBus::load_auto_bank_blocking()
{
	set_bank_id(AkBankManager::load_bank(get_name(), get_bank_type()));
	post_load_auto_bank(get_bank_id());
	set_is_auto_bank_loaded(true);
}

void WwiseAuxBus::post_load_auto_bank(uint32_t p_bank_id)
{
	if (p_bank_id == AK_INVALID_UNIQUE_ID)
	{
		return;
	}

	const AkUniqueID bus_id = get_id();
	AkUniqueID bus_ids[] = { bus_id };
	auto result = AK::SoundEngine::PrepareBus(AK::SoundEngine::Preparation_Load, bus_ids, 1);
	if (result != AK_Success)
	{
		UtilityFunctions::push_error(vformat(
				"WwiseGodot: PrepareBus for %s failed with result: %s.", get_name(), wwise_error_string(result)));
	}
}

void WwiseAuxBus::unload_auto_bank()
{
	if (get_bank_id() != AK_INVALID_UNIQUE_ID && get_is_auto_bank_loaded())
	{
		const AkUniqueID bus_id = get_id();
		AkUniqueID bus_ids[] = { bus_id };
		AK::SoundEngine::PrepareBus(AK::SoundEngine::Preparation_Unload, bus_ids, 1);
		AkBankManager::unload_bank(get_name());
		set_bank_id(AK_INVALID_UNIQUE_ID);
	}
}

AkBankTypeEnum WwiseAuxBus::get_bank_type() const
{
	switch (get_object_type())
	{
		case WwiseObjectType::AuxBus:
			return AkBankTypeEnum::AkBankType_Bus;
		case WwiseObjectType::Event:
			return AkBankTypeEnum::AkBankType_Event;
		default:
			return AkBankTypeEnum::AkBankType_User;
	}
}

void WwiseAuxBus::set_is_in_user_defined_sound_bank(bool p_is_in_user_defined_soundbank)
{
	is_in_user_defined_sound_bank = p_is_in_user_defined_soundbank;
	emit_changed();
}

bool WwiseAuxBus::get_is_in_user_defined_sound_bank() const { return is_in_user_defined_sound_bank; }

void WwiseAuxBus::set_bank_id(uint32_t p_bank_id)
{
	bank_id = p_bank_id;
	emit_changed();
}

uint32_t WwiseAuxBus::get_bank_id() const { return bank_id; }

void WwiseAuxBus::set_is_auto_bank_loaded(bool p_is_auto_bank_loaded)
{
	is_auto_bank_loaded = p_is_auto_bank_loaded;
	emit_changed();
}

bool WwiseAuxBus::get_is_auto_bank_loaded() const { return is_auto_bank_loaded; }

#include "ak_bank.h"

void AkBank::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("handle_game_event", "game_event"), &AkBank::handle_game_event);
	ClassDB::bind_method(D_METHOD("load_bank"), &AkBank::load_bank);
	ClassDB::bind_method(D_METHOD("unload_bank"), &AkBank::unload_bank);
	ClassDB::bind_method(D_METHOD("set_bank", "bank"), &AkBank::set_bank);
	ClassDB::bind_method(D_METHOD("get_bank"), &AkBank::get_bank);
	ClassDB::bind_method(D_METHOD("set_load_on", "load_on"), &AkBank::set_load_on);
	ClassDB::bind_method(D_METHOD("get_load_on"), &AkBank::get_load_on);
	ClassDB::bind_method(D_METHOD("set_unload_on", "unload_on"), &AkBank::set_unload_on);
	ClassDB::bind_method(D_METHOD("get_unload_on"), &AkBank::get_unload_on);

	ADD_PROPERTY(
			PropertyInfo(Variant::OBJECT, "bank", PROPERTY_HINT_RESOURCE_TYPE, "WwiseBank"), "set_bank", "get_bank");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "load_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_load_on", "get_load_on");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "unload_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_unload_on", "get_unload_on");
}

void AkBank::_enter_tree()
{
	WwiseSettings* settings = WwiseSettings::get_singleton();

	if (settings)
	{
		use_soundbank_names = settings->get_setting(settings->project_settings.use_soundbank_names, true);
	}

	handle_game_event(AkUtils::GameEvent::GAMEEVENT_ENTER_TREE);
}

void AkBank::_ready() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_READY); }

void AkBank::_exit_tree() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_EXIT_TREE); }

void AkBank::handle_game_event(AkUtils::GameEvent game_event)
{
	RETURN_IF_EDITOR;

	if (load_on == game_event)
	{
		load_bank();
	}
	if (unload_on == game_event)
	{
		unload_bank();
	}
}

void AkBank::load_bank()
{
	if (bank.is_null())
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: Trying to load a Bank, but the Bank property in the "
											   "AkBank node: %s is not set (null).",
				get_name()));
		return;
	}

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		if (use_soundbank_names)
		{
			bank->load();
		}
		else
		{
			uint32_t id = bank->get_id();
			soundengine->load_bank_id(id);
		}
	}
}

void AkBank::unload_bank()
{
	if (bank.is_null())
	{
		UtilityFunctions::push_warning(vformat("WwiseGodot: Trying to unload a Bank, but the Bank property in the "
											   "AkBank node: %s is not set (null).",
				get_name()));
		return;
	}

	Wwise* soundengine = Wwise::get_singleton();

	if (soundengine)
	{
		if (use_soundbank_names)
		{
			bank->unload();
		}
		else
		{
			uint32_t id = bank->get_id();
			soundengine->unload_bank_id(id);
		}
	}
}

void AkBank::set_bank(const Ref<WwiseBank>& bank)
{
	this->bank = bank;
	notify_property_list_changed();
}

Ref<WwiseBank> AkBank::get_bank() const { return bank; }

void AkBank::set_load_on(AkUtils::GameEvent load_on) { this->load_on = load_on; }

AkUtils::GameEvent AkBank::get_load_on() const { return load_on; }

void AkBank::set_unload_on(AkUtils::GameEvent unload_on) { this->unload_on = unload_on; }

AkUtils::GameEvent AkBank::get_unload_on() const { return unload_on; }

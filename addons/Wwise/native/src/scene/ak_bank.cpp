#include "ak_bank.h"

void AkBank::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("_on_load_async_completed", "bank_id", "result"), &AkBank::_on_load_async_completed);
	ClassDB::bind_method(
			D_METHOD("_on_unload_async_completed", "bank_id", "result"), &AkBank::_on_unload_async_completed);
	ClassDB::bind_method(D_METHOD("handle_game_event", "game_event"), &AkBank::handle_game_event);
	ClassDB::bind_method(D_METHOD("load_bank"), &AkBank::load_bank);
	ClassDB::bind_method(D_METHOD("unload_bank"), &AkBank::unload_bank);
	ClassDB::bind_method(D_METHOD("set_bank", "bank"), &AkBank::set_bank);
	ClassDB::bind_method(D_METHOD("get_bank"), &AkBank::get_bank);
	ClassDB::bind_method(D_METHOD("set_load_on", "load_on"), &AkBank::set_load_on);
	ClassDB::bind_method(D_METHOD("get_load_on"), &AkBank::get_load_on);
	ClassDB::bind_method(D_METHOD("set_unload_on", "unload_on"), &AkBank::set_unload_on);
	ClassDB::bind_method(D_METHOD("get_unload_on"), &AkBank::get_unload_on);
	ClassDB::bind_method(D_METHOD("set_async_load", "async_load"), &AkBank::set_async_load);
	ClassDB::bind_method(D_METHOD("get_async_load"), &AkBank::get_async_load);

	ADD_PROPERTY(
			PropertyInfo(Variant::OBJECT, "bank", PROPERTY_HINT_RESOURCE_TYPE, "WwiseBank"), "set_bank", "get_bank");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "load_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_load_on", "get_load_on");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "unload_on", PROPERTY_HINT_ENUM, "None,Enter Tree,Ready,Exit Tree"),
			"set_unload_on", "get_unload_on");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "async_load", PROPERTY_HINT_NONE), "set_async_load", "get_async_load");
	{
		MethodInfo mi;
		mi.name = "bank_loaded";
		mi.arguments.push_back(PropertyInfo(Variant::INT, "bank_id"));
		mi.arguments.push_back(PropertyInfo(Variant::INT, "result"));
		ClassDB::add_signal("AkBank", mi);
	}

	{
		MethodInfo mi;
		mi.name = "bank_unloaded";
		mi.arguments.push_back(PropertyInfo(Variant::INT, "bank_id"));
		mi.arguments.push_back(PropertyInfo(Variant::INT, "result"));
		ClassDB::add_signal("AkBank", mi);
	}
}

void AkBank::_on_load_async_completed(int p_bank_id, int p_result) { emit_signal("bank_loaded", p_bank_id, p_result); }

void AkBank::_on_unload_async_completed(int p_bank_id, int p_result)
{
	emit_signal("bank_unloaded", p_bank_id, p_result);
}

void AkBank::_enter_tree() { handle_game_event(AkUtils::GameEvent::GAMEEVENT_ENTER_TREE); }

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
		WwiseLogger::warning_format("Trying to load a Bank, but the Bank property in the "
									"AkBank node: %s is not set (null).",
				get_name());
		return;
	}

	if (async_load)
	{
		bank->load_async(Callable(this, StringName("_on_load_async_completed")));
	}
	else
	{
		bank->load();
	}
}

void AkBank::unload_bank()
{
	if (bank.is_null())
	{
		WwiseLogger::warning_format("Trying to unload a Bank, but the Bank property in the "
									"AkBank node: %s is not set (null).",
				get_name());
		return;
	}

	if (async_load)
	{
		bank->unload_async(Callable(this, StringName("_on_unload_async_completed")));
	}
	else
	{
		bank->unload();
	}
}

void AkBank::set_bank(const Ref<WwiseBank>& bank)
{
	this->bank = bank;
	notify_property_list_changed();
}

Ref<WwiseBank> AkBank::get_bank() const { return bank; }

void AkBank::set_load_on(AkUtils::GameEvent p_load_on) { load_on = p_load_on; }

AkUtils::GameEvent AkBank::get_load_on() const { return load_on; }

void AkBank::set_unload_on(AkUtils::GameEvent p_unload_on) { unload_on = p_unload_on; }

AkUtils::GameEvent AkBank::get_unload_on() const { return unload_on; }

void AkBank::set_async_load(bool p_async_load) { async_load = p_async_load; }

bool AkBank::get_async_load() const { return async_load; }
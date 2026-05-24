#include "wwise_bank.h"

void WwiseBank::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("load"), &WwiseBank::load);
	ClassDB::bind_method(D_METHOD("load_async", "load_callback"), &WwiseBank::load_async);
	ClassDB::bind_method(D_METHOD("unload"), &WwiseBank::unload);
	ClassDB::bind_method(D_METHOD("unload_async", "unload_callback"), &WwiseBank::unload_async);
}

void WwiseBank::load() { AkBankManager::load_bank(get_name()); }

void WwiseBank::load_async(const Callable& p_load_callback)
{
	AkBankManager::load_bank_async(get_name(), p_load_callback);
}

void WwiseBank::unload() { AkBankManager::unload_bank(get_name()); }

void WwiseBank::unload_async(const Callable& p_unload_callback)
{
	AkBankManager::unload_bank_async(get_name(), p_unload_callback);
}
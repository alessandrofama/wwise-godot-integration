#include "wwise_bank.h"

void WwiseBank::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("load"), &WwiseBank::load);
	ClassDB::bind_method(D_METHOD("unload"), &WwiseBank::unload);
}

void WwiseBank::load() { AkBankManager::load_bank(get_name()); }

void WwiseBank::unload() { AkBankManager::unload_bank(get_name()); }
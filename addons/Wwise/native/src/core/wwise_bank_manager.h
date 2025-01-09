#pragma once

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkAutoLock.h>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/templates/vector.hpp>

using namespace godot;

class AkBankManager
{
public:
	struct BankHandle
	{
		String bank_name;
		uint32_t bank_id{};
		AkBankTypeEnum bank_type;
		int ref_count = {};

		BankHandle(const String& p_name, AkBankTypeEnum p_type) : bank_name(p_name), bank_type(p_type) {}

		void inc_ref()
		{
			if (ref_count == 0)
			{
				banks_to_unload.erase(this);
			}
			ref_count++;
		}

		void dec_ref()
		{
			ref_count--;
			if (ref_count == 0)
			{
				banks_to_unload.push_back(this);
			}
		}

		uint32_t load_bank()
		{
			if (ref_count == 0 && !banks_to_unload.erase(this))
			{
				auto result = AK::SoundEngine::LoadBank(bank_name.utf8().get_data(), bank_id, bank_type);
				log_load_result(result);
			}

			inc_ref();
			return bank_id;
		}

		void unload_bank(bool remove = true)
		{
			AK::SoundEngine::UnloadBank(bank_id, nullptr, bank_type);

			if (remove)
			{
				AkAutoLock<CAkLock> scoped_lock(bank_handles_lock);
				auto find = bank_handles.find(bank_name);
				if (find != bank_handles.end())
				{
					bank_handles.remove(find);
				}
			}
		}

		void log_load_result(AKRESULT result)
		{
			if (result != AK_Success)
			{
				UtilityFunctions::push_warning(
						vformat("WwiseGodot: Bank %s failed to load (%s).", bank_name, String::num_int64(result)));
			}
		}
	};

	static HashMap<String, BankHandle*> bank_handles;
	static Vector<BankHandle*> banks_to_unload;
	static CAkLock bank_handles_lock;

	static void do_unload_banks()
	{
		for (auto* bank : banks_to_unload)
		{
			bank->unload_bank();
			delete bank;
		}
		banks_to_unload.clear();
	}

	static void reset()
	{
		AkAutoLock<CAkLock> scoped_lock(bank_handles_lock);
		for (auto& entry : bank_handles)
		{
			delete entry.value;
		}
		for (auto* bank : banks_to_unload)
		{
			delete bank;
		}
		bank_handles.clear();
		banks_to_unload.clear();
	}

	static void load_init_bank(bool do_reset = true)
	{
		if (do_reset)
		{
			reset();
		}

		AkBankID bank_id;
		AKRESULT result = AK::SoundEngine::LoadBank("Init.bnk", bank_id);
		if (result != AK_Success)
		{
			UtilityFunctions::push_warning(
					vformat("WwiseGodot: Failed to load Init.bnk with result: %s", wwise_error_string(result)));
		}
	}

	static void load_init_bank_id(bool do_reset = true)
	{
		if (do_reset)
		{
			reset();
		}

		AkUInt32 init_bank_id = AK::SoundEngine::GetIDFromString("Init");
		AKRESULT result = AK::SoundEngine::LoadBank(init_bank_id);
		if (result != AK_Success)
		{
			UtilityFunctions::push_warning(
					vformat("WwiseGodot: Failed to load Init.bnk with result: %s", wwise_error_string(result)));
		}
	}

	static void unload_init_bank() { AK::SoundEngine::UnloadBank("Init.bnk", nullptr); }

	static uint32_t load_bank(const String& name, AkBankTypeEnum bankType = AkBankTypeEnum::AkBankType_User)
	{
		BankHandle* handle{ nullptr };
		{
			AkAutoLock<CAkLock> scoped_lock(bank_handles_lock);
			if (bank_handles.has(name))
			{
				handle = bank_handles[name];
				handle->inc_ref();
				return AK_INVALID_UNIQUE_ID;
			}

			handle = new BankHandle(name, bankType);
			bank_handles.insert(name, handle);
		}

		return handle->load_bank();
	}

	static void unload_bank(const String& name)
	{
		AkAutoLock<CAkLock> scoped_lock(bank_handles_lock);

		if (bank_handles.has(name))
		{
			bank_handles[name]->dec_ref();
		}
	}
};

CAkLock AkBankManager::bank_handles_lock;
HashMap<String, AkBankManager::BankHandle*> AkBankManager::bank_handles;
Vector<AkBankManager::BankHandle*> AkBankManager::banks_to_unload;
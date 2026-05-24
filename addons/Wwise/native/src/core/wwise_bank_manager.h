#pragma once

#include "core/utils.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkAutoLock.h>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/callable.hpp>

#include <mutex>
#include <unordered_map>
#include <vector>

using namespace godot;

class AkBankManager
{
public:
	enum class BankState
	{
		UNLOADED,
		LOADING,
		LOADED,
		FAILED,
		UNLOADING,
		READY_TO_DELETE
	};

private:
	struct BankEntry
	{
		String bank_name;
		AkBankID bank_id{ AK_INVALID_BANK_ID };
		AkBankTypeEnum bank_type{ AkBankTypeEnum::AkBankType_User };

		int ref_count{ 0 };
		BankState state{ BankState::UNLOADED };

		std::vector<Callable> load_callbacks;
		Callable unload_callback;

		bool wants_async_unload{ false };

		BankEntry(const String& p_name, AkBankID p_id, AkBankTypeEnum p_type) :
				bank_name(p_name), bank_id(p_id), bank_type(p_type)
		{
		}
	};

	enum class CommandType
	{
		LOAD_FINISHED,
		UNLOAD_FINISHED
	};

	struct AsyncCommand
	{
		CommandType type;
		BankEntry* entry_ptr;
		AKRESULT result;
	};

	static inline std::unordered_map<AkBankID, BankEntry*> active_banks;
	static inline std::vector<BankEntry*> unload_queue;

	static inline std::vector<AsyncCommand> result_queue;
	static inline CAkLock result_queue_mutex;

	static String bake_state_to_string(BankState state)
	{
		switch (state)
		{
			case BankState::UNLOADED:
				return "BankState::UNLOADED";
			case BankState::LOADING:
				return "BankState::LOADING";
			case BankState::LOADED:
				return "BankState::LOADED";
			case BankState::FAILED:
				return "BankState::FAILED";
			case BankState::UNLOADING:
				return "BankState::UNLOADING";
			case BankState::READY_TO_DELETE:
				return "BankState::READY_TO_DELETE";
			default:
				return "UNKNOWN_STATE";
		}
	}

	static void _wwise_load_callback(
			AkUInt32 in_bankID, const void* in_pInMemoryBankPtr, AKRESULT in_eLoadResult, void* in_Cookie)
	{
		WwiseLogger::very_verbose_format("[AkBankManager] Load callback fired: bank %s result %s", in_bankID,
				wwise_error_string(in_eLoadResult));
		BankEntry* entry = static_cast<BankEntry*>(in_Cookie);
		AkAutoLock lock(result_queue_mutex);
		result_queue.push_back({ CommandType::LOAD_FINISHED, entry, in_eLoadResult });
	}

	static void _wwise_unload_callback(
			AkUInt32 in_bankID, const void* in_pInMemoryBankPtr, AKRESULT in_eUnloadResult, void* in_Cookie)
	{
		WwiseLogger::very_verbose_format("[AkBankManager] Unload callback fired: bank %s result %s", in_bankID,
				wwise_error_string(in_eUnloadResult));
		BankEntry* entry = static_cast<BankEntry*>(in_Cookie);
		AkAutoLock lock(result_queue_mutex);
		result_queue.push_back({ CommandType::UNLOAD_FINISHED, entry, in_eUnloadResult });
	}

	static BankEntry* _get_or_create_entry(const String& p_name, AkBankID p_id, AkBankTypeEnum p_type, bool& out_is_new)
	{
		auto it = active_banks.find(p_id);

		if (it != active_banks.end())
		{
			BankEntry* entry = it->second;

			if (entry->state == BankState::UNLOADING || entry->state == BankState::READY_TO_DELETE)
			{
				WwiseLogger::very_verbose_format("[AkBankManager] Bank %s is unloading. Spawning new entry.", p_name);
				active_banks.erase(it);
			}
			else
			{
				WwiseLogger::very_verbose_format("[AkBankManager] Found existing entry for bank %s", p_name);
				out_is_new = false;
				return entry;
			}
		}

		WwiseLogger::very_verbose_format("[AkBankManager] Creating new entry for bank %s", p_name);
		BankEntry* new_entry = new BankEntry(p_name, p_id, p_type);
		active_banks[p_id] = new_entry;
		out_is_new = true;
		return new_entry;
	}

	static void _queue_for_unload(BankEntry* entry)
	{
		WwiseLogger::very_verbose_format("[AkBankManager] Queuing bank %s for unload", entry->bank_name);
		unload_queue.push_back(entry);
	}

public:
	static void init() { reset(); }

	static void load_init_bank(bool do_reset = true)
	{
		if (do_reset)
		{
			reset();
		}

		WwiseLogger::very_verbose("[AkBankManager] Loading Init.bnk...");
		AkBankID bank_id;
		AKRESULT result = AK::SoundEngine::LoadBank("Init.bnk", bank_id);
		if (result != AK_Success)
		{
			WwiseLogger::warning_format("Failed to load Init.bnk with result: %s", wwise_error_string(result));
		}
		else
		{
			WwiseLogger::very_verbose("[AkBankManager] Init.bnk loaded successfully.");
		}
	}

	static uint32_t load_bank(const String& p_name, AkBankTypeEnum p_type = AkBankType_User)
	{
		WwiseLogger::very_verbose_format("[AkBankManager] load_bank called for %s", p_name);
		AkBankID bank_id = AK::SoundEngine::GetIDFromString(p_name.utf8().get_data());
		bool is_new = false;
		BankEntry* entry = _get_or_create_entry(p_name, bank_id, p_type, is_new);

		entry->ref_count++;
		WwiseLogger::very_verbose_format("[AkBankManager] Ref count for %s is now %d", p_name, entry->ref_count);

		if (is_new)
		{
			WwiseLogger::very_verbose_format("[AkBankManager] Synchronous load starting for %s", p_name);
			entry->state = BankState::LOADING;
			AKRESULT res = AK::SoundEngine::LoadBank(p_name.utf8().get_data(), entry->bank_id, p_type);

			if (res == AK_Success || res == AK_BankAlreadyLoaded)
			{
				WwiseLogger::very_verbose_format("[AkBankManager] Sync load succeeded for %s", p_name);
				entry->state = BankState::LOADED;
			}
			else
			{
				WwiseLogger::very_verbose_format("[AkBankManager] Sync load FAILED for %s", p_name);
				entry->state = BankState::FAILED;
				active_banks.erase(bank_id);
				delete entry;
				return AK_INVALID_BANK_ID;
			}
		}
		else
		{
			if (entry->state == BankState::LOADING)
			{
				WwiseLogger::very_verbose_format(
						"[AkBankManager] Cannot synchronously load bank %s while it is asynchronously loading.",
						p_name);
				entry->ref_count--;
				return AK_INVALID_BANK_ID;
			}
		}

		return entry->bank_id;
	}

	static uint32_t load_bank_async(
			const String& p_name, const Callable& p_callback, AkBankTypeEnum p_type = AkBankType_User)
	{
		WwiseLogger::very_verbose_format("[AkBankManager] load_bank_async called for %s", p_name);
		AkBankID bank_id = AK::SoundEngine::GetIDFromString(p_name.utf8().get_data());
		bool is_new = false;
		BankEntry* entry = _get_or_create_entry(p_name, bank_id, p_type, is_new);

		entry->ref_count++;
		WwiseLogger::very_verbose_format("[AkBankManager] Ref count for %s is now %d", p_name, entry->ref_count);

		if (p_callback.is_valid())
		{
			if (entry->state == BankState::LOADED)
			{
				WwiseLogger::very_verbose_format(
						"[AkBankManager] Bank %s already LOADED. Firing callback immediately.", p_name);
				p_callback.call_deferred(entry->bank_id, AK_BankAlreadyLoaded);
			}
			else if (entry->state == BankState::FAILED)
			{
				WwiseLogger::very_verbose_format(
						"[AkBankManager] Bank %s previously FAILED. Firing callback immediately.", p_name);
				p_callback.call_deferred(entry->bank_id, AK_Fail);
			}
			else
			{
				WwiseLogger::very_verbose_format("[AkBankManager] Registering async load callback for %s", p_name);
				entry->load_callbacks.push_back(p_callback);
			}
		}
		else
		{
			WwiseLogger::very_verbose_format("[AkBankManager] No valid callback provided for async load of %s", p_name);
		}

		if (is_new)
		{
			WwiseLogger::very_verbose_format("[AkBankManager] Scheduling async load for %s", p_name);
			entry->state = BankState::LOADING;
			AKRESULT res = AK::SoundEngine::LoadBank(
					p_name.utf8().get_data(), &_wwise_load_callback, entry, entry->bank_id, p_type);

			if (res != AK_Success)
			{
				WwiseLogger::very_verbose_format(
						"[AkBankManager] Async load scheduling FAILED for %s (result=%d)", p_name, res);
				entry->state = BankState::FAILED;

				for (const Callable& cb : entry->load_callbacks)
				{
					if (cb.is_valid())
						cb.call_deferred(entry->bank_id, (int)res);
				}
				entry->load_callbacks.clear();
			}
		}

		return entry->bank_id;
	}

	static void unload_bank(const String& p_name)
	{
		WwiseLogger::very_verbose_format("[AkBankManager] unload_bank called for %s", p_name);
		AkBankID bank_id = AK::SoundEngine::GetIDFromString(p_name.utf8().get_data());

		auto it = active_banks.find(bank_id);

		if (it != active_banks.end())
		{
			BankEntry* entry = it->second;
			entry->wants_async_unload = false;

			int prev = entry->ref_count--;
			WwiseLogger::very_verbose_format("[AkBankManager] Ref count for %s is now %d", p_name, prev - 1);

			if (prev == 1)
			{
				_queue_for_unload(entry);
			}
		}
		else
		{
			WwiseLogger::very_verbose_format(
					"[AkBankManager] Ignored unload_bank for %s: Not found in active_banks.", p_name);
		}
	}

	static void unload_bank_async(const String& p_name, const Callable& p_callback)
	{
		WwiseLogger::very_verbose_format("[AkBankManager] unload_bank_async called for %s", p_name);
		AkBankID bank_id = AK::SoundEngine::GetIDFromString(p_name.utf8().get_data());

		auto it = active_banks.find(bank_id);

		if (it != active_banks.end())
		{
			BankEntry* entry = it->second;
			entry->wants_async_unload = true;

			if (p_callback.is_valid())
			{
				WwiseLogger::very_verbose_format("[AkBankManager] Storing async unload callback for %s", p_name);
				entry->unload_callback = p_callback;
			}

			int prev = entry->ref_count--;
			WwiseLogger::very_verbose_format("[AkBankManager] Ref count for %s is now %d", p_name, prev - 1);

			if (prev == 1)
			{
				_queue_for_unload(entry);
			}
		}
		else
		{
			WwiseLogger::very_verbose_format(
					"[AkBankManager] Ignored async unload for %s: Not found in active_banks. Firing success callback.",
					p_name);

			if (p_callback.is_valid())
				p_callback.call_deferred(bank_id, AK_Success);
		}
	}

	static void update()
	{
		_process_async_results();
		_process_unload_queue();
	}

	static void reset()
	{
		WwiseLogger::very_verbose_format(
				"[AkBankManager] RESET called - moving %d active banks to unload queue", (int)active_banks.size());

		for (auto& pair : active_banks)
		{
			BankEntry* entry = pair.second;
			entry->ref_count = 0;
			unload_queue.push_back(entry);
		}
		active_banks.clear();
	}

private:
	static void _process_async_results()
	{
		std::vector<AsyncCommand> commands;
		{
			AkAutoLock lock(result_queue_mutex);
			if (result_queue.empty())
				return;
			commands.swap(result_queue);
		}

		WwiseLogger::very_verbose_format("[AkBankManager] Processing %d async results", (int)commands.size());

		for (const auto& cmd : commands)
		{
			BankEntry* entry = cmd.entry_ptr;

			if (cmd.type == CommandType::LOAD_FINISHED)
			{
				WwiseLogger::very_verbose_format(
						"[AkBankManager] Async LOAD COMPLETE for %s result=%d", entry->bank_name, cmd.result);

				if (cmd.result == AK_Success || cmd.result == AK_BankAlreadyLoaded)
					entry->state = BankState::LOADED;
				else
					entry->state = BankState::FAILED;

				for (const Callable& cb : entry->load_callbacks)
				{
					if (cb.is_valid())
						cb.call(entry->bank_id, (int)cmd.result);
				}
				entry->load_callbacks.clear();
			}
			else if (cmd.type == CommandType::UNLOAD_FINISHED)
			{
				WwiseLogger::very_verbose_format(
						"[AkBankManager] Async UNLOAD COMPLETE for %s result=%d", entry->bank_name, cmd.result);

				if (entry->unload_callback.is_valid())
					entry->unload_callback.call(entry->bank_id, (int)cmd.result);

				entry->state = BankState::READY_TO_DELETE;
			}
		}
	}

	static void _process_unload_queue()
	{
		for (int i = unload_queue.size() - 1; i >= 0; i--)
		{
			BankEntry* entry = unload_queue[i];

			if (entry->ref_count > 0 && entry->state != BankState::UNLOADING &&
					entry->state != BankState::READY_TO_DELETE)
			{
				WwiseLogger::very_verbose_format(
						"[AkBankManager] Bank %s resurrected from unload queue!", entry->bank_name);
				unload_queue[i] = unload_queue.back();
				unload_queue.pop_back();
				continue;
			}

			WwiseLogger::very_verbose_format("[AkBankManager] Checking unload queue entry: %s (state=%s)",
					entry->bank_name, bake_state_to_string(entry->state));

			if (entry->state == BankState::LOADING)
				continue;
			if (entry->state == BankState::UNLOADING)
				continue;

			if (entry->state == BankState::READY_TO_DELETE)
			{
				WwiseLogger::very_verbose_format("[AkBankManager] Deleting bank entry %s", entry->bank_name);

				auto it = active_banks.find(entry->bank_id);
				if (it != active_banks.end() && it->second == entry)
				{
					active_banks.erase(it);
				}

				delete entry;
				unload_queue[i] = unload_queue.back();
				unload_queue.pop_back();
				continue;
			}

			bool is_async = entry->wants_async_unload;

			if (is_async)
			{
				WwiseLogger::very_verbose_format("[AkBankManager] Starting async unload for %s", entry->bank_name);
				entry->state = BankState::UNLOADING;

				AKRESULT res = AK::SoundEngine::UnloadBank(
						entry->bank_name.utf8().get_data(), nullptr, &_wwise_unload_callback, entry, entry->bank_type);

				if (res != AK_Success)
				{
					WwiseLogger::warning_format("[AkBankManager] Async unload rejected immediately for %s (result=%d). "
												"Manual cleanup triggered.",
							entry->bank_name, res);

					AkAutoLock lock(result_queue_mutex);
					result_queue.push_back({ CommandType::UNLOAD_FINISHED, entry, res });
				}
			}
			else
			{
				WwiseLogger::very_verbose_format("[AkBankManager] Starting sync unload for %s", entry->bank_name);
				AK::SoundEngine::UnloadBank(entry->bank_name.utf8().get_data(), nullptr, entry->bank_type);

				auto it = active_banks.find(entry->bank_id);
				if (it != active_banks.end() && it->second == entry)
				{
					active_banks.erase(it);
				}

				delete entry;
				unload_queue[i] = unload_queue.back();
				unload_queue.pop_back();
			}
		}
	}
};
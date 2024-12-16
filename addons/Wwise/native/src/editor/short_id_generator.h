#pragma once

#include <cstdint>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

class ShortIDGenerator
{
private:
	static constexpr uint32_t s_prime32 = 16777619;
	static constexpr uint32_t s_offsetBasis32 = 2166136261;
	static uint8_t s_hashSize;
	static uint32_t s_mask;

public:
	static uint32_t compute(const String& in_name)
	{
		String lower_name = in_name.to_lower();
		PackedByteArray buffer = lower_name.to_utf8_buffer();

		uint32_t hval = s_offsetBasis32;

		for (int64_t i = 0; i < buffer.size(); i++)
		{
			hval *= s_prime32;
			hval ^= buffer[i];
		}

		if (s_hashSize == 32)
		{
			return hval;
		}

		return (hval >> s_hashSize) ^ (hval & s_mask);
	}

	static uint8_t get_hash_size() { return s_hashSize; }

	static void set_hash_size(uint8_t new_size)
	{
		s_hashSize = new_size;
		s_mask = (1 << s_hashSize) - 1;
	}
};

uint8_t ShortIDGenerator::s_hashSize = 32;
uint32_t ShortIDGenerator::s_mask = (1 << s_hashSize) - 1;
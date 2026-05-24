#if defined(AK_EMSCRIPTEN)
#include <emscripten/emscripten.h>
extern "C"
{
	GDE_EXPORT uint32_t emscripten_atomic_load_u32(const void* addr)
	{
		return __atomic_load_n((const volatile uint32_t*)addr, __ATOMIC_SEQ_CST);
	}

	GDE_EXPORT uint64_t emscripten_atomic_load_u64(const void* addr)
	{
		return __atomic_load_n((const volatile uint64_t*)addr, __ATOMIC_SEQ_CST);
	}

	GDE_EXPORT uint32_t emscripten_atomic_exchange_u32(void* addr, uint32_t value)
	{
		return __atomic_exchange_n((volatile uint32_t*)addr, value, __ATOMIC_SEQ_CST);
	}

	GDE_EXPORT uint64_t emscripten_atomic_exchange_u64(void* addr, uint64_t value)
	{
		return __atomic_exchange_n((volatile uint64_t*)addr, value, __ATOMIC_SEQ_CST);
	}

	GDE_EXPORT uint32_t emscripten_atomic_cas_u32(void* addr, uint32_t expected, uint32_t desired)
	{
		__atomic_compare_exchange_n(
				(volatile uint32_t*)addr, &expected, desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

		return expected;
	}

	GDE_EXPORT uint64_t emscripten_atomic_cas_u64(void* addr, uint64_t expected, uint64_t desired)
	{
		__atomic_compare_exchange_n(
				(volatile uint64_t*)addr, &expected, desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

		return expected;
	}

	GDE_EXPORT uint32_t emscripten_atomic_add_u32(void* addr, uint32_t value)
	{
		return __atomic_fetch_add((volatile uint32_t*)addr, value, __ATOMIC_SEQ_CST);
	}

	GDE_EXPORT uint64_t emscripten_atomic_add_u64(void* addr, uint64_t value)
	{
		return __atomic_fetch_add((volatile uint64_t*)addr, value, __ATOMIC_SEQ_CST);
	}

	GDE_EXPORT uint32_t emscripten_atomic_sub_u32(void* addr, uint32_t value)
	{
		return __atomic_fetch_sub((volatile uint32_t*)addr, value, __ATOMIC_SEQ_CST);
	}

	GDE_EXPORT uint64_t emscripten_atomic_sub_u64(void* addr, uint64_t value)
	{
		return __atomic_fetch_sub((volatile uint64_t*)addr, value, __ATOMIC_SEQ_CST);
	}

	GDE_EXPORT uint32_t emscripten_atomic_and_u32(void* addr, uint32_t value)
	{
		return __atomic_fetch_and((volatile uint32_t*)addr, value, __ATOMIC_SEQ_CST);
	}

	GDE_EXPORT uint32_t emscripten_atomic_or_u32(void* addr, uint32_t value)
	{
		return __atomic_fetch_or((volatile uint32_t*)addr, value, __ATOMIC_SEQ_CST);
	}

	GDE_EXPORT void emscripten_atomic_fence() { __atomic_thread_fence(__ATOMIC_SEQ_CST); }
}
#endif
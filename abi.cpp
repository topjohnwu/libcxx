// Minimal C++ ABI & STL runtime stub for Magisk (built with -fno-exceptions -fno-rtti)

#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <atomic>
#include <new>
#include <exception>
#include <string>
#include <vector>
#include <mutex>
#include <cstdio>

namespace std {
const nothrow_t nothrow{};
void __throw_bad_alloc() { ::abort(); }
}

// -------------------------------------------------------------
// Global operator new and delete implementations
// -------------------------------------------------------------

#define WEAK __attribute__((weak))

WEAK void* operator new(std::size_t s) { void* p = std::malloc(s ? s : 1); if (!p) std::__throw_bad_alloc(); return p; }
WEAK void* operator new[](std::size_t s) { return ::operator new(s); }
WEAK void* operator new(std::size_t s, const std::nothrow_t&) noexcept { return std::malloc(s ? s : 1); }
WEAK void* operator new[](std::size_t s, const std::nothrow_t&) noexcept { return ::operator new(s, std::nothrow); }
WEAK void operator delete(void* p) noexcept { std::free(p); }
WEAK void operator delete[](void* p) noexcept { ::operator delete(p); }
WEAK void operator delete(void* p, std::size_t) noexcept { ::operator delete(p); }
WEAK void operator delete[](void* p, std::size_t) noexcept { ::operator delete[](p); }
WEAK void operator delete(void* p, const std::nothrow_t&) noexcept { ::operator delete(p); }
WEAK void operator delete[](void* p, const std::nothrow_t&) noexcept { ::operator delete[](p); }

#undef WEAK

// -------------------------------------------------------------
// Standard Library Helpers & Instantiations
// -------------------------------------------------------------

_LIBCPP_BEGIN_NAMESPACE_STD

[[noreturn]] void terminate() noexcept { ::abort(); }
[[noreturn]] void __throw_runtime_error(const char*) { ::abort(); }
[[noreturn]] void __throw_system_error(int, const char*) { ::abort(); }
void __libcpp_verbose_abort(const char*, ...) _NOEXCEPT { ::abort(); }

#if !_LIBCPP_HAS_TRIVIAL_MUTEX_DESTRUCTION
mutex::~mutex() _NOEXCEPT { __libcpp_mutex_destroy(&__m_); }
#endif

void mutex::lock() { if (__libcpp_mutex_lock(&__m_)) ::abort(); }
bool mutex::try_lock() _NOEXCEPT { return __libcpp_mutex_trylock(&__m_) == 0; }
void mutex::unlock() _NOEXCEPT { __libcpp_mutex_unlock(&__m_); }

#ifndef _LIBCPP_ABI_DO_NOT_EXPORT_VECTOR_BASE_COMMON
template <bool> struct __vector_base_common;
template <> struct __vector_base_common<true> {
    [[noreturn]] void __throw_length_error() const { ::abort(); }
    [[noreturn]] void __throw_out_of_range() const { ::abort(); }
};
#endif

#ifndef _LIBCPP_ABI_DO_NOT_EXPORT_BASIC_STRING_COMMON
template <bool> struct __basic_string_common;
template <> struct __basic_string_common<true> {
    [[noreturn]] void __throw_length_error() const { ::abort(); }
    [[noreturn]] void __throw_out_of_range() const { ::abort(); }
};
#endif

#define _LIBCPP_EXTERN_TEMPLATE_DEFINE(...) template __VA_ARGS__;
_LIBCPP_STRING_V1_EXTERN_TEMPLATE_LIST(_LIBCPP_EXTERN_TEMPLATE_DEFINE, char)
#undef _LIBCPP_EXTERN_TEMPLATE_DEFINE

template string operator+ <char, char_traits<char>, allocator<char>>(char const*, string const&);

template <typename T>
static string int_to_string(const char* fmt, T val) {
    char buf[32];
    int len = ::snprintf(buf, sizeof(buf), fmt, val);
    return len > 0 ? string(buf, static_cast<size_t>(len)) : string();
}

string to_string(int val) { return int_to_string("%d", val); }
string to_string(long val) { return int_to_string("%ld", val); }
string to_string(long long val) { return int_to_string("%lld", val); }
string to_string(unsigned val) { return int_to_string("%u", val); }
string to_string(unsigned long val) { return int_to_string("%lu", val); }
string to_string(unsigned long long val) { return int_to_string("%llu", val); }
string to_string(float val) { return int_to_string("%f", val); }
string to_string(double val) { return int_to_string("%f", val); }
string to_string(long double val) { return int_to_string("%Lf", val); }

_LIBCPP_END_NAMESPACE_STD

// -------------------------------------------------------------
// Itanium C++ ABI runtime stubs
// -------------------------------------------------------------

#if defined(__arm__)
using guard_type = uint32_t;
#else
using guard_type = uint64_t;
#endif

extern "C" {

int __cxa_guard_acquire(guard_type* raw_guard) {
    auto* guard = reinterpret_cast<std::atomic<uint8_t>*>(raw_guard);
    if (guard->load(std::memory_order_acquire) == 1) return 0;
    uint8_t expected = 0;
    if (guard->compare_exchange_strong(expected, 2, std::memory_order_acquire)) return 1;
    while (guard->load(std::memory_order_acquire) != 1) {}
    return 0;
}

void __cxa_guard_release(guard_type* raw_guard) {
    reinterpret_cast<std::atomic<uint8_t>*>(raw_guard)->store(1, std::memory_order_release);
}

void __cxa_guard_abort(guard_type* raw_guard) {
    reinterpret_cast<std::atomic<uint8_t>*>(raw_guard)->store(0, std::memory_order_release);
}

void __cxa_pure_virtual() { ::abort(); }
void __cxa_deleted_virtual() { ::abort(); }

typedef void (*dtor_func)(void*);
int __cxa_thread_atexit_impl(dtor_func, void*, void*) __attribute__((weak));

int __cxa_thread_atexit(dtor_func dtor, void* obj, void* dso_symbol) {
    return __cxa_thread_atexit_impl ? __cxa_thread_atexit_impl(dtor, obj, dso_symbol) : 0;
}

} // extern "C"

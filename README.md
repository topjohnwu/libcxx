## libc++

LLVM libc++, specifically for Android, removing exception and RTTI support.

Source code is extracted from both the AOSP `llvm-project` repository.

An opt-in [C++23 `std` module subset](modules/README.md) is available for the
project's ndk-build module extension. It uses the same minimal runtime described
below and does not enable the unsupported subsystems.

### Minimal Header-Only Architecture

Magisk uses a minimal build setup for libc++ to keep native binary sizes small and compilation fast:

- **Header-Only STL:** Standard template features (`std::vector`, `std::map`, `std::span`, `std::unique_ptr`, `std::atomic`, `std::function`, etc.) are included header-only from `include/`.
- **`abi.cpp`:** A single 100% self-contained ABI source file that replaces `libc++abi` and `libc++` runtime libraries, providing:
  - Weak global `operator new` and `operator delete` overloads
  - The `std::nothrow` allocation tag
  - Itanium C++ ABI runtime stubs (`__cxa_guard_*`, `__cxa_pure_virtual`, `__cxa_thread_atexit`)
  - Core STL abort/throw helpers (`std::terminate()`, `std::__throw_runtime_error()`, `std::__throw_system_error()`, etc.)
  - Explicit template instantiations for `std::basic_string<char>` methods (required by libc++'s `extern template` declarations) and string conversion functions (`std::to_string`)
- **Exported Flags:** `Android.mk` automatically exports `-fno-exceptions` and `-fno-rtti` to all dependent modules via `LOCAL_EXPORT_CFLAGS` and `LOCAL_EXPORT_CPPFLAGS`.

### Subsystem Limitations

Because out-of-line C++ runtime library code is omitted in favor of `abi.cpp`, standard C++ library subsystems requiring compiled runtime support are not available:

- **`<filesystem>`:** Path manipulation, directory iterators, and status operations.
- **`<regex>`:** Regular expression parsing, searching, and collate facets.
- **Threading & Concurrency (`<thread>`, `<future>`, `<condition_variable>`, `<barrier>`, `<latch>`, `<semaphore>`, `<stop_token>`):** Thread creation, async futures, and condition variable notifications. *(Note: primitive `std::mutex` is supported via `abi.cpp`).*
- **Locales & Encoding (`<locale>`, `<codecvt>`):** Global locales, formatting facets, and character conversions.
- **`<valarray>`, `<any>`:** Non-template helpers and type info routines.

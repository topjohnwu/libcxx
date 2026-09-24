# Magisk's `std` module subset

This directory supplies **a subset of the C++23 `std` module**, using the same
headers and minimal `abi.cpp` runtime as Magisk's existing libc++ build. It is
not a complete implementation of the standard library module.

The module exports common C++ containers, strings and string views, algorithms,
ranges, iterators, concepts, type traits, utilities, smart pointer declarations,
`optional`, `expected`, `variant`, atomics, and mutexes. `std.cppm` lists the exact
header/export-fragment set. Existing runtime limitations still apply: importing
a declaration does not supply its missing out-of-line implementation. For
example, the default-comparator integral `std::sort`/`std::ranges::sort` path
requires runtime specializations absent from `abi.cpp`; use a custom comparator
to select the header-defined implementation.

C library function exports (`std::printf`, `std::isalnum`, etc.), streams,
localization, filesystem, regex, thread creation and futures are omitted.
`std.compat` and header units are not provided. C headers can still be included
normally. Like other named modules, `import std` does not import macros.

Unmodified upstream `std.cppm` fails on Android because Bionic exposes some C
functions with internal linkage, including FORTIFY overloads. See
[Android NDK issue 2119](https://github.com/android/ndk/issues/2119).
This subset does not disable `_FORTIFY_SOURCE`, alter Bionic linkage macros,
or modify NDK headers.

## Use with ndk-build

Load the project's `scripts/ndk-modules/init.mk`, libcxx's `Android.mk`, and this
directory's `Android.mk`. Consumers declare the normal static-library dependency
and enable module compilation:

```makefile
include $(CLEAR_VARS)
LOCAL_MODULE := example
LOCAL_SRC_FILES := main.cpp
LOCAL_STATIC_LIBRARIES := cxx_std
include $(BUILD_EXECUTABLE_MODULE)
```

`main.cpp` can use `import std;`. The `cxx_std` target exports the BMI mapping and
the libcxx dependency, declaring `std.cppm` in `LOCAL_MODULE_SRC_FILES` as a BMI
provider. Consumers stay in `LOCAL_SRC_FILES`. The module scanner supplies
compilation ordering and transitive import mappings. Exporting a compiler flag
alone does not establish that ordering for an ordinary `BUILD_EXECUTABLE` consumer.

Select C++23 or newer in `APP_CPPFLAGS`. The provider and consumers must use
compatible language/ABI flags; a single BMI cannot serve different C++ standards.
Each ABI and ndk-build configuration builds its own BMI. `abi.cpp` remains a
separate target with its existing C++26 flags.

This target is opt-in through the `cxx_std` dependency.

## Source provenance

`std/*.inc` are copied verbatim from the libc++ 21.0.0 module sources bundled with
ONDK r30.1 (`share/libc++/v1/std/`). They match this repository's NDK r30 libc++
headers. Each file retains its upstream Apache-2.0 WITH LLVM-exception notice;
the full license is in `../LICENSE.TXT`. `std.cppm` selects only the subset above.
Revalidate the export fragments when updating libc++.

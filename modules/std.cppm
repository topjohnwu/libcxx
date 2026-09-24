// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Magisk libc++ subset. Keep the export fragments in sync with include/.
// C library exports and subsystems requiring the full runtime are omitted.

module;

#include <__config>
#if _LIBCPP_VERSION != 210000
#  error Update and validate the std module exports for this libc++ version
#endif
#if __cplusplus < 202302L
#  error The std module requires C++23 or newer
#endif

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <expected>
#include <forward_list>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <numbers>
#include <numeric>
#include <optional>
#include <queue>
#include <ranges>
#include <ratio>
#include <set>
#include <span>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

export module std;

#include "std/algorithm.inc"
#include "std/array.inc"
#include "std/atomic.inc"
#include "std/bit.inc"
#include "std/compare.inc"
#include "std/concepts.inc"
#include "std/cstddef.inc"
#include "std/cstdint.inc"
#include "std/deque.inc"
#include "std/expected.inc"
#include "std/forward_list.inc"
#include "std/functional.inc"
#include "std/initializer_list.inc"
#include "std/iterator.inc"
#include "std/limits.inc"
#include "std/list.inc"
#include "std/map.inc"
#include "std/memory.inc"
#include "std/mutex.inc"
#include "std/new.inc"
#include "std/numbers.inc"
#include "std/numeric.inc"
#include "std/optional.inc"
#include "std/queue.inc"
#include "std/ranges.inc"
#include "std/ratio.inc"
#include "std/set.inc"
#include "std/span.inc"
#include "std/stack.inc"
#include "std/string.inc"
#include "std/string_view.inc"
#include "std/tuple.inc"
#include "std/type_traits.inc"
#include "std/unordered_map.inc"
#include "std/unordered_set.inc"
#include "std/utility.inc"
#include "std/variant.inc"
#include "std/vector.inc"

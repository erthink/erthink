/*
 *  Copyright (c) 1994-2024 Leonid Yuriev <leo@yuriev.ru>.
 *  https://gitflic.ru/project/erthink/erthink
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

#if !defined(__cplusplus) || __cplusplus < 201103L
#error "This source code requires C++11 at least."
#endif

#include "erthink_defs.h"
#include "erthink_dynamic_constexpr.h++"

#include <cstring> // for std::strlen, std::memcmp, std::memcpy

#if defined(__cpp_lib_string_view) && __cpp_lib_string_view >= 201606L
#include <string_view>
#endif

namespace erthink {

#if defined(__LCC__) && __LCC__ >= 126
#pragma diagnostic push
#if __LCC__ < 127
#pragma diag_suppress 3058 /* workaround: call to is_constant_evaluated()      \
                              appearing in a constant expression `true` */
#pragma diag_suppress 3060 /* workaround: call to is_constant_evaluated()      \
                              appearing in a constant expression `false` */
#pragma diag_suppress 2416 /* constexpr function return is non-constant */
#endif
#endif /* E2K LCC (warnings) */

static cxx14_constexpr size_t strlen_constexpr(const char *c_str) noexcept {
  for (size_t i = 0; c_str; ++i)
    if (!c_str[i])
      return i;
  return 0;
}

static inline size_t strlen_dynamic(const char *c_str) noexcept {
  return c_str ? ::std::strlen(c_str) : 0;
}

ERTHINK_DYNAMIC_CONSTEXPR(size_t, strlen, (const char *c_str), (c_str), c_str)

static cxx14_constexpr void *memcpy_constexpr(void *dest, const void *src,
                                              size_t bytes) noexcept {
  for (size_t i = 0; i < bytes; ++i)
    static_cast<char *>(dest)[i] = static_cast<const char *>(src)[i];
  return dest;
}

static inline void *memcpy_dynamic(void *dest, const void *src,
                                   size_t bytes) noexcept {
  return ::std::memcpy(dest, src, bytes);
}

ERTHINK_DYNAMIC_CONSTEXPR(void *, memcpy,
                          (void *dest, const void *src, size_t bytes),
                          (dest, src, bytes),
                          (static_cast<char *>(dest) +
                           static_cast<const char *>(src)[0] + bytes))

static cxx14_constexpr int memcmp_constexpr(const void *a, const void *b,
                                            size_t bytes) noexcept {
  for (size_t i = 0; i < bytes; ++i) {
    const int diff = static_cast<const unsigned char *>(a)[i] -
                     static_cast<const unsigned char *>(b)[i];
    if (diff)
      return diff;
  }
  return 0;
}

static inline int memcmp_dynamic(const void *a, const void *b,
                                 size_t bytes) noexcept {
  return ::std::memcmp(a, b, bytes);
}

ERTHINK_DYNAMIC_CONSTEXPR(
    int, memcmp, (const void *a, const void *b, size_t bytes), (a, b, bytes),
    (static_cast<const char *>(a)[0] + static_cast<const char *>(b)[0] + bytes))

#if defined(__LCC__) && __LCC__ >= 126
#pragma diagnostic pop
#endif

} // namespace erthink

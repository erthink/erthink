/*
 *  Copyright (c) 1994-2022 Leonid Yuriev <leo@yuriev.ru>.
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

#if !(defined(__cplusplus) && __cplusplus >= 201103L)
#error "This source code requires C++11 at least."
#endif

#include <cstdint>
#include <type_traits>

#include "erthink_arch.h"
#include "erthink_byteorder.h"
#include "erthink_casting.h++" // for erthink::enable_if_t stub
#include "erthink_defs.h"
#include "erthink_intrin.h"

namespace erthink {

template <typename T,
          typename = typename erthink::enable_if_t<std::is_unsigned<T>::value>>
inline uint_least8_t add_carry(const T base, const T addend, T &sum) noexcept {
#if __GNUC_PREREQ(5, 0) || __has_builtin(__builtin_add_overflow)
  return __builtin_add_overflow(base, addend, &sum);
#else
  sum = base + addend;
  return sum < addend;
#endif
}

template <typename T,
          typename = typename erthink::enable_if_t<std::is_unsigned<T>::value>>
inline uint_least8_t add_carry(const T base, const T addend, T &sum,
                               const uint_least8_t carry_in) noexcept {
  T midterm;
  const auto carry_out = add_carry(addend, T(carry_in), midterm);
  return add_carry<T>(base, midterm, sum) | carry_out;
}

template <typename T,
          typename = typename erthink::enable_if_t<std::is_unsigned<T>::value>>
inline uint_least8_t addcarry_first(const T &base, const T &addend,
                                    T &sum) noexcept {
  return add_carry<T>(base, addend, sum);
}

template <typename T,
          typename = typename erthink::enable_if_t<std::is_unsigned<T>::value>>
inline uint_least8_t addcarry_next(const uint_least8_t carry_in, const T &base,
                                   const T &addend, T &sum) noexcept {
  return add_carry<T>(base, addend, sum, carry_in);
}

template <typename T,
          typename = typename erthink::enable_if_t<std::is_unsigned<T>::value>>
inline void addcarry_last(const uint_least8_t carry_in, const T &base,
                          const T &addend, T &sum) noexcept {
  add_carry<T>(base, addend, sum, carry_in);
}

// ----------------------------------------------------------------------------

template <typename T,
          typename = typename erthink::enable_if_t<std::is_unsigned<T>::value>>
inline uint_least8_t sub_borrow(const T base, const T subtrahend,
                                T &diff) noexcept {
#if __GNUC_PREREQ(5, 0) || __has_builtin(__builtin_sub_overflow)
  return __builtin_sub_overflow(base, subtrahend, &diff);
#else
  diff = base - subtrahend;
  return base < subtrahend;
#endif
}

template <typename T,
          typename = typename erthink::enable_if_t<std::is_unsigned<T>::value>>
inline uint_least8_t sub_borrow(const T base, const T subtrahend, T &diff,
                                const uint_least8_t borrow_in) noexcept {
  T midterm;
  const auto borrow_out = add_carry(subtrahend, T(borrow_in), midterm);
  diff = base - subtrahend;
  return sub_borrow<T>(base, midterm, diff) | borrow_out;
}

template <typename T,
          typename = typename erthink::enable_if_t<std::is_unsigned<T>::value>>
inline uint_least8_t subborrow_first(const T &base, const T &subtrahend,
                                     T &diff) noexcept {
  return sub_borrow<T>(base, subtrahend, diff);
}

template <typename T,
          typename = typename erthink::enable_if_t<std::is_unsigned<T>::value>>
inline uint_least8_t subborrow_next(const uint_least8_t borrow_in,
                                    const T &base, const T &subtrahend,
                                    T &diff) noexcept {
  return sub_borrow<T>(base, subtrahend, diff, borrow_in);
}

template <typename T,
          typename = typename erthink::enable_if_t<std::is_unsigned<T>::value>>
inline void subborrow_last(const uint_least8_t borrow_in, const T &base,
                           const T &subtrahend, T &diff) noexcept {
  sub_borrow<T>(base, subtrahend, diff, borrow_in);
}

// ----------------------------------------------------------------------------

#if __has_builtin(__builtin_addcll)
template <>
inline uint_least8_t
add_carry<unsigned long long>(const unsigned long long base,
                              const unsigned long long addend,
                              unsigned long long &sum) noexcept {
  unsigned long long carry_out;
  sum = __builtin_addcll(base, addend, 0, &carry_out);
  return uint_least8_t(carry_out);
}

template <>
inline uint_least8_t add_carry<unsigned long long>(
    const unsigned long long base, const unsigned long long addend,
    unsigned long long &sum, uint_least8_t carry_in) noexcept {
  unsigned long long carry_out;
  sum = __builtin_addcll(base, addend, carry_in, &carry_out);
  return uint_least8_t(carry_out);
}
#endif /* __builtin_addcll */

#if __has_builtin(__builtin_addcl)
template <>
inline uint_least8_t add_carry<unsigned long>(const unsigned long base,
                                              const unsigned long addend,
                                              unsigned long &sum) noexcept {
  unsigned long carry_out;
  sum = __builtin_addcl(base, addend, 0, &carry_out);
  return uint_least8_t(carry_out);
}

template <>
inline uint_least8_t
add_carry<unsigned long>(const unsigned long base, const unsigned long addend,
                         unsigned long &sum, uint_least8_t carry_in) noexcept {
  unsigned long carry_out;
  sum = __builtin_addcl(base, addend, carry_in, &carry_out);
  return uint_least8_t(carry_out);
}
#endif /* __builtin_addcl */

#if __has_builtin(__builtin_addc)
template <>
inline uint_least8_t add_carry<unsigned>(const unsigned base,
                                         const unsigned addend,
                                         unsigned &sum) noexcept {
  unsigned carry_out;
  sum = __builtin_addc(base, addend, 0, &carry_out);
  return uint_least8_t(carry_out);
}

template <>
inline uint_least8_t add_carry<unsigned>(const unsigned base,
                                         const unsigned addend, unsigned &sum,
                                         uint_least8_t carry_in) noexcept {
  unsigned carry_out;
  sum = __builtin_addc(base, addend, carry_in, &carry_out);
  return uint_least8_t(carry_out);
}
#endif /* __builtin_addc */

// ----------------------------------------------------------------------------

#if __has_builtin(__builtin_subcll)
template <>
inline uint_least8_t
sub_borrow<unsigned long long>(const unsigned long long base,
                               const unsigned long long subtrahend,
                               unsigned long long &diff) noexcept {
  unsigned long long borrow_out;
  diff = __builtin_subcll(base, subtrahend, 0, &borrow_out);
  return uint_least8_t(borrow_out);
}

template <>
inline uint_least8_t sub_borrow<unsigned long long>(
    const unsigned long long base, const unsigned long long subtrahend,
    unsigned long long &diff, uint_least8_t borrow_in) noexcept {
  unsigned long long borrow_out;
  diff = __builtin_subcll(base, subtrahend, borrow_in, &borrow_out);
  return uint_least8_t(borrow_out);
}
#endif /* __builtin_subcll */

#if __has_builtin(__builtin_subcl)
template <>
inline uint_least8_t sub_borrow<unsigned long>(const unsigned long base,
                                               const unsigned long subtrahend,
                                               unsigned long &diff) noexcept {
  unsigned long borrow_out;
  diff = __builtin_subcl(base, subtrahend, 0, &borrow_out);
  return uint_least8_t(borrow_out);
}

template <>
inline uint_least8_t
sub_borrow<unsigned long>(const unsigned long base,
                          const unsigned long subtrahend, unsigned long &diff,
                          uint_least8_t borrow_in) noexcept {
  unsigned long borrow_out;
  diff = __builtin_subcl(base, subtrahend, borrow_in, &borrow_out);
  return uint_least8_t(borrow_out);
}
#endif /* __builtin_subcl */

#if __has_builtin(__builtin_subc)
template <>
inline uint_least8_t sub_borrow<unsigned>(const unsigned base,
                                          const unsigned subtrahend,
                                          unsigned &diff) noexcept {
  unsigned borrow_out;
  diff = __builtin_subc(base, subtrahend, 0, &borrow_out);
  return uint_least8_t(borrow_out);
}

template <>
inline uint_least8_t
sub_borrow<unsigned>(const unsigned base, const unsigned subtrahend,
                     unsigned &diff, uint_least8_t borrow_in) noexcept {
  unsigned borrow_out;
  diff = __builtin_subc(base, subtrahend, borrow_in, &borrow_out);
  return uint_least8_t(borrow_out);
}
#endif /* __builtin_subc */

// ----------------------------------------------------------------------------

#if defined(__e2k__) && __iset__ >= 5
template <>
inline uint_least8_t add_carry<uint64_t>(const uint64_t base,
                                         const uint64_t addend,
                                         uint64_t &sum) noexcept {
  sum = base + addend;
  return __builtin_e2k_addcd_c(base, addend, 0);
}

template <>
inline uint_least8_t add_carry<uint64_t>(const uint64_t base,
                                         const uint64_t addend, uint64_t &sum,
                                         uint_least8_t carry_in) noexcept {
  sum = __builtin_e2k_addcd(base, addend, carry_in);
  return __builtin_e2k_addcd_c(base, addend, carry_in);
}

template <>
inline uint_least8_t sub_borrow<uint64_t>(const uint64_t base,
                                          const uint64_t subtrahend,
                                          uint64_t &diff) noexcept {
  diff = base - subtrahend;
  return __builtin_e2k_subcd_c(base, subtrahend, 0);
}

template <>
inline uint_least8_t
sub_borrow<uint64_t>(const uint64_t base, const uint64_t subtrahend,
                     uint64_t &diff, uint_least8_t borrow_in) noexcept {
  diff = __builtin_e2k_subcd(base, subtrahend, borrow_in);
  return __builtin_e2k_subcd_c(base, subtrahend, borrow_in);
}
#endif /* __e2k__ && __iset__ >= 5 */

// ----------------------------------------------------------------------------

#if !defined(__clang__) && (defined(_M_X64) || defined(_M_AMD64))
#pragma intrinsic(_addcarry_u64)
template <>
inline uint_least8_t add_carry<uint64_t>(const uint64_t base,
                                         const uint64_t addend,
                                         uint64_t &sum) noexcept {
  return _addcarry_u64(0, base, addend, &sum);
}

template <>
inline uint_least8_t add_carry<uint64_t>(const uint64_t base,
                                         const uint64_t addend, uint64_t &sum,
                                         uint_least8_t carry_in) noexcept {
  return _addcarry_u64(carry_in, base, addend, &sum);
}

#pragma intrinsic(_subborrow_u64)
template <>
inline uint_least8_t sub_borrow<uint64_t>(const uint64_t base,
                                          const uint64_t subtrahend,
                                          uint64_t &diff) noexcept {
  return _subborrow_u64(0, base, subtrahend, &diff);
}

template <>
inline uint_least8_t
sub_borrow<uint64_t>(const uint64_t base, const uint64_t subtrahend,
                     uint64_t &diff, uint_least8_t borrow_in) noexcept {
  return _subborrow_u64(borrow_in, base, subtrahend, &diff);
}
#endif /* MSVC AMD64 */

#if !defined(__clang__) &&                                                     \
    ((defined(_M_IX86) &&                                                      \
      _MSC_VER >= 1915 /* LY: workaround for SSA-optimizer bug */) ||          \
     defined(_M_X64) || defined(_M_AMD64))
#pragma intrinsic(_addcarry_u32)
template <>
inline uint_least8_t add_carry<uint32_t>(const uint32_t base,
                                         const uint32_t addend,
                                         uint32_t &sum) noexcept {
  return _addcarry_u32(0, base, addend, &sum);
}

template <>
inline uint_least8_t add_carry<uint32_t>(const uint32_t base,
                                         const uint32_t addend, uint32_t &sum,
                                         uint_least8_t carry_in) noexcept {
  return _addcarry_u32(carry_in, base, addend, &sum);
}

#pragma intrinsic(_subborrow_u32)
template <>
inline uint_least8_t sub_borrow<uint32_t>(const uint32_t base,
                                          const uint32_t subtrahend,
                                          uint32_t &diff) noexcept {
  return _subborrow_u32(0, base, subtrahend, &diff);
}

template <>
inline uint_least8_t
sub_borrow<uint32_t>(const uint32_t base, const uint32_t subtrahend,
                     uint32_t &diff, uint_least8_t borrow_in) noexcept {
  return _subborrow_u32(borrow_in, base, subtrahend, &diff);
}

#endif /* MSVC x86 */

//------------------------------------------------------------------------------

#if ERTHINK_ARCH_BITS < 64 &&                                                  \
    !(__GNUC_PREREQ(5, 0) || __has_builtin(__builtin_add_overflow) ||          \
      __builtin_addcll)
template <>
inline uint_least8_t add_carry<uint64_t>(const uint64_t base,
                                         const uint64_t addend,
                                         uint64_t &sum) noexcept {
  uint32_t *const sum32_ptr = constexpr_pointer_cast<uint32_t *>(&sum);
  const uint32_t base_32l = uint32_t(base);
  const uint32_t base_32h = uint32_t(base >> 32);
  const uint32_t addend_32l = uint32_t(addend);
  const uint32_t addend_32h = uint32_t(addend >> 32);
  return add_carry<uint32_t>(
      base_32h, addend_32h,
      sum32_ptr[__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__],
      add_carry<uint32_t>(
          base_32l, addend_32l,
          sum32_ptr[__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__]));
}

template <>
inline uint_least8_t add_carry<uint64_t>(const uint64_t base,
                                         const uint64_t addend, uint64_t &sum,
                                         uint_least8_t carry_in) noexcept {
  uint32_t *const sum32_ptr = constexpr_pointer_cast<uint32_t *>(&sum);
  const uint32_t base_32l = uint32_t(base);
  const uint32_t base_32h = uint32_t(base >> 32);
  const uint32_t addend_32l = uint32_t(addend);
  const uint32_t addend_32h = uint32_t(addend >> 32);
  return add_carry<uint32_t>(
      base_32h, addend_32h,
      sum32_ptr[__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__],
      add_carry<uint32_t>(base_32l, addend_32l,
                          sum32_ptr[__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__],
                          carry_in));
}
#endif /* ERTHINK_ARCH_BITS < 64 */

#if ERTHINK_ARCH_BITS < 64 &&                                                  \
    !(__GNUC_PREREQ(5, 0) || __has_builtin(__builtin_sub_overflow) ||          \
      __builtin_subcll)
template <>
inline uint_least8_t sub_borrow<uint64_t>(const uint64_t base,
                                          const uint64_t subtrahend,
                                          uint64_t &diff) noexcept {
  uint32_t *const diff32_ptr = constexpr_pointer_cast<uint32_t *>(&diff);
  const uint32_t base_32l = uint32_t(base);
  const uint32_t base_32h = uint32_t(base >> 32);
  const uint32_t subtrahend_32l = uint32_t(subtrahend);
  const uint32_t subtrahend_32h = uint32_t(subtrahend >> 32);
  return sub_borrow<uint32_t>(
      base_32h, subtrahend_32h,
      diff32_ptr[__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__],
      sub_borrow<uint32_t>(
          base_32l, subtrahend_32l,
          diff32_ptr[__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__]));
}

template <>
inline uint_least8_t
sub_borrow<uint64_t>(const uint64_t base, const uint64_t subtrahend,
                     uint64_t &diff, uint_least8_t borrow_in) noexcept {
  uint32_t *const diff32_ptr = constexpr_pointer_cast<uint32_t *>(&diff);
  const uint32_t base_32l = uint32_t(base);
  const uint32_t base_32h = uint32_t(base >> 32);
  const uint32_t subtrahend_32l = uint32_t(subtrahend);
  const uint32_t subtrahend_32h = uint32_t(subtrahend >> 32);
  return sub_borrow<uint32_t>(
      base_32h, subtrahend_32h,
      diff32_ptr[__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__],
      sub_borrow<uint32_t>(
          base_32l, subtrahend_32l,
          diff32_ptr[__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__], borrow_in));
}
#endif /* ERTHINK_ARCH_BITS < 64 */

//------------------------------------------------------------------------------

inline uint_least8_t add64carry_first(const uint64_t &base,
                                      const uint64_t &addend,
                                      uint64_t *sum) noexcept {
  return addcarry_first<uint64_t>(base, addend, *sum);
}

inline uint_least8_t add64carry_next(const uint_least8_t carry_in,
                                     const uint64_t &base,
                                     const uint64_t &addend,
                                     uint64_t *sum) noexcept {
  return addcarry_next<uint64_t>(carry_in, base, addend, *sum);
}

inline void add64carry_last(const uint_least8_t carry_in, const uint64_t &base,
                            const uint64_t &addend, uint64_t *sum) noexcept {
  return addcarry_last<uint64_t>(carry_in, base, addend, *sum);
}

inline uint_least8_t sub64borrow_first(const uint64_t &base,
                                       const uint64_t &subtrahend,
                                       uint64_t *diff) noexcept {
  return subborrow_first<uint64_t>(base, subtrahend, *diff);
}

inline uint_least8_t sub64borrow_next(const uint_least8_t borrow_in,
                                      const uint64_t &base,
                                      const uint64_t &subtrahend,
                                      uint64_t *diff) noexcept {
  return subborrow_next<uint64_t>(borrow_in, base, subtrahend, *diff);
}

inline void sub64borrow_last(const uint_least8_t borrow_in,
                             const uint64_t &base, const uint64_t &subtrahend,
                             uint64_t *diff) noexcept {
  return subborrow_last<uint64_t>(borrow_in, base, subtrahend, *diff);
}

} // namespace erthink

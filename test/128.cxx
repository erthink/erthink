/*
 *  Copyright (c) 1994-2022 Leonid Yuriev <leo@yuriev.ru>.
 *  https://github.com/erthink/erthink
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

#define ERTHINK_USE_NATIVE_128 0

#include "testing.h++"

#include "erthink.h++"
#include <algorithm> // for std::random_shuffle
#include <cfloat>    // for DBL_MAX
#include <numeric>   // for std::iota
#include <sstream>

#ifndef DO_GENERATE_BASIC
#include "autogen_u128_basic.h++"
#endif /* !DO_GENERATE_BASIC */

//------------------------------------------------------------------------------

TEST(u128, to_string) {
  ASSERT_EQ(std::to_string(std::numeric_limits<erthink::uint128_t>::max()),
            "340282366920938463463374607431768211455");
  ASSERT_EQ(std::to_string(std::numeric_limits<erthink::uint128_t>::min()),
            "0");
  ASSERT_EQ(std::to_string(erthink::uint128_t(142)), "142");
  ASSERT_EQ(std::to_string(erthink::uint128_t(42), 16), "2a");
  ASSERT_EQ(std::to_string(erthink::uint128_t(57), 8), "71");
  ASSERT_EQ(std::to_string(erthink::uint128_t(UINT64_C(0x6E10784D412040D),
                                              UINT64_C(0xFF39F12CF4081907))),
            "9143787268497110792970552074639513863");
  ASSERT_EQ(std::to_string(erthink::uint128_t(~UINT64_C(0))),
            "18446744073709551615");
  ASSERT_EQ(std::to_string(erthink::uint128_t(~UINT64_C(0), 0)),
            "340282366920938463444927863358058659840");
  ASSERT_EQ(std::to_string(erthink::uint128_t(UINT64_C(0x90770897eb39d46c), 0)),
            "192026889014721788266898567285392277504");
  ASSERT_EQ(std::to_string(erthink::uint128_t(UINT64_C(0xb0ec5848ef24a556))),
            "12748661715452077398");
}

TEST(i128, to_string) {
  ASSERT_EQ(std::to_string(std::numeric_limits<erthink::int128_t>::max()),
            "170141183460469231731687303715884105727");
  ASSERT_EQ(std::to_string(std::numeric_limits<erthink::int128_t>::min()),
            "-170141183460469231731687303715884105728");
  ASSERT_EQ(std::to_string(erthink::int128_t(142)), "142");
  ASSERT_EQ(std::to_string(erthink::int128_t(-142)), "-142");
  ASSERT_EQ(std::to_string(erthink::int128_t(42), 16), "2a");
  ASSERT_EQ(std::to_string(erthink::int128_t(-42), 16), "-2a");
  ASSERT_EQ(std::to_string(erthink::int128_t(57), 8), "71");
  ASSERT_EQ(std::to_string(erthink::int128_t(-57), 8), "-71");

  ASSERT_EQ(std::to_string(erthink::int128_t(INT64_C(0x3E121E588E3EAFD3),
                                             UINT64_C(0x4C51BF2CD35A9402))),
            "82506212570507476930068008648333497346");
  ASSERT_EQ(std::to_string(erthink::int128_t(0, ~UINT64_C(0))),
            "18446744073709551615");
  ASSERT_EQ(std::to_string(erthink::int128_t(-1, 0)), "-18446744073709551616");
  ASSERT_EQ(std::to_string(erthink::int128_t(UINT64_C(0xD6AA68591879D977), 0)),
            "-54942768927535355213284596552279523328");
  ASSERT_EQ(std::to_string(erthink::int128_t(-INT64_C(0x4E2D64F136AB6CEB))),
            "-5633269696077458667");
}

TEST(u128, from_string) {
  using erthink::operator"" _u128;
  ASSERT_EQ(340282366920938463463374607431768211455_u128,
            std::numeric_limits<erthink::uint128_t>::max());
  ASSERT_EQ(0_u128, std::numeric_limits<erthink::uint128_t>::min());
  ASSERT_EQ(142_u128, erthink::uint128_t(142));
  ASSERT_EQ(0x2A_u128, erthink::uint128_t(42));
  ASSERT_EQ(071_u128, erthink::uint128_t(57));
  ASSERT_EQ(9143787268497110792970552074639513863_u128,
            erthink::uint128_t(UINT64_C(0x6E10784D412040D),
                               UINT64_C(0xFF39F12CF4081907)));
  ASSERT_EQ(18446744073709551615_u128, erthink::uint128_t(~UINT64_C(0)));
  ASSERT_EQ(340282366920938463444927863358058659840_u128,
            erthink::uint128_t(~UINT64_C(0), 0));
  ASSERT_EQ(192026889014721788266898567285392277504_u128,
            erthink::uint128_t(UINT64_C(0x90770897eb39d46c), 0));
  ASSERT_EQ(12748661715452077398_u128,
            erthink::uint128_t(UINT64_C(0xb0ec5848ef24a556)));
}

TEST(i128, from_string) {
  using erthink::operator"" _i128;
  ASSERT_EQ(170141183460469231731687303715884105727_i128,
            std::numeric_limits<erthink::int128_t>::max());
  ASSERT_EQ(-/* unary minus is not part of an integer literal */
                170141183460469231731687303715884105727_i128 -
                1_i128,
            std::numeric_limits<erthink::int128_t>::min());
  ASSERT_EQ(142_i128, erthink::int128_t(142));
  ASSERT_EQ(-142_i128, erthink::int128_t(-142));
  ASSERT_EQ(0x2A_i128, erthink::int128_t(42));
  ASSERT_EQ(-0x2A_i128, erthink::int128_t(-42));
  ASSERT_EQ(071_i128, erthink::int128_t(57));
  ASSERT_EQ(-071_i128, erthink::int128_t(-57));
  ASSERT_EQ(82506212570507476930068008648333497346_i128,
            erthink::int128_t(INT64_C(0x3E121E588E3EAFD3),
                              UINT64_C(0x4C51BF2CD35A9402)));
  ASSERT_EQ(18446744073709551615_i128, erthink::int128_t(0, ~UINT64_C(0)));
  ASSERT_EQ(-18446744073709551616_i128, erthink::int128_t(-1, 0));
  ASSERT_EQ(-54942768927535355213284596552279523328_i128,
            erthink::int128_t(UINT64_C(0xD6AA68591879D977), 0));
  ASSERT_EQ(-5633269696077458667_i128,
            erthink::int128_t(-INT64_C(0x4E2D64F136AB6CEB)));
}

TEST(u128, from_chars) {
#if !ERTHINK_HAVE_std_to_chars
  GTEST_SKIP() << "SKIPPEND because of no std::to_chars<>";
#else
  const char *zero = "000000";
  const auto from_zero =
      erthink::uint128_t::from_chars(zero, zero + strlen(zero));
  ASSERT_EQ(std::get<0>(from_zero)[0], '\0');
  ASSERT_EQ(std::get<1>(from_zero), erthink::uint128_t(0));
  ASSERT_EQ(std::get<2>(from_zero), std::errc());

  const char *hex = "0x12345";
  const auto from_hex = erthink::uint128_t::from_chars(hex, hex + strlen(hex));
  ASSERT_EQ(std::get<0>(from_hex)[0], '\0');
  ASSERT_EQ(std::get<1>(from_hex), erthink::uint128_t(0x12345));
  ASSERT_EQ(std::get<2>(from_hex), std::errc());

  const char *max = "340282366920938463463374607431768211455";
  const auto from_max = erthink::uint128_t::from_chars(max, max + strlen(max));
  ASSERT_EQ(std::get<0>(from_max)[0], '\0');
  ASSERT_EQ(std::get<1>(from_max),
            std::numeric_limits<erthink::uint128_t>::max());
  ASSERT_EQ(std::get<2>(from_max), std::errc());

  const char *overflow = "0x340282366920938463463374607431768211455";
  const auto from_overflow =
      erthink::uint128_t::from_chars(overflow, overflow + strlen(overflow));
  ASSERT_EQ(std::get<0>(from_overflow)[0], '\0');
  ASSERT_EQ(std::get<2>(from_overflow), std::errc::result_out_of_range);

  const char *partial = "0123456789"; // octal literal with invalid '89'
  const auto from_partial =
      erthink::uint128_t::from_chars(partial, partial + strlen(partial));
  ASSERT_EQ(std::get<0>(from_partial), partial + 8);
  ASSERT_EQ(std::get<0>(from_partial)[0], '8');
  ASSERT_EQ(std::get<1>(from_partial), erthink::uint128_t(01234567));
  ASSERT_EQ(std::get<2>(from_partial), std::errc());

  const char *invalid = "invalid";
  const auto from_invalid =
      erthink::uint128_t::from_chars(invalid, invalid + strlen(invalid));
  ASSERT_EQ(std::get<0>(from_invalid), invalid);
  ASSERT_EQ(std::get<2>(from_invalid), std::errc::invalid_argument);
#endif /* ERTHINK_HAVE_std_to_chars */
}

TEST(i128, from_chars) {
#if !ERTHINK_HAVE_std_to_chars
  GTEST_SKIP() << "SKIPPEND because of no std::to_chars<>";
#else
  const char *neg_zero = "-0000";
  const auto from_neg_zero =
      erthink::int128_t::from_chars(neg_zero, neg_zero + strlen(neg_zero));
  ASSERT_EQ(std::get<0>(from_neg_zero)[0], '\0');
  ASSERT_EQ(std::get<1>(from_neg_zero), erthink::int128_t(0));
  ASSERT_EQ(std::get<2>(from_neg_zero), std::errc());

  const char *zero = "000000";
  const auto from_zero =
      erthink::int128_t::from_chars(zero, zero + strlen(zero));
  ASSERT_EQ(std::get<0>(from_zero)[0], '\0');
  ASSERT_EQ(std::get<1>(from_zero), erthink::int128_t(0));
  ASSERT_EQ(std::get<2>(from_zero), std::errc());

  const char *neg_hex = "-0x54321";
  const auto from_neg_hex =
      erthink::int128_t::from_chars(neg_hex, neg_hex + strlen(neg_hex));
  ASSERT_EQ(std::get<0>(from_neg_hex)[0], '\0');
  ASSERT_EQ(std::get<1>(from_neg_hex), erthink::int128_t(-0x54321));
  ASSERT_EQ(std::get<2>(from_neg_hex), std::errc());

  const char *hex = "0x12345";
  const auto from_hex = erthink::int128_t::from_chars(hex, hex + strlen(hex));
  ASSERT_EQ(std::get<0>(from_hex)[0], '\0');
  ASSERT_EQ(std::get<1>(from_hex), erthink::int128_t(0x12345));
  ASSERT_EQ(std::get<2>(from_hex), std::errc());

  const char *max = "170141183460469231731687303715884105727";
  const auto from_max = erthink::int128_t::from_chars(max, max + strlen(max));
  ASSERT_EQ(std::get<0>(from_max)[0], '\0');
  ASSERT_EQ(std::get<1>(from_max),
            std::numeric_limits<erthink::int128_t>::max());
  ASSERT_EQ(std::get<2>(from_max), std::errc());

  const char *min = "-170141183460469231731687303715884105728";
  const auto from_min = erthink::int128_t::from_chars(min, min + strlen(min));
  ASSERT_EQ(std::get<0>(from_min)[0], '\0');
  ASSERT_EQ(std::get<1>(from_min),
            std::numeric_limits<erthink::int128_t>::min());
  ASSERT_EQ(std::get<2>(from_min), std::errc());

  const char *overflow_posi = "170141183460469231731687303715884105728";
  const auto from_overflow_posi = erthink::int128_t::from_chars(
      overflow_posi, overflow_posi + strlen(overflow_posi));
  ASSERT_EQ(std::get<0>(from_overflow_posi)[0], '\0');
  ASSERT_EQ(std::get<2>(from_overflow_posi), std::errc::result_out_of_range);

  const char *overflow_nega = "-170141183460469231731687303715884105729";
  const auto from_overflow_nega = erthink::int128_t::from_chars(
      overflow_nega, overflow_nega + strlen(overflow_nega));
  ASSERT_EQ(std::get<0>(from_overflow_nega)[0], '\0');
  ASSERT_EQ(std::get<2>(from_overflow_nega), std::errc::result_out_of_range);

  const char *neg_partial = "-0123456789"; // octal literal with invalid '89'
  const auto from_neg_partial = erthink::int128_t::from_chars(
      neg_partial, neg_partial + strlen(neg_partial));
  ASSERT_EQ(std::get<0>(from_neg_partial), neg_partial + 9);
  ASSERT_EQ(std::get<0>(from_neg_partial)[0], '8');
  ASSERT_EQ(std::get<1>(from_neg_partial), erthink::int128_t(-01234567));
  ASSERT_EQ(std::get<2>(from_neg_partial), std::errc());

  const char *partial = "0123456789"; // octal literal with invalid '89'
  const auto from_partial =
      erthink::int128_t::from_chars(partial, partial + strlen(partial));
  ASSERT_EQ(std::get<0>(from_partial), partial + 8);
  ASSERT_EQ(std::get<0>(from_partial)[0], '8');
  ASSERT_EQ(std::get<1>(from_partial), erthink::int128_t(01234567));
  ASSERT_EQ(std::get<2>(from_partial), std::errc());

  const char *neg_invalid = "-invalid";
  const auto from_neg_invalid = erthink::int128_t::from_chars(
      neg_invalid, neg_invalid + strlen(neg_invalid));
  ASSERT_EQ(std::get<0>(from_neg_invalid), neg_invalid + 1);
  ASSERT_EQ(std::get<2>(from_neg_invalid), std::errc::invalid_argument);

  const char *invalid = "invalid";
  const auto from_invalid =
      erthink::int128_t::from_chars(invalid, invalid + strlen(invalid));
  ASSERT_EQ(std::get<0>(from_invalid), invalid);
  ASSERT_EQ(std::get<2>(from_invalid), std::errc::invalid_argument);
#endif /* ERTHINK_HAVE_std_to_chars */
}

TEST(u128, into_stream) {
  std::stringstream ss;
  const auto default_flags = ss.flags();
  ss << std::numeric_limits<erthink::uint128_t>::max();
  ASSERT_EQ(ss.str(), "340282366920938463463374607431768211455");

  ss.str("");
  ss << std::numeric_limits<erthink::uint128_t>::min();
  ASSERT_EQ(ss.str(), "0");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setw(9) << 42;
  ASSERT_EQ(ss.str(), "       42");

  ss.str("");
  ss.flags(default_flags);
  ss << std::left << std::setw(5) << erthink::uint128_t(42);
  ASSERT_EQ(ss.str(), "42   ");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setfill('_') << std::showbase << std::uppercase << std::oct
     << std::internal << std::setw(5) << erthink::uint128_t(42);
  ASSERT_EQ(ss.str(), "0__52");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setfill('_') << std::showpos << std::showbase << std::oct
     << std::left << std::setw(5) << erthink::uint128_t(42);
  ASSERT_EQ(ss.str(), "+052_");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setfill('_') << std::showbase << std::uppercase << std::hex
     << std::internal << std::setw(5) << erthink::uint128_t(42);
  ASSERT_EQ(ss.str(), "0X_2A");
}

TEST(i128, into_stream) {
  std::stringstream ss;
  const auto default_flags = ss.flags();
  ss << std::numeric_limits<erthink::int128_t>::max();
  ASSERT_EQ(ss.str(), "170141183460469231731687303715884105727");

  ss.str("");
  ss << std::numeric_limits<erthink::int128_t>::min();
  ASSERT_EQ(ss.str(), "-170141183460469231731687303715884105728");

  ss.str("");
  ss << erthink::int128_t(0);
  ASSERT_EQ(ss.str(), "0");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setw(9) << -42;
  ASSERT_EQ(ss.str(), "      -42");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setw(9) << 42;
  ASSERT_EQ(ss.str(), "       42");

  ss.str("");
  ss.flags(default_flags);
  ss << std::left << std::setw(5) << erthink::int128_t(-42);
  ASSERT_EQ(ss.str(), "-42  ");

  ss.str("");
  ss.flags(default_flags);
  ss << std::left << std::setw(5) << erthink::int128_t(42);
  ASSERT_EQ(ss.str(), "42   ");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setfill('_') << std::showbase << std::uppercase << std::oct
     << std::internal << std::setw(5) << erthink::int128_t(-42);
  ASSERT_EQ(ss.str(), "-0_52");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setfill('_') << std::showbase << std::uppercase << std::oct
     << std::internal << std::setw(5) << erthink::int128_t(42);
  ASSERT_EQ(ss.str(), "0__52");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setfill('_') << std::showpos << std::showbase << std::oct
     << std::left << std::setw(5) << erthink::int128_t(-42);
  ASSERT_EQ(ss.str(), "-052_");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setfill('_') << std::showpos << std::showbase << std::oct
     << std::left << std::setw(5) << erthink::int128_t(42);
  ASSERT_EQ(ss.str(), "+052_");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setfill('_') << std::showbase << std::uppercase << std::hex
     << std::internal << std::setw(5) << erthink::int128_t(-42);
  ASSERT_EQ(ss.str(), "-0X2A");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setfill('_') << std::showbase << std::uppercase << std::hex
     << std::internal << std::setw(6) << erthink::int128_t(-42);
  ASSERT_EQ(ss.str(), "-0X_2A");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setfill('_') << std::showbase << std::uppercase << std::hex
     << std::internal << std::setw(1) << erthink::int128_t(-42);
  ASSERT_EQ(ss.str(), "-0X2A");

  ss.str("");
  ss.flags(default_flags);
  ss << std::setfill('#') << std::showbase << std::hex << std::internal
     << std::setw(5) << erthink::int128_t(42);
  ASSERT_EQ(ss.str(), "0x#2a");
}

//------------------------------------------------------------------------------

struct uint64_lcg {
  using result_type = uint64_t;
  result_type state;

  constexpr uint64_lcg(uint64_t seed) noexcept : state(seed) {}
  static constexpr result_type min() noexcept { return 0; }
  static constexpr result_type max() noexcept { return ~result_type(0); }
  cxx14_constexpr result_type operator()() {
    const result_type r = (state += UINT64_C(1442695040888963407));
    state *= UINT64_C(6364136223846793005);
    return r;
  }
  cxx14_constexpr result_type operator()(size_t range) {
    return operator()() % range;
  }
};

static uint64_lcg lcg(uint64_t(time(0)));

#if defined(ERTHINK_NATIVE_U128_TYPE) || defined(ERTHINK_NATIVE_I128_TYPE)

static std::array<unsigned, 128> random_shuffle_0_127() noexcept {
  std::array<unsigned, 128> r;
  std::iota(r.begin(), r.end(), 0);
  std::shuffle(r.begin(), r.end(), lcg);
  return r;
}

#endif /* ERTHINK_NATIVE_U128_TYPE || ERTHINK_NATIVE_I128_TYPE */

__maybe_unused static uint64_t N;

template <typename T> static void probe(const T &a, const T &b) {
  using NATIVE = typename T::native;
  ++N;
  ASSERT_EQ((a > b), (NATIVE(a) > NATIVE(b)));
  ASSERT_EQ((a >= b), (NATIVE(a) >= NATIVE(b)));
  ASSERT_EQ((a == b), (NATIVE(a) == NATIVE(b)));
  ASSERT_EQ((a != b), (NATIVE(a) != NATIVE(b)));
  ASSERT_EQ((a < b), (NATIVE(a) < NATIVE(b)));
  ASSERT_EQ((a <= b), (NATIVE(a) <= NATIVE(b)));

#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid signed-integer-overflow UB
  if (std::is_signed<NATIVE>::value && NATIVE(a) != 0 && NATIVE(b) != 0) {
    ASSERT_EQ(NATIVE(a + b), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) +
                                    ERTHINK_NATIVE_U128_TYPE(b)));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(a + b), NATIVE(a) + NATIVE(b));
  }

#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid signed-integer-overflow UB
  if (std::is_signed<NATIVE>::value && ((NATIVE(a) != 0 && NATIVE(b) != 0) ||
                                        b == std::numeric_limits<T>::min())) {
    ASSERT_EQ(NATIVE(a - b), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) -
                                    ERTHINK_NATIVE_U128_TYPE(b)));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(a - b), NATIVE(a) - NATIVE(b));
  }

  ASSERT_EQ(NATIVE(a ^ b), NATIVE(a) ^ NATIVE(b));
  ASSERT_EQ(NATIVE(a | b), NATIVE(a) | NATIVE(b));
  ASSERT_EQ(NATIVE(a & b), NATIVE(a) & NATIVE(b));
#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid signed-integer-overflow UB
  if (std::is_signed<NATIVE>::value && NATIVE(a) != 0 && NATIVE(b) != 0) {
    ASSERT_EQ(NATIVE(a * b), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) *
                                    ERTHINK_NATIVE_U128_TYPE(b)));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(a * b), NATIVE(a) * NATIVE(b));
  }

#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid signed-integer-overflow UB
  if (std::is_signed<NATIVE>::value && a == std::numeric_limits<T>::min()) {
    ASSERT_EQ(NATIVE(-a), NATIVE(-ERTHINK_NATIVE_U128_TYPE(a)));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(-a), -NATIVE(a));
  }
  ASSERT_EQ(NATIVE(~a), ~NATIVE(a));
  ASSERT_EQ(!NATIVE(!a), !!NATIVE(a));

  if (b) {
    const auto pair = T::divmod(a, b);
    const auto q = NATIVE(a) / NATIVE(b);
    const auto r = NATIVE(a) % NATIVE(b);
    ASSERT_EQ(NATIVE(pair.quotient), q);
    ASSERT_EQ(NATIVE(pair.remainder), r);
  }

  const auto s = unsigned(b) & 127;
  ASSERT_EQ(NATIVE(a >> s), NATIVE(a) >> s);
#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid left-shift-of-negative and left-shift-signed-overflow UB
  if (std::is_signed<NATIVE>::value &&
      (NATIVE(a) < 0 || (a && 128 - clz(a) + s > 128))) {
    ASSERT_EQ(NATIVE(a << s), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) << s));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(a << s), NATIVE(a) << s);
  }
}

template <typename T> static void probe_full(const T &a, const T &b) {
  using NATIVE = typename T::native;
  const auto s = unsigned(b) & 127;

  ASSERT_NE(NATIVE(a), NATIVE(a) + 1);
#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid signed-integer-overflow UB
  if (std::is_signed<NATIVE>::value && a == std::numeric_limits<T>::min()) {
    ASSERT_NE(NATIVE(a), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) - 1));
  } else
#endif
  {
    ASSERT_NE(NATIVE(a), NATIVE(a) - 1);
  }
  probe<T>(a, b);

  auto t = a;
#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid signed-integer-overflow UB
  if (std::is_signed<NATIVE>::value && NATIVE(a) != 0 && NATIVE(b) != 0) {
    ASSERT_EQ(NATIVE(t += b), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) +
                                     ERTHINK_NATIVE_U128_TYPE(b)));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(t += b), NATIVE(a) + NATIVE(b));
  }

  t = a;
#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid signed-integer-overflow UB
  if (std::is_signed<NATIVE>::value && ((NATIVE(a) != 0 && NATIVE(b) != 0) ||
                                        b == std::numeric_limits<T>::min())) {
    ASSERT_EQ(NATIVE(t -= b), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) -
                                     ERTHINK_NATIVE_U128_TYPE(b)));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(t -= b), NATIVE(a) - NATIVE(b));
  }

  t = a;
  ASSERT_EQ(NATIVE(t ^= b), NATIVE(a) ^ NATIVE(b));
  t = a;
  ASSERT_EQ(NATIVE(t |= b), NATIVE(a) | NATIVE(b));
  t = a;
  ASSERT_EQ(NATIVE(t &= b), NATIVE(a) & NATIVE(b));

  t = a;
#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid signed-integer-overflow UB
  if (std::is_signed<NATIVE>::value && NATIVE(a) != 0 && NATIVE(b) != 0) {
    ASSERT_EQ(NATIVE(t *= b), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) *
                                     ERTHINK_NATIVE_U128_TYPE(b)));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(t *= b), NATIVE(a) * NATIVE(b));
  }

  if (b) {
    t = a;
    ASSERT_EQ(NATIVE(t /= b), NATIVE(a) / NATIVE(b));
    t = a;
    ASSERT_EQ(NATIVE(t %= b), NATIVE(a) % NATIVE(b));
  }

  t = a;
  ASSERT_EQ(NATIVE(t >>= s), NATIVE(a) >> s);
  t = a;
#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid left-shift-of-negative UB
  if (std::is_signed<NATIVE>::value &&
      (NATIVE(a) < 0 || (a && 128 - clz(a) + s > 128))) {
    ASSERT_EQ(NATIVE(t <<= s), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) << s));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(t <<= s), NATIVE(a) << s);
  }

  ASSERT_EQ(NATIVE(ror(a, s)), erthink::ror(NATIVE(a), s));
  ASSERT_EQ(NATIVE(rol(a, s)), erthink::rol(NATIVE(a), s));

  t = a;
  ASSERT_EQ(NATIVE(t++), NATIVE(a));
#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid signed-integer-overflow UB
  if (std::is_signed<NATIVE>::value &&
      ((a == std::numeric_limits<T>::min() && NATIVE(b) < 0) ||
       (NATIVE(a) < 0 && b == std::numeric_limits<T>::min()))) {
    ASSERT_EQ(NATIVE(t), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) + 1));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(t), NATIVE(a) + 1);
  }

  t = a;
  ASSERT_EQ(NATIVE(t--), NATIVE(a));
#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid signed-integer-overflow UB
  if (std::is_signed<NATIVE>::value && a == std::numeric_limits<T>::min()) {
    ASSERT_EQ(NATIVE(t), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) - 1));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(t), NATIVE(a) - 1);
  }

  t = a;
  ASSERT_EQ(NATIVE(++t), NATIVE(a) + 1);
#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid signed-integer-overflow UB
  if (std::is_signed<NATIVE>::value &&
      ((a == std::numeric_limits<T>::min() && NATIVE(b) < 0) ||
       (NATIVE(a) < 0 && b == std::numeric_limits<T>::min()))) {
    ASSERT_EQ(NATIVE(t), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) + 1));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(t), NATIVE(a) + 1);
  }

  t = a;
#if defined(ERTHINK_NATIVE_U128_TYPE) && defined(UBSAN_ENABLED)
  // avoid signed-integer-overflow UB
  if (std::is_signed<NATIVE>::value && a == std::numeric_limits<T>::min()) {
    ASSERT_EQ(NATIVE(--t), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) - 1));
    ASSERT_EQ(NATIVE(t), NATIVE(ERTHINK_NATIVE_U128_TYPE(a) - 1));
  } else
#endif
  {
    ASSERT_EQ(NATIVE(--t), NATIVE(a) - 1);
    ASSERT_EQ(NATIVE(t), NATIVE(a) - 1);
  }
}

#ifdef DO_GENERATE_BASIC

#include <fstream>

static void replace(std::string &oper, const std::string &var,
                    const std::string &val) {
  const auto pos = oper.find(var);
  if (pos != std::string::npos)
    oper.replace(pos, var.length(), val);
}

static void gen_expect(std::ostream &out, const char *_oper, const unsigned s,
                       const bool x, const bool y) {
  assert(x == y);
  std::string oper(_oper);

  replace(oper, "!(!", "(");
  out << "  EXPECT_" << (x ? "TRUE" : "FALSE") << oper << ";\n";
  (void)s;
  (void)y;
}

static const char *value(const erthink::uint128_t &v) {
  static char buf[128];
  if (v.h == 0)
    snprintf(buf, sizeof(buf), "IMP(UINT64_C(0x%016" PRIx64 "))", v.l);
  else if (v.h == UINT64_MAX && v.l > INT64_MAX)
    snprintf(buf, sizeof(buf), "IMP(-INT64_C(%" PRIi64 "))", -int64_t(v));
  else
    snprintf(buf, sizeof(buf),
             "IMP(UINT64_C(0x%016" PRIx64 "), UINT64_C(0x%016" PRIx64 "))", v.h,
             v.l);
  return buf;
}

static const char *value(const erthink::int128_t &v) {
  static char buf[128];
  if ((v.h == 0 && v.ls >= 0) || (v.h == -1 && v.ls < 0))
    snprintf(buf, sizeof(buf), "IMP(INT64_C(%" PRIi64 "))", int64_t(v));
  else
    snprintf(buf, sizeof(buf),
             "IMP(UINT64_C(0x%016" PRIx64 "), UINT64_C(0x%016" PRIx64 "))", v.h,
             v.lu);
  return buf;
}

template <typename IMP>
static void gen_expect(std::ostream &out, const char *_oper, const unsigned s,
                       const IMP x, const typename IMP::native y) {
  assert(typename IMP::native(x) == y);
  std::string oper(_oper);

  replace(oper, "S", std::to_string(s));
  if (strchr(_oper, 'T') && oper != "(T)")
    replace(oper, "(", "(T = A, ");
  else if (oper.front() == '(' && oper.back() == ')')
    oper = oper.substr(1, oper.size() - 2);

  out << "  EXPECT_EQ(" << oper << ", " << value(x) << ");\n";
  (void)y;
}

template <typename IMP>
static void gen_body(std::ostream &out, const IMP &A, const IMP &B) {
  using NATIVE = typename IMP::native;
  out << "  const IMP A = " << value(A) << ";\n";
  out << "  const IMP B = " << value(B) << ";\n";
  out << "  IMP T;\n";
  const auto S = unsigned(B) & 127;
  auto T = A;
#define OUT(X, Y) gen_expect(out, STRINGIFY(X), S, X, Y)

  OUT((A > B), (NATIVE(A) > NATIVE(B)));
  OUT((A >= B), (NATIVE(A) >= NATIVE(B)));
  OUT((A == B), (NATIVE(A) == NATIVE(B)));
  OUT((A != B), (NATIVE(A) != NATIVE(B)));
  OUT((A < B), (NATIVE(A) < NATIVE(B)));
  OUT((A <= B), (NATIVE(A) <= NATIVE(B)));

  OUT((A + B), NATIVE(A) + NATIVE(B));
  OUT((A - B), NATIVE(A) - NATIVE(B));
  OUT((A ^ B), NATIVE(A) ^ NATIVE(B));
  OUT((A | B), NATIVE(A) | NATIVE(B));
  OUT((A & B), NATIVE(A) & NATIVE(B));
  OUT((A * B), NATIVE(A) * NATIVE(B));

  OUT((-A), -NATIVE(A));
  OUT((~A), ~NATIVE(A));
  OUT(!(!A), !!NATIVE(A));

  if (B) {
    OUT(IMP::divmod(A, B).quotient, NATIVE(A) / NATIVE(B));
    OUT(IMP::divmod(A, B).remainder, NATIVE(A) % NATIVE(B));
  }

  OUT((A >> S), NATIVE(A) >> S);
  OUT((A << S), NATIVE(A) << S);

  T = A;
  OUT((T += B), NATIVE(A) + NATIVE(B));
  T = A;
  OUT((T -= B), NATIVE(A) - NATIVE(B));
  T = A;
  OUT((T ^= B), NATIVE(A) ^ NATIVE(B));
  T = A;
  OUT((T |= B), NATIVE(A) | NATIVE(B));
  T = A;
  OUT((T &= B), NATIVE(A) & NATIVE(B));
  T = A;
  OUT((T *= B), NATIVE(A) * NATIVE(B));

  if (B) {
    T = A;
    OUT((T /= B), NATIVE(A) / NATIVE(B));
    T = A;
    OUT((T %= B), NATIVE(A) % NATIVE(B));
  }

  T = A;
  OUT((T >>= S), NATIVE(A) >> S);
  T = A;
  OUT((T <<= S), NATIVE(A) << S);

  OUT((ror(A, S)), erthink::ror(NATIVE(A), S));
  OUT((rol(A, S)), erthink::rol(NATIVE(A), S));

  T = A;
  OUT((T++), NATIVE(A));
  OUT((T), NATIVE(A) + 1);
  T = A;
  OUT((T--), NATIVE(A));
  OUT((T), NATIVE(A) - 1);
  T = A;
  OUT((++T), NATIVE(A) + 1);
  OUT((T), NATIVE(A) + 1);
  T = A;
  OUT((--T), NATIVE(A) - 1);
  OUT((T), NATIVE(A) - 1);

#undef OUT
}

static void gen(std::ostream &out, const erthink::uint128_t &a,
                const erthink::uint128_t &b) {
  static unsigned n;

  out << "\nTEST(u128, autogen_basic_" << n++ << ") {\n";
  out << "  using IMP = erthink::uint128_t;\n";
  gen_body<erthink::uint128_t>(out, a, b);
  out << "}\n";

  out << "\nTEST(i128, autogen_basic_" << n++ << ") {\n";
  out << "  using IMP = erthink::int128_t;\n";
  gen_body<erthink::int128_t>(out, erthink::int128_t(a), erthink::int128_t(b));
  out << "}\n";
}

#endif /* DO_GENERATE_BASIC */

//------------------------------------------------------------------------------

TEST(u128, smoke) {
#ifndef ERTHINK_NATIVE_U128_TYPE
  GTEST_SKIP() << "SKIPPEND because of no native __uint128_t";
#else
  using imp = erthink::uint128_t;
  using native = imp::native;
  probe_full<imp>(0, 0);
  probe_full<imp>(~native(0), ~native(0));
  probe_full<imp>(~native(0), 11);
  probe_full<imp>(7, ~native(0));
  probe_full<imp>(1, 0);
  probe_full<imp>(0, -2);
  probe_full<imp>(3, 42);
  probe_full<imp>(~0, 421);
  probe_full<imp>(~42, 5);
  probe_full<imp>(~421, INT_MAX);
  probe_full<imp>(UINT64_C(13632396072180810313),
                  UINT64_C(4895412794877399892));
  probe_full<imp>(UINT64_C(5008002785836588600), UINT64_C(6364136223846793005));

  double a = DBL_MAX, b = DBL_MAX;
  while (a + b > 1) {
    a /= 1.1283791670955125739 /* 2/sqrt(pi) */;
    probe_full<imp>(native(std::fmod(a, std::ldexp(1.0, 128))),
                    native(std::fmod(b, std::ldexp(1.0, 128))));
    probe_full<imp>(native(std::fmod(b, std::ldexp(1.0, 128))),
                    native(std::fmod(a, std::ldexp(1.0, 128))));
    b *= 0.91893853320467274178 /* ln(sqrt(2pi)) */;
    probe_full<imp>(native(std::fmod(a, std::ldexp(1.0, 128))),
                    native(std::fmod(b, std::ldexp(1.0, 128))));
    probe_full<imp>(native(std::fmod(b, std::ldexp(1.0, 128))),
                    native(std::fmod(a, std::ldexp(1.0, 128))));
  }
#endif /* ERTHINK_NATIVE_U128_TYPE */
}

TEST(i128, smoke) {
#ifndef ERTHINK_NATIVE_I128_TYPE
  GTEST_SKIP() << "SKIPPEND because of no native __int128_t";
#else
  using imp = erthink::int128_t;
  using native = imp::native;
  probe_full<imp>(0, 0);
  probe_full<imp>(~native(0), ~native(0));
  probe_full<imp>(~native(0), 11);
  probe_full<imp>(7, ~native(0));
  probe_full<imp>(1, 0);
  probe_full<imp>(0, -2);
  probe_full<imp>(3, 42);
  probe_full<imp>(~0, 421);
  probe_full<imp>(~42, 5);
  probe_full<imp>(~421, INT_MAX);
  probe_full<imp>(UINT64_C(13632396072180810313),
                  UINT64_C(4895412794877399892));
  probe_full<imp>(UINT64_C(5008002785836588600), UINT64_C(6364136223846793005));

  double a = DBL_MAX, b = DBL_MAX;
  while (a + b > 1) {
    a /= 1.1283791670955125739 /* 2/sqrt(pi) */;
    probe_full<imp>(
        native(std::fmod(a, std::ldexp(1.0, 128)) - std::ldexp(1.0, 127)),
        native(std::fmod(b, std::ldexp(1.0, 128)) - std::ldexp(1.0, 127)));
    probe_full<imp>(
        native(std::fmod(b, std::ldexp(1.0, 128)) - std::ldexp(1.0, 127)),
        native(std::fmod(a, std::ldexp(1.0, 128)) - std::ldexp(1.0, 127)));
    b *= 0.91893853320467274178 /* ln(sqrt(2pi)) */;
    probe_full<imp>(
        native(std::fmod(a, std::ldexp(1.0, 128)) - std::ldexp(1.0, 127)),
        native(std::fmod(b, std::ldexp(1.0, 128)) - std::ldexp(1.0, 127)));
    probe_full<imp>(
        native(std::fmod(b, std::ldexp(1.0, 128)) - std::ldexp(1.0, 127)),
        native(std::fmod(a, std::ldexp(1.0, 128)) - std::ldexp(1.0, 127)));
  }
#endif /* ERTHINK_NATIVE_I128_TYPE */
}

//------------------------------------------------------------------------------

TEST(u128, random3e6) {
#ifndef ERTHINK_NATIVE_U128_TYPE
  GTEST_SKIP() << "SKIPPEND because of no native __uint128_t";
#else
  using imp = erthink::uint128_t;
  SCOPED_TRACE("PRNG seed=" + std::to_string(lcg.state));
  for (auto i = 0; i < 33333; ++i) {
    probe_full<imp>(lcg(), lcg());
    probe_full<imp>({lcg(), lcg()}, lcg());
    probe_full<imp>(lcg(), {lcg(), lcg()});
    probe_full<imp>({lcg(), lcg()}, {lcg(), lcg()});
    probe_full<imp>({lcg(), lcg()}, {lcg(), lcg()});

    probe_full<imp>({lcg(), 0}, {lcg(), lcg()});
    probe_full<imp>({lcg(), lcg()}, {lcg(), 0});
    probe_full<imp>({lcg(), 0}, {lcg(), 0});
    probe_full<imp>({lcg(), 0}, lcg());
    probe_full<imp>(lcg(), {lcg(), 0});

    probe_full<imp>({UINT64_MAX, lcg()}, {lcg(), lcg()});
    probe_full<imp>({lcg(), lcg()}, {UINT64_MAX, lcg()});
    probe_full<imp>({UINT64_MAX, lcg()}, {UINT64_MAX, lcg()});
    probe_full<imp>({UINT64_MAX, lcg()}, lcg());
    probe_full<imp>(lcg(), {UINT64_MAX, lcg()});

    probe_full<imp>({INT64_MAX, lcg()}, {lcg(), lcg()});
    probe_full<imp>({lcg(), lcg()}, {INT64_MAX, lcg()});
    probe_full<imp>({INT64_MAX, lcg()}, {INT64_MAX, lcg()});
    probe_full<imp>({INT64_MAX, lcg()}, lcg());
    probe_full<imp>(lcg(), {INT64_MAX, lcg()});

    probe_full<imp>({UINT64_C(0x8000000000000000), lcg()}, {lcg(), lcg()});
    probe_full<imp>({lcg(), lcg()}, {UINT64_C(0x8000000000000000), lcg()});
    probe_full<imp>({UINT64_C(0x8000000000000000), lcg()},
                    {UINT64_C(0x8000000000000000), lcg()});
    probe_full<imp>({UINT64_C(0x8000000000000000), lcg()}, lcg());
    probe_full<imp>(lcg(), {UINT64_C(0x8000000000000000), lcg()});

    if (GTEST_IS_EXECUTION_TIMEOUT())
      break;
  }
#endif /* ERTHINK_NATIVE_U128_TYPE */
}

TEST(i128, random3e6) {
#ifndef ERTHINK_NATIVE_I128_TYPE
  GTEST_SKIP() << "SKIPPEND because of no native __int128_t";
#else
  using imp = erthink::int128_t;
  SCOPED_TRACE("PRNG seed=" + std::to_string(lcg.state));
  for (auto i = 0; i < 33333; ++i) {
    probe_full<imp>(lcg(), lcg());
    probe_full<imp>({int64_t(lcg()), lcg()}, lcg());
    probe_full<imp>(lcg(), {int64_t(lcg()), lcg()});
    probe_full<imp>({int64_t(lcg()), lcg()}, {int64_t(lcg()), lcg()});
    probe_full<imp>({int64_t(lcg()), lcg()}, {int64_t(lcg()), lcg()});

    probe_full<imp>({int64_t(lcg()), 0}, {int64_t(lcg()), lcg()});
    probe_full<imp>({int64_t(lcg()), lcg()}, {int64_t(lcg()), 0});
    probe_full<imp>({int64_t(lcg()), 0}, {int64_t(lcg()), 0});
    probe_full<imp>({int64_t(lcg()), 0}, lcg());
    probe_full<imp>(lcg(), {int64_t(lcg()), 0});

    probe_full<imp>({INT64_MAX, lcg()}, {int64_t(lcg()), lcg()});
    probe_full<imp>({int64_t(lcg()), lcg()}, {INT64_MAX, lcg()});
    probe_full<imp>({INT64_MAX, lcg()}, {INT64_MAX, lcg()});
    probe_full<imp>({INT64_MAX, lcg()}, lcg());
    probe_full<imp>(lcg(), {INT64_MAX, lcg()});

    probe_full<imp>({INT64_MIN, lcg()}, {int64_t(lcg()), lcg()});
    probe_full<imp>({int64_t(lcg()), lcg()}, {INT64_MIN, lcg()});
    probe_full<imp>({INT64_MIN, lcg()}, {INT64_MIN, lcg()});
    probe_full<imp>({INT64_MIN, lcg()}, lcg());
    probe_full<imp>(lcg(), {INT64_MIN, lcg()});

    if (GTEST_IS_EXECUTION_TIMEOUT())
      break;
  }
#endif /* ERTHINK_NATIVE_I128_TYPE */
}

//------------------------------------------------------------------------------

TEST(u128, DISABLED_stairwell) {
#ifndef ERTHINK_NATIVE_U128_TYPE
  GTEST_SKIP() << "SKIPPEND because of no native __uint128_t";
#else
  using imp = erthink::uint128_t;
  using native = imp::native;
  SCOPED_TRACE("PRNG seed=" + std::to_string(lcg.state));
  const auto outer = random_shuffle_0_127();
  const auto inner = random_shuffle_0_127();
  // Total 1`065`418`752 probe() calls
  for (const auto i : outer) {
    const auto base_a = (~native(0)) >> i;
    for (const auto j : inner) {
      const auto base_b = (~native(0)) >> j;
      for (auto offset_a = base_a; offset_a >>= 1;) {
        for (auto offset_b = base_b; offset_b >>= 1;) {
          probe<imp>(base_a + offset_a, base_b + offset_b);
          probe<imp>(base_a + offset_a, base_b - offset_b);
          probe<imp>(base_a - offset_a, base_b + offset_b);
          probe<imp>(base_a - offset_a, base_b - offset_b);

          probe<imp>(base_a + offset_a, ~base_b + offset_b);
          probe<imp>(base_a + offset_a, ~base_b - offset_b);
          probe<imp>(base_a - offset_a, ~base_b + offset_b);
          probe<imp>(base_a - offset_a, ~base_b - offset_b);

          probe<imp>(~base_a + offset_a, base_b + offset_b);
          probe<imp>(~base_a + offset_a, base_b - offset_b);
          probe<imp>(~base_a - offset_a, base_b + offset_b);
          probe<imp>(~base_a - offset_a, base_b - offset_b);

          probe<imp>(~base_a + offset_a, ~base_b + offset_b);
          probe<imp>(~base_a + offset_a, ~base_b - offset_b);
          probe<imp>(~base_a - offset_a, ~base_b + offset_b);
          probe<imp>(~base_a - offset_a, ~base_b - offset_b);
        }
        probe<imp>(base_a + offset_a, base_b);
        probe<imp>(base_a - offset_a, base_b);
        probe<imp>(base_a + offset_a, ~base_b);
        probe<imp>(base_a - offset_a, ~base_b);
        probe<imp>(~base_a + offset_a, base_b);
        probe<imp>(~base_a - offset_a, base_b);
        probe<imp>(~base_a + offset_a, ~base_b);
        probe<imp>(~base_a - offset_a, ~base_b);
      }
      probe<imp>(base_a, base_b);
      probe<imp>(base_a, ~base_b);
      probe<imp>(~base_a, base_b);
      probe<imp>(~base_a, ~base_b);
      if (GTEST_IS_EXECUTION_TIMEOUT())
        return;
    }
  }
#endif /* ERTHINK_NATIVE_U128_TYPE */
}

TEST(i128, DISABLED_stairwell) {
#ifndef ERTHINK_NATIVE_I128_TYPE
  GTEST_SKIP() << "SKIPPEND because of no native __int128_t";
#else
  using imp = erthink::int128_t;
  using native = imp::native;
  SCOPED_TRACE("PRNG seed=" + std::to_string(lcg.state));
  const auto outer = random_shuffle_0_127();
  const auto inner = random_shuffle_0_127();
  // Total 1`065`418`752 probe() calls
  for (const auto i : outer) {
    const auto base_a = (~ERTHINK_NATIVE_U128_TYPE(0)) >> i;
    for (const auto j : inner) {
      const auto base_b = (~ERTHINK_NATIVE_U128_TYPE(0)) >> j;
      for (auto offset_a = base_a; offset_a >>= 1;) {
        for (auto offset_b = base_b; offset_b >>= 1;) {
          probe<imp>(native(base_a) + offset_a, native(base_b) + offset_b);
          probe<imp>(native(base_a) + offset_a, native(base_b) - offset_b);
          probe<imp>(native(base_a) - offset_a, native(base_b) + offset_b);
          probe<imp>(native(base_a) - offset_a, native(base_b) - offset_b);

          probe<imp>(native(base_a) + offset_a, ~native(base_b) + offset_b);
          probe<imp>(native(base_a) + offset_a, ~native(base_b) - offset_b);
          probe<imp>(native(base_a) - offset_a, ~native(base_b) + offset_b);
          probe<imp>(native(base_a) - offset_a, ~native(base_b) - offset_b);

          probe<imp>(~native(base_a) + offset_a, native(base_b) + offset_b);
          probe<imp>(~native(base_a) + offset_a, native(base_b) - offset_b);
          probe<imp>(~native(base_a) - offset_a, native(base_b) + offset_b);
          probe<imp>(~native(base_a) - offset_a, native(base_b) - offset_b);

          probe<imp>(~native(base_a) + offset_a, ~native(base_b) + offset_b);
          probe<imp>(~native(base_a) + offset_a, ~native(base_b) - offset_b);
          probe<imp>(~native(base_a) - offset_a, ~native(base_b) + offset_b);
          probe<imp>(~native(base_a) - offset_a, ~native(base_b) - offset_b);
        }
        probe<imp>(native(base_a) + offset_a, native(base_b));
        probe<imp>(native(base_a) - offset_a, native(base_b));
        probe<imp>(native(base_a) + offset_a, ~native(base_b));
        probe<imp>(native(base_a) - offset_a, ~native(base_b));
        probe<imp>(~native(base_a) + offset_a, native(base_b));
        probe<imp>(~native(base_a) - offset_a, native(base_b));
        probe<imp>(~native(base_a) + offset_a, ~native(base_b));
        probe<imp>(~native(base_a) - offset_a, ~native(base_b));
      }
      probe<imp>(native(base_a), native(base_b));
      probe<imp>(native(base_a), ~native(base_b));
      probe<imp>(~native(base_a), native(base_b));
      probe<imp>(~native(base_a), ~native(base_b));
      if (GTEST_IS_EXECUTION_TIMEOUT())
        return;
    }
  }
#endif /* ERTHINK_NATIVE_I128_TYPE */
}

//------------------------------------------------------------------------------

runtime_limiter ci_runtime_limiter;

int main(int argc, char **argv) {

#ifdef DO_GENERATE_BASIC
  std::ofstream out("autogen_u128_basic.h++");
  out << "// The contents of this file are generated automatically.\n";
  out << "// You should not edit it manually.\n";
  for (auto i = 0; i < 5; ++i) {
    gen(out, lcg(), lcg());
    gen(out, {lcg(), lcg()}, lcg());
    gen(out, lcg(), {lcg(), lcg()});
    gen(out, {lcg(), lcg()}, {lcg(), lcg()});
    gen(out, {lcg(), lcg()}, {lcg(), lcg()});

    gen(out, {lcg(), 0}, {lcg(), lcg()});
    gen(out, {lcg(), lcg()}, {lcg(), 0});
    gen(out, {lcg(), 0}, {lcg(), 0});
    gen(out, {lcg(), 0}, lcg());
    gen(out, lcg(), {lcg(), 0});

    gen(out, {UINT64_MAX, lcg()}, {lcg(), lcg()});
    gen(out, {lcg(), lcg()}, {UINT64_MAX, lcg()});
    gen(out, {UINT64_MAX, lcg()}, {UINT64_MAX, lcg()});
    gen(out, {UINT64_MAX, lcg()}, lcg());
    gen(out, lcg(), {UINT64_MAX, lcg()});
  }
  out.close();
#endif /* DO_GENERATE_BASIC */

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#include "testing.h++"

#include "erthink.h++"

#include "erthink_carryadd.h"
#include "erthink_mul.h"

#include <ostream>

//------------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201) /* nonstandard extension used :                \
                                   nameless struct / union */
#endif                          /* _MSC_VER (warnings) */

typedef union u256 {
  struct {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint64_t a, b, c, d;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    uint64_t d, c, b, a;
#else
#error "FIXME: Unsupported byte order"
#endif /* __BYTE_ORDER__ */
  };
  uint64_t u64[4];
} u256_t;

inline std::ostream &operator<<(::std::ostream &out, u256_t v) {
  const ::std::ostream::sentry sentry(out);
  const auto flags = out.setf(::std::ios_base::hex, ::std::ios_base::basefield);
  out << v.d << "." << v.c << "." << v.b << "." << v.a;
  out.setf(flags);
  return out;
}

inline bool operator==(const u256_t &x, const u256_t &y) noexcept {
  return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d;
}

typedef union u512 {
  struct {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint64_t a, b, c, d, e, f, g, h;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    uint64_t h, g, f, e, d, c, b, a;
#else
#error "FIXME: Unsupported byte order"
#endif /* __BYTE_ORDER__ */
  };
  uint64_t u64[8];
} u512_t;

inline std::ostream &operator<<(::std::ostream &out, u512_t v) {
  const ::std::ostream::sentry sentry(out);
  const auto flags = out.setf(::std::ios_base::hex, ::std::ios_base::basefield);
  out << v.h << "." << v.g << "." << v.f << "." << v.e << "." << v.d << "."
      << v.c << "." << v.b << "." << v.a;
  out.setf(flags);
  return out;
}

inline bool operator==(const u512_t &x, const u512_t &y) noexcept {
  return x.a == y.a && x.b == y.b && x.c == y.c && x.d == y.d && x.e == y.e &&
         x.f == y.f && x.g == y.g && x.h == y.h;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

//------------------------------------------------------------------------------

extern "C" __dll_export __hot void
mulset_256x256_256(u256_t *__restrict r, const u256_t *x, const u256_t *y) {
  using namespace erthink;
#define MUL(X, Y)                                                              \
  uint64_t H##X##Y, L##X##Y = mul_64x64_128(x->X, y->Y, &H##X##Y)
  MUL(a, a);
  MUL(a, b);
  MUL(a, c);
  MUL(a, d);
  MUL(b, a);
  MUL(b, b);
  MUL(b, c);
  MUL(c, a);
  MUL(c, b);
  uint64_t Lda = x->d * y->a;
#undef MUL
  //        d   c   b   a
  //  a x Lad Lac Lab Laa
  //      Hac Hab Haa
  //  b x Lbc Lbb Lba
  //      Hbb Lba
  //  c x Lcb Lca
  //      Hca
  //  d x Lda

  r->a = Laa;

  auto c0 = add64carry_first(Lab, Haa, &r->b);
  c0 = add64carry_next(c0, Lac, Hab, &r->c);
  add64carry_last(c0, Lad, Hac, &r->d);

  auto c1 = add64carry_first(Lba, r->b, &r->b);
  c1 = add64carry_next(c1, Lbb, r->c, &r->c);
  add64carry_last(c1, Lbc, r->d, &r->d);

  auto c2 = add64carry_first(Hba, r->c, &r->c);
  add64carry_last(c2, Hbb, r->d, &r->d);

  auto c3 = add64carry_first(Lca, r->c, &r->c);
  add64carry_last(c3, Lcb + Hca, r->d + Lda, &r->d);
}

extern "C" __dll_export __hot void
mulset_256x256_512(u512_t *__restrict r, const u256_t *x, const u256_t *y) {
  using namespace erthink;
#define MUL(X, Y)                                                              \
  uint64_t H##X##Y, L##X##Y = mul_64x64_128(x->X, y->Y, &H##X##Y)
  MUL(a, a);
  MUL(a, b);
  MUL(a, c);
  MUL(a, d);
  MUL(b, a);
  MUL(b, b);
  MUL(b, c);
  MUL(b, d);
  MUL(c, a);
  MUL(c, b);
  MUL(c, c);
  MUL(c, d);
  MUL(d, a);
  MUL(d, b);
  MUL(d, c);
  MUL(d, d);
#undef MUL
  //        h   g   f   e   d   c   b   a
  //  a x                 Lad Lac Lab Laa
  //                  Had Hac Hab Haa
  //  b x             Lbd Lbc Lbb Lba
  //              Hbd Hbc Hbb Hba
  //  c x         Lcd Lcc Lcb Lca
  //          Hcd Hcc Hcb Hca
  //  d x     Ldd Ldc Ldb Lda
  //      Hdd Hdc Hdb Hda

  r->a = Laa;

  uint64_t b0, c0, d0, e0, f0, g0;
  auto _0 = add64carry_first(Lab, Haa, &b0);
  _0 = add64carry_next(_0, Lac, Hab, &c0);
  _0 = add64carry_next(_0, Lad, Hac, &d0);
  _0 = add64carry_next(_0, Had, Lbd, &e0);
  _0 = add64carry_next(_0, Hbd, Lcd, &f0);
  _0 = add64carry_next(_0, Hcd, Ldd, &g0) ? 1 : 0;

  uint64_t c1, d1, e1, f1;
  auto _1 = add64carry_first(Lba, b0, &r->b);
  _1 = add64carry_next(_1, Lbb, Hba, &c1);
  _1 = add64carry_next(_1, Lbc, Hbb, &d1);
  _1 = add64carry_next(_1, Hbc, Lcc, &e1);
  _1 = add64carry_next(_1, Hcc, Ldc, &f1);
  _1 = add64carry_next(_1, Hdc, g0, &g0);
  if (_1)
    _0 += 1;

  uint64_t d2, e2;
  auto _2 = add64carry_first(c0, Lca, &c0);
  _2 = add64carry_next(_2, Lcb, Hca, &d2);
  _2 = add64carry_next(_2, Hcb, Ldb, &e2);
  _2 = add64carry_next(_2, f0, Hdb, &f0) ? 1 : 0;

  auto _3 = add64carry_first(c1, c0, &r->c);
  _3 = add64carry_next(_3, d0, Lda, &d0);
  _3 = add64carry_next(_3, e0, Hda, &e0);
  _3 = add64carry_next(_3, f0, f1, &f0);
  if (_3)
    _2 += 1;

  auto _4 = add64carry_first(d1, d2, &d1);
  _4 = add64carry_next(_4, e1, e2, &e1);

  auto _5 = add64carry_first(d0, d1, &r->d);
  _5 = add64carry_next(_5, e0, e1, &r->e);
  _5 = add64carry_next(_5, _4, f0, &r->f);
  _5 = add64carry_next(_5, _2, g0, &r->g);
  add64carry_last(_5, _0, Hdd, &r->h);
}

inline u256_t mul_256(const u256_t &x, const u256_t &y) {
  u256_t xy;
  mulset_256x256_256(&xy, &x, &y);
  return xy;
}

inline u512_t mul_256x256_512(const u256_t &x, const u256_t &y) {
  u512_t xy;
  mulset_256x256_512(&xy, &x, &y);
  return xy;
}

//------------------------------------------------------------------------------

TEST(demo, mul_256x256_256) {
  const u256 x = {{61, 67, 71, 73}};
  const u256 y = {{79, 83, 89, 97}};
  const u256 xy = {{0x12D3, 0x2874, 0x40D7, 0x5BF4}};
  ASSERT_EQ(xy, mul_256(x, y));
  ASSERT_EQ(xy, mul_256(y, x));
}

TEST(mul_demo, u256x256_512) {
  // fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210
  // 00BE768C6FD17F4900655CC73C703B630013E3DD1DE9F1B10029D5041B07BBC5
  //                 fedcba9876543210    fedcba9876543210    fedcba9876543210
  const u256 x = {{0x0029D5041B07BBC5, 0x0013E3DD1DE9F1B1, 0x00655CC73C703B63,
                   0x00BE768C6FD17F49}};

  // fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210
  // 0086A17845E30DFF008E2A7B0B2274FD008C9C281884AD5700A7B54C6A22E2B5
  //                 fedcba9876543210    fedcba9876543210    fedcba9876543210
  const u256 y = {{0x00A7B54C6A22E2B5, 0x008C9C281884AD57, 0x008E2A7B0B2274FD,
                   0x0086A17845E30DFF}};

  // fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210
  // 0000642A2F8C6AB12DDF07EF6C05E68706D9ECA5D5B9B4BBFEA3AEC8D71927DA25879513C3B3EE9AB5826F7887CF4A4C8DF65EE05A27335F04EF0E067265AC49
  //                  fedcba9876543210    fedcba9876543210    fedcba9876543210
  const u512 xy = {{0x04EF0E067265AC49, 0x8DF65EE05A27335F, 0xB5826F7887CF4A4C,
                    0x25879513C3B3EE9A, 0xFEA3AEC8D71927DA, 0x06D9ECA5D5B9B4BB,
                    0x2DDF07EF6C05E687, 0x0000642A2F8C6AB1}};

  ASSERT_EQ(xy, mul_256x256_512(x, y));
  ASSERT_EQ(xy, mul_256x256_512(y, x));
}

//------------------------------------------------------------------------------

runtime_limiter ci_runtime_limiter;

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

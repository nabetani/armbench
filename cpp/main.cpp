#include <array>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <random>
#include <thread>

using namespace std;

using stdclock = chrono::high_resolution_clock;

constexpr int SAMPLE_COUNT = 31;

template <typename u> //
void uint_bench_core(u seed, char const *name) {
  struct u4 {
    u m0, m1, m2, m3;
    u sum() const { return m0 + m1 + m2 + m3; }
  };
  auto m0 = [seed](u4 const &s) -> u4 {
    return {u(s.m1 + s.m2 + seed + 3), //
            u(s.m2 + s.m3 + seed + 5), //
            u(s.m3 + s.m0 + seed + 7), //
            u(s.m0 + s.m1 + seed + 11)};
  };
  auto m1 = [](u4 const &s) -> u4 {
    return {u(s.m1 + s.m2 * s.m3), //
            u(s.m2 + s.m3 * s.m0), //
            u(s.m3 + s.m0 * s.m1), //
            u(s.m0 + s.m1 * s.m2)};
  };
  auto m2 = [](u4 const &s) -> u4 {
    return {u(s.m1 * s.m2 - s.m3), //
            u(s.m2 * s.m3 - s.m0), //
            u(s.m3 * s.m0 - s.m1), //
            u(s.m0 * s.m1 - s.m2)};
  };
  auto m3 = [](u4 const &a, u4 const &b, u4 const &c) -> u4 {
    return {u(a.m1 / u(1u | b.m2)), //
            u(a.m2 / u(1u | b.m3)), //
            u(a.m3 / u(1u | b.m0)), //
            u(a.m0 / u(1u | b.m1))};
  };
  u4 v = {u(seed + 1), u(seed + 2), u(seed + 3), u(seed + 4)};
  atomic<stdclock::time_point> t0 = stdclock::now();
  for (int i = 0; i < 1000000; ++i) {
    v = m3(m0(m1(m2(v))), //
           m1(m2(m0(v))), //
           m2(m0(m1(v))));
  }
  atomic<chrono::nanoseconds> t = stdclock::now() - t0.load();
  auto nano = chrono::duration_cast<chrono::nanoseconds>(t.load()).count();
  printf("%s,%.6f,", name, nano * 1e-6);
  cout << +v.sum() << "," << +seed << endl;
}

template <typename u> //
void uint_bench(u seed, char const *name) {
  for (int ix = 0; ix < SAMPLE_COUNT; ++ix) {
    uint_bench_core<u>((seed + ix) ^ 0x55, name);
  }
}

template <typename f> //
void float_bench_core(f seed, char const *name) {

  struct f4 {
    f m0, m1, m2, m3;
    f sum() const { return m0 + m1 + m2 + m3; }
  };

  auto m0 = [seed](f4 const &a) -> f4 {
    return {f(a.m1 + a.m2 * (seed + f(9))), //
            f(a.m2 + a.m3 * (seed + f(8))), //
            f(a.m3 + a.m0 * (seed + f(7))), //
            f(a.m0 + a.m1 * (seed + f(6)))};
  };
  auto m1 = [seed](f4 const &a) -> f4 {
    return {sin(f(a.m1 * a.m2 + a.m3)), //
            cos(f(a.m2 * a.m3 + a.m0)), //
            sin(f(a.m3 * a.m0 + a.m1)), //
            cos(f(a.m0 * a.m1 + a.m2))};
  };
  auto m2 = [seed](f4 const &a) -> f4 {
    return {f(f(1) / (a.m1 * a.m1 + a.m2 * a.m2 + f(1))), //
            f(f(1) / (a.m2 * a.m2 + a.m3 * a.m3 + f(1))), //
            f(f(1) / (a.m3 * a.m3 + a.m0 * a.m0 + f(1))), //
            f(f(1) / (a.m0 * a.m0 + a.m1 * a.m1 + f(1)))};
  };
  auto m3 = [seed](f4 const &a, f4 const &b, f4 const &c) -> f4 {
    return {f(a.m1 + b.m2 * c.m3), //
            f(a.m2 + b.m3 * c.m0), //
            f(a.m3 + b.m0 * c.m1), //
            f(a.m0 + b.m1 * c.m2)};
  };

  atomic<stdclock::time_point> t0 = stdclock::now();
  f4 v = {f(seed + 1), f(seed + 2), f(seed + 3), f(seed + 4)};
  for (int i = 0; i < 40000; ++i) {
    v = m3(m0(m1(m2(v))), //
           m1(m2(m0(v))), //
           m2(m0(m1(v))));
  }
  atomic<chrono::nanoseconds> t = stdclock::now() - t0.load();
  auto nano = chrono::duration_cast<chrono::nanoseconds>(t.load()).count();
  printf("%s,%.6f,", name, nano * 1e-6);
  cout << +v.sum() << "," << +seed << endl;
}

template <typename f> //
void float_bench(f seed, char const *name) {
  for (int ix = 0; ix < SAMPLE_COUNT; ++ix) {
    float_bench_core<f>(seed * 100 + sin(f(ix + 1)), name);
  }
}

int main(int argc, char const *argv[]) {
  int n = argc < 2 ? 1 : atoi(argv[1]);
  uint_bench<uint8_t>(n, "uint8");
  uint_bench<uint16_t>(n, "uint16");
  uint_bench<uint32_t>(n, "uint32");
  uint_bench<uint64_t>(n, "uint64");
  float_bench<float>(n, "float");
  float_bench<double>(n, "double");
}

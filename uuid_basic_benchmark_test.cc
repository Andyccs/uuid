#include "uuid_basic.h"

#include <benchmark/benchmark.h>
#include <random>

namespace andyccs {

static void BM_BasicUuidV4FromString(benchmark::State &state) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(BasicUuidV4::FromString(from));
    }
  }
}
BENCHMARK(BM_BasicUuidV4FromString)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4FromArrayData(benchmark::State &state) {
  // "FEDCBA98-7654-3210-8899-AABBCCDDEEFF";
  std::uint8_t data[16] = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
                           0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(BasicUuidV4(data));
    }
  }
}
BENCHMARK(BM_BasicUuidV4FromArrayData)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4FromStdArrayData(benchmark::State &state) {
  // "FEDCBA98-7654-3210-8899-AABBCCDDEEFF";
  std::array<std::uint8_t, 16> data = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54,
                                       0x32, 0x10, 0x88, 0x99, 0xAA, 0xBB,
                                       0xCC, 0xDD, 0xEE, 0xFF};
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(BasicUuidV4(data));
    }
  }
}
BENCHMARK(BM_BasicUuidV4FromStdArrayData)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4ToString(benchmark::State &state) {
  BasicUuidV4 uuid(8507, 9486);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(std::string(uuid));
    }
  }
}
BENCHMARK(BM_BasicUuidV4ToString)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4ToStringPrealloc(benchmark::State &state) {
  BasicUuidV4 uuid(8507, 9486);
  std::string result;
  result.resize(36);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      uuid.ToString(result);
      benchmark::DoNotOptimize(result);
    }
  }
}
BENCHMARK(BM_BasicUuidV4ToStringPrealloc)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4ToChars(benchmark::State &state) {
  BasicUuidV4 uuid(8507, 9486);
  char result[37];
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      uuid.ToChars(result);
      benchmark::DoNotOptimize(result);
    }
  }
}
BENCHMARK(BM_BasicUuidV4ToChars)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4GeneratorMt19937(benchmark::State &state) {
  BasicUuidV4Generator<std::mt19937> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator.GenerateUuid());
    }
  }
}
BENCHMARK(BM_BasicUuidV4GeneratorMt19937)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4GeneratorMt19937_64(benchmark::State &state) {
  BasicUuidV4Generator<std::mt19937_64> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator.GenerateUuid());
    }
  }
}
BENCHMARK(BM_BasicUuidV4GeneratorMt19937_64)->Range(1 << 8, 1 << 8);

} // namespace andyccs

BENCHMARK_MAIN();

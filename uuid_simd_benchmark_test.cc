#include "uuid_simd.h"

#include <benchmark/benchmark.h>
#include <random>

#include "uuid_benchmark_utils.h"

namespace andyccs {

static void BM_SimdUuidFromString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  SimdUuid uuid(data);

  std::string from = std::string(uuid);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(SimdUuid::FromString(from));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_SimdUuidFromString)->Range(1 << 8, 1 << 8);

static void BM_SimdUuidFromArrayData(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(SimdUuid(data));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_SimdUuidFromArrayData)->Range(1 << 8, 1 << 8);

static void BM_SimdUuidFromStdArrayData(benchmark::State &state) {
  std::array<std::uint8_t, 16> data;
  GenerateRandomData(data);

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(SimdUuid(data));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_SimdUuidFromStdArrayData)->Range(1 << 8, 1 << 8);

static void BM_SimdUuidToString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  SimdUuid uuid(data);

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(std::string(uuid));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_SimdUuidToString)->Range(1 << 8, 1 << 8);

static void BM_SimdUuidToStringPrealloc(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  SimdUuid uuid(data);

  std::string result;
  result.resize(36);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      uuid.ToString(result);
      benchmark::DoNotOptimize(result);
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_SimdUuidToStringPrealloc)->Range(1 << 8, 1 << 8);

static void BM_SimdUuidToChars(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  SimdUuid uuid(data);

  char result[37];
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      uuid.ToChars(result);
      benchmark::DoNotOptimize(result);
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_SimdUuidToChars)->Range(1 << 8, 1 << 8);

static void BM_SimdUuidGeneratorMt19937(benchmark::State &state) {
  SimdUuidGenerator<std::mt19937> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator.GenerateUuid());
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_SimdUuidGeneratorMt19937)->Range(1 << 8, 1 << 8);

static void BM_SimdUuidGeneratorMt19937_64(benchmark::State &state) {
  SimdUuidGenerator<std::mt19937_64> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator.GenerateUuid());
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_SimdUuidGeneratorMt19937_64)->Range(1 << 8, 1 << 8);

} // namespace andyccs

BENCHMARK_MAIN();

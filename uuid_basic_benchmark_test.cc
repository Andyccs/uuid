#include "uuid_basic.h"

#include <benchmark/benchmark.h>
#include <random>

#include "uuid_benchmark_utils.h"

namespace andyccs {

static void BM_BasicUuidFromString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  BasicUuid uuid(data);
  std::string from = std::string(uuid);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(BasicUuid::FromString(from));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidFromString)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidFromArrayData(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(BasicUuid(data));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidFromArrayData)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidFromStdArrayData(benchmark::State &state) {
  std::array<std::uint8_t, 16> data;
  GenerateRandomData(data);

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(BasicUuid(data));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidFromStdArrayData)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidToString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  BasicUuid uuid(data);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(std::string(uuid));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidToString)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidToStringPrealloc(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  BasicUuid uuid(data);
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
BENCHMARK(BM_BasicUuidToStringPrealloc)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidToChars(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  BasicUuid uuid(data);

  char result[37];
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      uuid.ToChars(result);
      benchmark::DoNotOptimize(result);
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidToChars)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidGeneratorMt19937(benchmark::State &state) {
  BasicUuidGenerator<std::mt19937> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator.GenerateUuid());
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidGeneratorMt19937)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidGeneratorMt19937_64(benchmark::State &state) {
  BasicUuidGenerator<std::mt19937_64> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator.GenerateUuid());
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidGeneratorMt19937_64)->Range(1 << 8, 1 << 8);

} // namespace andyccs

BENCHMARK_MAIN();

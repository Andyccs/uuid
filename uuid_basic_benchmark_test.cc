#include "uuid_basic.h"

#include <benchmark/benchmark.h>
#include <random>

#include "uuid_benchmark_utils.h"

namespace andyccs {

static void BM_BasicUuidV4FromString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  BasicUuidV4 uuid(data);
  std::string from = std::string(uuid);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(BasicUuidV4::FromString(from));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidV4FromString)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4FromArrayData(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(BasicUuidV4(data));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidV4FromArrayData)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4FromStdArrayData(benchmark::State &state) {
  std::array<std::uint8_t, 16> data;
  GenerateRandomData(data);

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(BasicUuidV4(data));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidV4FromStdArrayData)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4ToString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  BasicUuidV4 uuid(data);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(std::string(uuid));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidV4ToString)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4ToStringPrealloc(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  BasicUuidV4 uuid(data);
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
BENCHMARK(BM_BasicUuidV4ToStringPrealloc)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4ToChars(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  BasicUuidV4 uuid(data);

  char result[37];
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      uuid.ToChars(result);
      benchmark::DoNotOptimize(result);
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidV4ToChars)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4GeneratorMt19937(benchmark::State &state) {
  BasicUuidV4Generator<std::mt19937> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator.GenerateUuid());
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidV4GeneratorMt19937)->Range(1 << 8, 1 << 8);

static void BM_BasicUuidV4GeneratorMt19937_64(benchmark::State &state) {
  BasicUuidV4Generator<std::mt19937_64> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator.GenerateUuid());
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BasicUuidV4GeneratorMt19937_64)->Range(1 << 8, 1 << 8);

} // namespace andyccs

BENCHMARK_MAIN();

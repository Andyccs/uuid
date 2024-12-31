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
BENCHMARK(BM_BasicUuidV4FromString)->Range(1, 1 << 8);

static void BM_BasicUuidV4ToString(benchmark::State &state) {
  BasicUuidV4 uuid(8507, 9486);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(std::string(uuid));
    }
  }
}
BENCHMARK(BM_BasicUuidV4ToString)->Range(1, 1 << 8);

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
BENCHMARK(BM_BasicUuidV4ToStringPrealloc)->Range(1, 1 << 8);

static void BM_BasicUuidV4GeneratorMt19937(benchmark::State &state) {
  BasicUuidV4Generator<std::mt19937> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator.GenerateUuid());
    }
  }
}
BENCHMARK(BM_BasicUuidV4GeneratorMt19937)->Range(1, 1 << 8);

static void BM_BasicUuidV4GeneratorMt19937_64(benchmark::State &state) {
  BasicUuidV4Generator<std::mt19937_64> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator.GenerateUuid());
    }
  }
}
BENCHMARK(BM_BasicUuidV4GeneratorMt19937_64)->Range(1, 1 << 8);

} // namespace andyccs

BENCHMARK_MAIN();

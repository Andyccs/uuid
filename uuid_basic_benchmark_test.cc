#include "uuid_basic.h"

#include <benchmark/benchmark.h>
#include <random>

namespace andyccs {

namespace {

// Generate random data each time to prevent the compiler from optimizing the
// code to merely load and store constant values in the char array. This
// ensures that the benchmark accurately measures the performance of the
// to_chars function.
template <typename T> void GenerateRandomData(T &data) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<std::uint8_t> dis(0, 255);
  for (auto &byte : data) {
    byte = dis(gen);
  }
}

} // namespace

static void BM_BasicUuidV4FromString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  BasicUuidV4 uuid(data);
  std::string from = std::string(uuid);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(BasicUuidV4::FromString(from));
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

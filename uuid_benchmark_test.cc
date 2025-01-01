#include "boost/uuid/basic_random_generator.hpp"
#include "boost/uuid/string_generator.hpp"
#include <benchmark/benchmark.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
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

static void BM_BoostUuidV4FromString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  boost::uuids::uuid uuid(data);
  std::string from = boost::uuids::to_string(uuid);

  boost::uuids::string_generator gen;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(gen(from));
    }
  }
}
BENCHMARK(BM_BoostUuidV4FromString)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidV4FromArrayData(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(boost::uuids::uuid(data));
    }
  }
}
BENCHMARK(BM_BoostUuidV4FromArrayData)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidV4ToString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  boost::uuids::uuid uuid(data);

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(boost::uuids::to_string(uuid));
    }
  }
}
BENCHMARK(BM_BoostUuidV4ToString)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidV4ToChars(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  boost::uuids::uuid uuid(data);

  char str[37];
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(boost::uuids::to_chars<char, 37>(uuid, str));
    }
  }
}
BENCHMARK(BM_BoostUuidV4ToChars)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidV4GeneratorMt19937(benchmark::State &state) {
  boost::uuids::basic_random_generator<std::mt19937> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator());
    }
  }
}
BENCHMARK(BM_BoostUuidV4GeneratorMt19937)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidV4GeneratorMt19937_64(benchmark::State &state) {
  boost::uuids::basic_random_generator<std::mt19937_64> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator());
    }
  }
}
BENCHMARK(BM_BoostUuidV4GeneratorMt19937_64)->Range(1 << 8, 1 << 8);

} // namespace andyccs

BENCHMARK_MAIN();

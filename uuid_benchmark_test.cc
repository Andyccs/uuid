#include "boost/uuid/basic_random_generator.hpp"
#include "boost/uuid/string_generator.hpp"
#include <benchmark/benchmark.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <random>

#include "uuid_benchmark_utils.h"
#include "vendor/yesmey/UUID.h"

namespace andyccs {

static void BM_BoostUuidFromString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  boost::uuids::uuid uuid(data);
  std::string from = boost::uuids::to_string(uuid);

  boost::uuids::string_generator gen;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(gen(from));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BoostUuidFromString)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidFromArrayData(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(boost::uuids::uuid(data));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BoostUuidFromArrayData)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidToString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  boost::uuids::uuid uuid(data);

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(boost::uuids::to_string(uuid));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BoostUuidToString)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidToChars(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  boost::uuids::uuid uuid(data);

  char str[37];
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(boost::uuids::to_chars<char, 37>(uuid, str));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BoostUuidToChars)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidGeneratorMt19937(benchmark::State &state) {
  boost::uuids::basic_random_generator<std::mt19937> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator());
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BoostUuidGeneratorMt19937)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidGeneratorMt19937_64(benchmark::State &state) {
  boost::uuids::basic_random_generator<std::mt19937_64> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator());
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BoostUuidGeneratorMt19937_64)->Range(1 << 8, 1 << 8);

static void BM_MeyrUuidFromString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  std::string from = boost::uuids::to_string(boost::uuids::uuid(data));

  meyr::UUID meyr_uuid;
  benchmark::DoNotOptimize(meyr_uuid);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(meyr_uuid.try_parse(from));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_MeyrUuidFromString)->Range(1 << 8, 1 << 8);

static void BM_MeyrUuidToString(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);
  boost::uuids::uuid uuid(data);
  std::string from = boost::uuids::to_string(uuid);

  meyr::UUID meyr_uuid;
  if (!meyr_uuid.try_parse(from)) {
    state.SkipWithError("Error parsing UUID");
  }
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(meyr_uuid.to_string('D'));
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_MeyrUuidToString)->Range(1 << 8, 1 << 8);

} // namespace andyccs

BENCHMARK_MAIN();

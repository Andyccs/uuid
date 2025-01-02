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

static void BM_BoostUuidV4FromString(benchmark::State &state) {
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
BENCHMARK(BM_BoostUuidV4FromString)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidV4FromArrayData(benchmark::State &state) {
  std::uint8_t data[16];
  GenerateRandomData(data);

  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(boost::uuids::uuid(data));
      benchmark::ClobberMemory();
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
      benchmark::ClobberMemory();
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
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BoostUuidV4ToChars)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidV4GeneratorMt19937(benchmark::State &state) {
  boost::uuids::basic_random_generator<std::mt19937> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator());
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BoostUuidV4GeneratorMt19937)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidV4GeneratorMt19937_64(benchmark::State &state) {
  boost::uuids::basic_random_generator<std::mt19937_64> generator;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(generator());
      benchmark::ClobberMemory();
    }
  }
}
BENCHMARK(BM_BoostUuidV4GeneratorMt19937_64)->Range(1 << 8, 1 << 8);

static void BM_MeyrUuidV4FromString(benchmark::State &state) {
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
BENCHMARK(BM_MeyrUuidV4FromString)->Range(1 << 8, 1 << 8);

static void BM_MeyrUuidV4ToString(benchmark::State &state) {
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
BENCHMARK(BM_MeyrUuidV4ToString)->Range(1 << 8, 1 << 8);

} // namespace andyccs

BENCHMARK_MAIN();

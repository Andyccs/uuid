#include "boost/uuid/basic_random_generator.hpp"
#include "boost/uuid/string_generator.hpp"
#include <benchmark/benchmark.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <random>

namespace andyccs {

static void BM_BoostUuidV4FromString(benchmark::State &state) {
  std::string from = "FEDCBA98-7654-3210-8899-AABBCCDDEEFF";
  boost::uuids::string_generator gen;
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(gen(from));
    }
  }
}
BENCHMARK(BM_BoostUuidV4FromString)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidV4FromArrayData(benchmark::State &state) {
  // "FEDCBA98-7654-3210-8899-AABBCCDDEEFF";
  std::uint8_t data[16] = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
                           0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(boost::uuids::uuid(data));
    }
  }
}
BENCHMARK(BM_BoostUuidV4FromArrayData)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidV4ToString(benchmark::State &state) {
  std::uint8_t data[16] = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
                           0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  boost::uuids::uuid uuid(data);
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(boost::uuids::to_string(uuid));
    }
  }
}
BENCHMARK(BM_BoostUuidV4ToString)->Range(1 << 8, 1 << 8);

static void BM_BoostUuidV4ToStringPrealloc(benchmark::State &state) {
  std::uint8_t data[16] = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
                           0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  boost::uuids::uuid uuid(data);

  char str[37];
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); ++i) {
      benchmark::DoNotOptimize(boost::uuids::to_chars<char, 37>(uuid, str));
    }
  }
}
BENCHMARK(BM_BoostUuidV4ToStringPrealloc)->Range(1 << 8, 1 << 8);

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

# Usages

```
#include "uuid_basic.h"
#include "uuid_simd.h"

#include <random>

int main() {
  andyccs::BasicUuidV4Generator<std::mt19937_64> generator;

  // Generate a arandom BasicUuidV4
  andyccs::BasicUuidV4 uuid_1 = generator.GenerateUuid();
  std::cout << "BasicUuidV4 1: " << std::string(uuid_1) << std::endl;

  // Construct a BasicUuidV4
  andyccs::BasicUuidV4 uuid_2(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  std::cout << "BasicUuidV4 2: " << std::string(uuid_2) << std::endl;
  andyccs::BasicUuidV4Generator<std::mt19937_64> generator;

  // Generate a random SimdUuidV4Generator
  andyccs::SimdUuidV4Generator<std::mt19937_64> generator;
  andyccs::SimdUuidV4 uuid_3 = generator.GenerateUuid();
  std::cout << "SimdUuidV4 3: " << std::string(uuid_3) << std::endl;

  // Construct a SimdUuidV4
  andyccs::SimdUuidV4 uuid_4(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  std::cout << "SimdUuidV4 4: " << std::string(uuid_4) << std::endl;

  return 0;
}
```

## Benchmark Results

```
2024-12-29T20:59:03-08:00
Running ./uuid_basic_benchmark_test
Run on (32 X 3187.2 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x16)
  L1 Instruction 32 KiB (x16)
  L2 Unified 2048 KiB (x16)
  L3 Unified 36864 KiB (x1)
Load Average: 0.10, 0.05, 0.01
--------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations
--------------------------------------------------------------------------------
BM_BasicUuidV4FromString/256                4934 ns         4765 ns       147121
BM_BasicUuidV4FromArrayData/256              113 ns          111 ns      6283132
BM_BasicUuidV4FromStdArrayData/256          50.4 ns         51.8 ns     13366355
BM_BasicUuidV4ToString/256                  3504 ns         3599 ns       193746
BM_BasicUuidV4ToStringPrealloc/256          1204 ns         1236 ns       567258
BM_BasicUuidV4ToChars/256                   1070 ns         1099 ns       636219
BM_BasicUuidV4GeneratorMt19937/256          2890 ns         3083 ns       225420
BM_BasicUuidV4GeneratorMt19937_64/256        878 ns          936 ns       754271

-------------------------------------------------------------------------------
Benchmark                                     Time             CPU   Iterations
-------------------------------------------------------------------------------
BM_SimdUuidV4FromString/256                 576 ns          593 ns      1165466
BM_SimdUuidV4FromArrayData/256              152 ns          156 ns      4522420
BM_SimdUuidV4ToString/256                  2319 ns         2387 ns       289077
BM_SimdUuidV4ToStringPrealloc/256           399 ns          411 ns      1684737
BM_SimdUuidV4ToChars/256                    343 ns          353 ns      1981980
BM_SimdUuidV4GeneratorMt19937/256          2593 ns         2669 ns       262181
BM_SimdUuidV4GeneratorMt19937_64/256        535 ns          551 ns      1277668

--------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations
--------------------------------------------------------------------------------
BM_BoostUuidV4FromString/256               25671 ns        25447 ns        27283
BM_BoostUuidV4FromArrayData/256             48.7 ns         50.0 ns     13909588
BM_BoostUuidV4ToString/256                  1453 ns         1493 ns       463779
BM_BoostUuidV4ToChars/256                    171 ns          176 ns      3981865
BM_BoostUuidV4GeneratorMt19937/256          2331 ns         2415 ns       290862
BM_BoostUuidV4GeneratorMt19937_64/256        978 ns         1013 ns       683980
```

# Development

## Build using CMAKE

```shell
# From root directory
mkdir build
cd build
cmake ../
cmake --build .
./Andyccs
```

## Rebuild

```shell
cmake --build . && ./Andyccs
```

## Unit Tests

```shell
cmake --build . && ctest --rerun-failed --output-on-failure
```

## Benchmark

```shell
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . && ./uuid_basic_benchmark_test
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . && ./uuid_simd_benchmark_test
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . && ./uuid_benchmark_test
```

## Clean up

```shell
# From root directory
rm -r build/
```

## TODO

- Override stream operators
- Benchmarking comparisons with other libraries
- Thread safe generators

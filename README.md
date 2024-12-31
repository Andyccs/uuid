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
BM_BasicUuidV4FromString/256                5340 ns         5340 ns       134485
BM_BasicUuidV4ToString/256                  4348 ns         4348 ns       153613
BM_BasicUuidV4ToStringPrealloc/256          1986 ns         1986 ns       351988
BM_BasicUuidV4GeneratorMt19937/256          2741 ns         2741 ns       255379
BM_BasicUuidV4GeneratorMt19937_64/256        555 ns          555 ns      1270523

-------------------------------------------------------------------------------
Benchmark                                     Time             CPU   Iterations
-------------------------------------------------------------------------------
BM_SimdUuidV4FromString/256                 588 ns          588 ns      1172408
BM_SimdUuidV4ToString/256                  3288 ns         3288 ns       214730
BM_SimdUuidV4ToStringPrealloc/256          1222 ns         1222 ns       562022
BM_SimdUuidV4GeneratorMt19937/256          2685 ns         2685 ns       264501
BM_SimdUuidV4GeneratorMt19937_64/256        543 ns          543 ns      1298150
--------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations
--------------------------------------------------------------------------------
BM_BoostUuidV4FromString/256               25102 ns        25102 ns        27800
BM_BoostUuidV4FromData/256                  49.4 ns         49.4 ns     14248729
BM_BoostUuidV4ToString/256                  1484 ns         1484 ns       474523
BM_BoostUuidV4ToStringPrealloc/256           175 ns          175 ns      3919590
BM_BoostUuidV4GeneratorMt19937/256          2393 ns         2393 ns       285523
BM_BoostUuidV4GeneratorMt19937_64/256       1003 ns         1003 ns       696179
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

- Experiment with `uint8_t data[16]` instead of two `unint64_t`
- Override stream operators
- Benchmarking comparisons with other libraries
- Thread safe generators

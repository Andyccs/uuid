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
BM_BasicUuidV4FromString/256                4888 ns         5011 ns       137240
BM_BasicUuidV4FromArrayData/256              113 ns          116 ns      6121525
BM_BasicUuidV4FromStdArrayData/256          49.2 ns         50.4 ns     13808449
BM_BasicUuidV4ToString/256                  4063 ns         4165 ns       166739
BM_BasicUuidV4ToStringPrealloc/256          1868 ns         1915 ns       369032
BM_BasicUuidV4GeneratorMt19937/256          3013 ns         3088 ns       228029
BM_BasicUuidV4GeneratorMt19937_64/256        900 ns          923 ns       754815

-------------------------------------------------------------------------------
Benchmark                                     Time             CPU   Iterations
-------------------------------------------------------------------------------
BM_SimdUuidV4FromString/256                 588 ns          588 ns      1181425
BM_SimdUuidV4FromArrayData/256              159 ns          158 ns      4386427
BM_SimdUuidV4ToString/256                  3136 ns         3138 ns       225226
BM_SimdUuidV4ToStringPrealloc/256           500 ns          502 ns      1000000
BM_SimdUuidV4GeneratorMt19937/256          2723 ns         2666 ns       263826
BM_SimdUuidV4GeneratorMt19937_64/256        560 ns          550 ns      1257505

--------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations
--------------------------------------------------------------------------------
BM_BoostUuidV4FromString/256               25102 ns        25102 ns        27800
BM_BoostUuidV4FromArrayData/256             49.4 ns         49.4 ns     14248729
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

- Override stream operators
- Benchmarking comparisons with other libraries
- Thread safe generators

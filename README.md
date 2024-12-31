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
BM_BasicUuidV4FromString/1                  18.1 ns         18.1 ns     38352256
BM_BasicUuidV4FromString/8                   154 ns          154 ns      4485926
BM_BasicUuidV4FromString/64                 1156 ns         1156 ns       599144
BM_BasicUuidV4FromString/256                4622 ns         4622 ns       152093
BM_BasicUuidV4ToString/1                    16.8 ns         16.8 ns     41446016
BM_BasicUuidV4ToString/8                     141 ns          141 ns      4960644
BM_BasicUuidV4ToString/64                   1067 ns         1067 ns       653297
BM_BasicUuidV4ToString/256                  4230 ns         4230 ns       165194
BM_BasicUuidV4ToStringPrealloc/1            7.60 ns         7.60 ns     91979638
BM_BasicUuidV4ToStringPrealloc/8            61.4 ns         61.4 ns     11581063
BM_BasicUuidV4ToStringPrealloc/64            498 ns          498 ns      1406233
BM_BasicUuidV4ToStringPrealloc/256          1938 ns         1938 ns       356535
BM_BasicUuidV4GeneratorMt19937/1            10.4 ns         10.4 ns     66995327
BM_BasicUuidV4GeneratorMt19937/8            90.9 ns         90.9 ns      7886391
BM_BasicUuidV4GeneratorMt19937/64            688 ns          688 ns      1019944
BM_BasicUuidV4GeneratorMt19937/256          2672 ns         2672 ns       260126
BM_BasicUuidV4GeneratorMt19937_64/1         2.29 ns         2.29 ns    308805501
BM_BasicUuidV4GeneratorMt19937_64/8         16.2 ns         16.2 ns     43028149
BM_BasicUuidV4GeneratorMt19937_64/64         140 ns          140 ns      4948521
BM_BasicUuidV4GeneratorMt19937_64/256        543 ns          543 ns      1271851

-------------------------------------------------------------------------------
Benchmark                                     Time             CPU   Iterations
-------------------------------------------------------------------------------
BM_SimdUuidV4FromString/1                  2.22 ns         2.26 ns    309218010
BM_SimdUuidV4FromString/8                  19.0 ns         19.4 ns     36295890
BM_SimdUuidV4FromString/64                  146 ns          149 ns      4688553
BM_SimdUuidV4FromString/256                 562 ns          574 ns      1189305
BM_SimdUuidV4ToString/1                    15.0 ns         15.3 ns     45415353
BM_SimdUuidV4ToString/8                     109 ns          111 ns      6266528
BM_SimdUuidV4ToString/64                    823 ns          840 ns       815478
BM_SimdUuidV4ToString/256                  3306 ns         3377 ns       205805
BM_SimdUuidV4ToStringPrealloc/1            2.05 ns         2.09 ns    334532073
BM_SimdUuidV4ToStringPrealloc/8            21.8 ns         22.2 ns     31217890
BM_SimdUuidV4ToStringPrealloc/64            129 ns          132 ns      5208097
BM_SimdUuidV4ToStringPrealloc/256           490 ns          501 ns      1408739
BM_SimdUuidV4GeneratorMt19937/1            10.2 ns         10.5 ns     66243215
BM_SimdUuidV4GeneratorMt19937/8            88.1 ns         89.9 ns      7784317
BM_SimdUuidV4GeneratorMt19937/64            664 ns          677 ns      1039399
BM_SimdUuidV4GeneratorMt19937/256          2618 ns         2673 ns       260201
BM_SimdUuidV4GeneratorMt19937_64/1         2.25 ns         2.29 ns    307684328
BM_SimdUuidV4GeneratorMt19937_64/8         15.9 ns         16.2 ns     43205396
BM_SimdUuidV4GeneratorMt19937_64/64         138 ns          141 ns      4957468
BM_SimdUuidV4GeneratorMt19937_64/256        536 ns          547 ns      1244086
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

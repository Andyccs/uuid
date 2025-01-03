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
BM_BasicUuidV4FromString/256                4221 ns         3889 ns       173221
BM_BasicUuidV4FromArrayData/256              113 ns          116 ns      6110021
BM_BasicUuidV4FromStdArrayData/256          82.5 ns         84.6 ns      8070011
BM_BasicUuidV4ToString/256                  2752 ns         2823 ns       244132
BM_BasicUuidV4ToStringPrealloc/256          1197 ns         1228 ns       555348
BM_BasicUuidV4ToChars/256                   1101 ns         1130 ns       619016
BM_BasicUuidV4GeneratorMt19937/256          2636 ns         2705 ns       259139
BM_BasicUuidV4GeneratorMt19937_64/256        528 ns          542 ns      1298976

-------------------------------------------------------------------------------
Benchmark                                     Time             CPU   Iterations
-------------------------------------------------------------------------------
BM_SimdUuidV4FromString/256                 443 ns          458 ns      1511570**
BM_SimdUuidV4FromArrayData/256              109 ns          113 ns      6265002
BM_SimdUuidV4FromStdArrayData/256          50.4 ns         52.2 ns     13250047**
BM_SimdUuidV4ToString/256                  1793 ns         1855 ns       376358**
BM_SimdUuidV4ToStringPrealloc/256           369 ns          382 ns      1836975**
BM_SimdUuidV4ToChars/256                    308 ns          319 ns      2203280**
BM_SimdUuidV4GeneratorMt19937/256          2706 ns         2804 ns       249540
BM_SimdUuidV4GeneratorMt19937_64/256        532 ns          545 ns      1298896**

--------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations
--------------------------------------------------------------------------------
BM_BoostUuidV4FromString/256               23046 ns        23663 ns        28821
BM_BoostUuidV4FromArrayData/256             48.1 ns         49.4 ns     14121043**
BM_BoostUuidV4ToString/256                  2396 ns         2461 ns       286785
BM_BoostUuidV4ToChars/256                    851 ns          874 ns       802192
BM_BoostUuidV4GeneratorMt19937/256          2202 ns         2262 ns       304858**
BM_BoostUuidV4GeneratorMt19937_64/256        980 ns         1007 ns       697156

BM_MeyrUuidV4FromString/256                  468 ns          488 ns      1403036
BM_MeyrUuidV4ToString/256                   2829 ns         2951 ns       229226
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
- Override comparator operators
- Benchmarking comparisons with other libraries
  - https://github.com/mariusbancila/stduuid
  - https://github.com/libstud/libstud-uuid
- Thread safe generators
- RFC 9562 Compliant
- Other UUID version
- Easy adoption by other project
- Portable class and method

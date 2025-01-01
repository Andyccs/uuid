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
BM_BasicUuidV4FromString/256                4784 ns         4915 ns       142807
BM_BasicUuidV4FromArrayData/256              113 ns          116 ns      6047349
BM_BasicUuidV4FromStdArrayData/256          44.8 ns         46.1 ns     15267399**
BM_BasicUuidV4ToString/256                  2791 ns         2847 ns       243260
BM_BasicUuidV4ToStringPrealloc/256          1191 ns         1220 ns       570493
BM_BasicUuidV4ToChars/256                   1080 ns         1107 ns       630965
BM_BasicUuidV4GeneratorMt19937/256          3077 ns         3153 ns       223853
BM_BasicUuidV4GeneratorMt19937_64/256        923 ns          946 ns       728672

-------------------------------------------------------------------------------
Benchmark                                     Time             CPU   Iterations
-------------------------------------------------------------------------------
BM_SimdUuidV4FromString/256                 573 ns          583 ns      1182113**
BM_SimdUuidV4FromArrayData/256              110 ns          112 ns      6347128
BM_SimdUuidV4FromStdArrayData/256          84.7 ns         86.3 ns      8285210
BM_SimdUuidV4ToString/256                  1808 ns         1854 ns       378505**
BM_SimdUuidV4ToStringPrealloc/256           371 ns          381 ns      1784369**
BM_SimdUuidV4ToChars/256                    298 ns          305 ns      2289882**
BM_SimdUuidV4GeneratorMt19937/256          3139 ns         3219 ns       213085
BM_SimdUuidV4GeneratorMt19937_64/256        910 ns          933 ns       762352**

--------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations
--------------------------------------------------------------------------------
BM_BoostUuidV4FromString/256               22057 ns        22829 ns        29817
BM_BoostUuidV4FromArrayData/256             48.9 ns         50.6 ns     14161607**
BM_BoostUuidV4ToString/256                  2274 ns         2446 ns       281807
BM_BoostUuidV4ToChars/256                    838 ns          901 ns       791324
BM_BoostUuidV4GeneratorMt19937/256          2090 ns         2248 ns       307998**
BM_BoostUuidV4GeneratorMt19937_64/256        947 ns         1019 ns       694793
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
- RFC 9562 Compliant
- Other UUID version
- Easy adoption by other project
- Portable class and method

#include "AndyccsConfig.h"
#include "uuid_basic.h"
#include "uuid_simd.h"

#include <immintrin.h>
#include <iostream>
#include <random>
#include <stdint.h>
#include <string>

int main() {
  // Generate a random BasicUuidV4
  andyccs::BasicUuidV4Generator<std::mt19937_64> generator_1;
  andyccs::BasicUuidV4 basic_uuid_1 = generator_1.GenerateUuid();
  std::cout << "BasicUuidV4 1: " << std::string(basic_uuid_1) << std::endl;
  // BasicUuidV4 1: 6EE37CFB-05E3-3CD7-D6A2-7B68F43C6E52

  // Construct a BasicUuidV4
  andyccs::BasicUuidV4 basic_uuid_2(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  std::cout << "BasicUuidV4 2: " << std::string(basic_uuid_2) << std::endl;
  // BasicUuidV4 2: FEDCBA98-7654-3210-8899-AABBCCDDEEFF

  // Construct a BasicUuidV4 from string
  std::optional<andyccs::BasicUuidV4> basic_uuid_3 =
      andyccs::BasicUuidV4::FromString("FEDCBA98-7654-3210-8899-AABBCCDDEEFF");
  std::cout << "BasicUuidV4 3: " << std::string(*basic_uuid_3) << std::endl;
  // BasicUuidV4 3: FEDCBA98-7654-3210-8899-AABBCCDDEEFF

  // Generate a random SimdUuidV4
  andyccs::SimdUuidV4Generator<std::mt19937_64> generator_2;
  andyccs::SimdUuidV4 simd_uuid_1 = generator_2.GenerateUuid();
  std::cout << "SimdUuidV4 1: " << std::string(simd_uuid_1) << std::endl;
  // SimdUuidV4 1: 7DA2E470-5228-EF80-9BD6-B33D98445578

  // Construct a SimdUuidV4
  andyccs::SimdUuidV4 simd_uuid_2(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  std::cout << "SimdUuidV4 2: " << std::string(simd_uuid_2) << std::endl;
  // SimdUuidV4 2: FEDCBA98-7654-3210-8899-AABBCCDDEEFF

  // Construct a BasicUuidV4 from string
  std::optional<andyccs::SimdUuidV4> simd_uuid_3 =
      andyccs::SimdUuidV4::FromString("FEDCBA98-7654-3210-8899-AABBCCDDEEFF");
  std::cout << "SimdUuidV4 3: " << std::string(*simd_uuid_3) << std::endl;
  // SimdUuidV4 3: FEDCBA98-7654-3210-8899-AABBCCDDEEFF

  return 0;
}

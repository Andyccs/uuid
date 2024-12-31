#include "AndyccsConfig.h"
#include "uuid_basic.h"
#include "uuid_simd.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <immintrin.h>
#include <iostream>
#include <random>
#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <unordered_map>

uint64_t convert_to_uint64(const uint8_t *array) {
  uint64_t result = 0;
  for (int i = 0; i < 8; ++i) {
    result |= (uint64_t)array[i] << (8 * (7 - i));
  }
  return result;
}

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

  std::unordered_map<andyccs::BasicUuidV4, std::string> my_map;

  // auto generator = boost::uuids::random_generator();
  boost::uuids::basic_random_generator<std::mt19937> generator;
  boost::uuids::uuid boost_uuid_1 = generator();
  std::cout << "Boost UUID 1: " << boost_uuid_1 << std::endl;

  std::uint8_t data[16] = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
                           0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  boost::uuids::uuid boost_uuid_2(data);
  std::cout << "Boost UUID 2: " << boost_uuid_2 << std::endl;

  char str[37];
  boost::uuids::to_chars<char, 37>(boost_uuid_2, str);
  std::cout << "Boost UUID 3: " << std::string(str) << std::endl;

  std::string from = "FEDCBA98-7654-3210-8899-AABBCCDDEEFF";
  boost::uuids::string_generator gen;
  boost::uuids::uuid boost_uuid_4 = gen(from);
  std::cout << "Boost UUID 4: " << boost_uuid_4 << std::endl;

  uint64_t high = convert_to_uint64(data);
  uint64_t low = convert_to_uint64(&data[8]);
  printf("%lX %lX\n", high, low);

  return 0;
}

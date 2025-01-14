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

#include "AndyccsConfig.h"
#include "uuid_basic.h"
#include "uuid_simd.h"
#include "vendor/yesmey/UUID.h"

uint64_t convert_to_uint64(const uint8_t *array) {
  uint64_t result = 0;
  for (int i = 0; i < 8; ++i) {
    result |= (uint64_t)array[i] << (8 * (7 - i));
  }
  return result;
}

void print128_num(__m128i var, std::string_view description) {
  uint16_t val[8];
  memcpy(val, &var, sizeof(val));
  printf("%s:\t%04X%04X %04X%04X %04X%04X %04X%04X \n", description.data(),
         val[7], val[6], val[5], val[4], val[3], val[2], val[1], val[0]);
}

void print256_num(__m256i var, std::string_view description) {
  uint16_t val[16];
  memcpy(val, &var, sizeof(val));
  printf("%s:\t%04X%04X %04X%04X %04X%04X %04X%04X %04X%04X %04X%04X %04X%04X "
         "%04X%04X \n",
         description.data(), val[15], val[14], val[13], val[12], val[11],
         val[10], val[9], val[8], val[7], val[6], val[5], val[4], val[3],
         val[2], val[1], val[0]);
}

int main() {
  // Generate a random BasicUuid
  andyccs::BasicUuidGenerator<std::mt19937_64> generator_1;
  andyccs::BasicUuid basic_uuid_1 = generator_1.GenerateUuid();
  std::cout << "BasicUuid 1: " << std::string(basic_uuid_1) << std::endl;
  // BasicUuid 1: 6EE37CFB-05E3-3CD7-D6A2-7B68F43C6E52

  // Construct a BasicUuid
  andyccs::BasicUuid basic_uuid_2(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  std::cout << "BasicUuid 2: " << std::string(basic_uuid_2) << std::endl;
  // BasicUuid 2: FEDCBA98-7654-3210-8899-AABBCCDDEEFF

  // Construct a BasicUuid from string
  std::optional<andyccs::BasicUuid> basic_uuid_3 =
      andyccs::BasicUuid::FromString("FEDCBA98-7654-3210-8899-AABBCCDDEEFF");
  std::cout << "BasicUuid 3: " << std::string(*basic_uuid_3) << std::endl;
  // BasicUuid 3: FEDCBA98-7654-3210-8899-AABBCCDDEEFF

  // Generate a random SimdUuid
  andyccs::SimdUuidGenerator<std::mt19937_64> generator_2;
  andyccs::SimdUuid simd_uuid_1 = generator_2.GenerateUuid();
  std::cout << "SimdUuid 1: " << std::string(simd_uuid_1) << std::endl;
  // SimdUuid 1: 7DA2E470-5228-EF80-9BD6-B33D98445578

  // Construct a SimdUuid
  andyccs::SimdUuid simd_uuid_2(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  std::cout << "SimdUuid 2: " << std::string(simd_uuid_2) << std::endl;
  // SimdUuid 2: FEDCBA98-7654-3210-8899-AABBCCDDEEFF

  // Construct a BasicUuid from string
  std::optional<andyccs::SimdUuid> simd_uuid_3 =
      andyccs::SimdUuid::FromString("FEDCBA98-7654-3210-8899-AABBCCDDEEFF");
  std::cout << "SimdUuid 3: " << std::string(*simd_uuid_3) << std::endl;
  // SimdUuid 3: FEDCBA98-7654-3210-8899-AABBCCDDEEFF

  std::unordered_map<andyccs::BasicUuid, std::string> my_map;

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

  meyr::UUID meyr_uuid_1;
  meyr_uuid_1.try_parse("FEDCBA98-7654-3210-8899-AABBCCDDEEFF");
  std::cout << "MEYR UUID 1: " << meyr_uuid_1.to_string('D') << std::endl;

  return 0;
}

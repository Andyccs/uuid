#include "AndyccsConfig.h"
#include "uuid_basic.h"
#include "uuid_simd.h"

#include <immintrin.h>
#include <iostream>
#include <random>
#include <stdint.h>
#include <string.h>
#include <string>

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
  const double input_value = std::stod("1.5");
  std::cout << "Hello, World! " << std::endl;
  std::cout << "Input value: " << input_value << std::endl;
  std::cout << "Version: " << ANDYCCS_VERSION_MAJOR << "."
            << ANDYCCS_VERSION_MINOR << std::endl;

  andyccs::BasicUuidV4Generator<std::mt19937_64> generator;
  andyccs::BasicUuidV4 uuid_v4 = generator.GenerateUuid();
  std::cout << "UUID: " << std::string(uuid_v4) << std::endl;

  std::string from = "FEDCBA98-7654-3210-8899-AABBCCDDEEFF";
  const char *mem = from.data();

  // Remove dashes and pack hex ascii bytes in a 256-bits int
  const __m256i dash_shuffle =
      _mm256_set_epi32(0x80808080, 0x0f0e0d0c, 0x0b0a0908, 0x06050403,
                       0x80800f0e, 0x0c0b0a09, 0x07060504, 0x03020100);

  // input:
  // 46464545 44444343 42424141 39393838 30313233 34353637 38394142 43444546
  __m256i x = _mm256_loadu_si256((__m256i *)mem);
  x = _mm256_shuffle_epi8(x, dash_shuffle);
  x = _mm256_insert_epi16(x, *(uint16_t *)(mem + 16), 7);
  x = _mm256_insert_epi32(x, *(uint32_t *)(mem + 32), 7);

  const __m256i pretty_shuffle =
      _mm256_set_epi32(0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f,
                       0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f);
  x = _mm256_shuffle_epi8(x, pretty_shuffle);
  print256_num(x, "x");

  // Build a mask to apply a different offset to alphas and digits
  const __m256i sub = _mm256_set1_epi8(0x30);

  // mask to determine whether it is a alpha
  const __m256i mask = _mm256_set1_epi8(0x10);

  // 'A' -> 0x11
  // 0x11 - 0x07 = 0x0A
  // So the offset for alpha ix 0x07
  const __m256i alpha_offset = _mm256_set1_epi8(0x07);

  // No offset for digit.
  const __m256i digits_offset = _mm256_set1_epi8(0x00);

  // 46464545 44444343 42424141 39393838 30313233 34353637 38394142 43444546
  // 30303030 30303030 30303030 30303030 30303030 30303030 30303030 30303030 SUB
  // 16161515 14141313 12121111 09090808 00010203 04050607 08091112 13141516
  __m256i a = _mm256_sub_epi8(x, sub);
  print256_num(a, "a");

  // alpha: Which bytes are alpha
  // 16161515 14141313 12121111 09090808 00010203 04050607 08091112 13141516
  // 10101010 10101010 10101010 10101010 10101010 10101010 10101010 10101010 AND
  // SHIFT LEFT 3
  // 80808080 80808080 80808080 00000000 00000000 00000000 00008080 80808080
  __m256i alpha = _mm256_slli_epi64(_mm256_and_si256(a, mask), 3);
  print256_num(alpha, "alpha");

  // sub_mask: Subtraction mask. What should b subtracted from each byte.
  // 07070707 07070707 07070707 00000000 00000000 00000000 00000707 07070707
  __m256i sub_mask = _mm256_blendv_epi8(digits_offset, alpha_offset, alpha);
  print256_num(sub_mask, "sub_ma");

  // spaced_result: Almost the result, but there is a 0x0 space in between
  // 16161515 14141313 12121111 09090808 00010203 04050607 08091112 13141516
  // 0F0F0E0E 0D0D0C0C 0B0B0A0A 09090808 00010203 04050607 08090A0B 0C0D0E0F SUB
  // 0F0F0E0E 0D0D0C0C 0B0B0A0A 09090808 00010203 04050607 08090A0B 0C0D0E0F
  __m256i spaced_result = _mm256_sub_epi8(a, sub_mask);
  print256_num(spaced_result, "spaced_result");

  // 00880099 00AA00BB 00CC00DD 00EE00FF 00FE00DC 00BA0098 00760054 00320010
  const __m256i mask_2 = _mm256_set1_epi16(0x00FF);
  __m256i spaced_result_intermediate = _mm256_and_si256(
      _mm256_or_si256(spaced_result, _mm256_srli_epi16(spaced_result, 4)),
      mask_2);
  print256_num(spaced_result_intermediate, "spaced_result_intermediate");

  // 00000099 000000BB 000000DD 000000FF 000000DC 00000098 00000054 00000010
  const __m256i mask_3 = _mm256_set1_epi32(0x000000FF);
  __m256i spaced_result_right =
      _mm256_and_si256(spaced_result_intermediate, mask_3);
  print256_num(spaced_result_right, "spaced_result_right");

  // 88000000 AA000000 CC000000 EE000000 FE000000 BA000000 76000000 32000000
  const __m256i mask_4 = _mm256_set1_epi32(0xFF000000);
  __m256i spaced_result_left = _mm256_and_si256(
      _mm256_slli_epi32(spaced_result_intermediate, 8), mask_4);
  print256_num(spaced_result_left, "spaced_result_left");

  // 88000099 AA0000BB CC0000DD EE0000FF FE0000DC BA000098 76000054 32000010
  __m256i spaced_result_2 =
      _mm256_or_si256(spaced_result_left, spaced_result_right);
  print256_num(spaced_result_2, "spaced_result_2");

  // 00000000 00000000 8899AABB CCDDEEFF 00000000 00000000 FEDCBA98 76543210
  __m256i hadd_result =
      _mm256_hadd_epi16(spaced_result_2, _mm256_setzero_si256());
  print256_num(hadd_result, "hadd_result");

  __m128i result = _mm_set_epi64x(_mm256_extract_epi64(hadd_result, 0),
                                  _mm256_extract_epi64(hadd_result, 2));

  uint64_t high = _mm256_extract_epi64(hadd_result, 0);
  uint64_t low = _mm256_extract_epi64(hadd_result, 2);
  printf("high: %lX\n", high);
  printf("low: %lX\n", low);

  andyccs::BasicUuidV4 uuid_b(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  std::cout << "BasicUuidV4: " << std::string(uuid_b) << std::endl;

  andyccs::SimdUuidV4 uuid_s(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  std::cout << "SimdUuidV4: " << std::string(uuid_s) << std::endl;

  std::cout << "FromString: "
            << std::string(*andyccs::SimdUuidV4::FromString(
                   ("FEDCBA98-7654-3210-8899-AABBCCDDEEFF")))
            << std::endl;
  return 0;
}

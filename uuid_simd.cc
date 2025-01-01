#include "uuid_simd.h"

#include <cstdint>
#include <immintrin.h>
#include <optional>
#include <smmintrin.h>

namespace andyccs {
namespace {

inline __m256i reverse_m256i(__m256i a) {
  // Create a shuffle mask to reverse each 128-bit lane
  const __m256i shuffleMask =
      _mm256_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0,
                      1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

  // Shuffle the bytes within each lane
  a = _mm256_shuffle_epi8(a, shuffleMask);

  // Permute the 128-bit lanes
  a = _mm256_permute2x128_si256(a, a, 1);

  return a;
}

// Converts a 128-bits unsigned int to an UUIDv4 string representation.
// Uses SIMD via Intel's AVX2 instruction set.
inline void m256itos(__m256i input256, char *mem,
                     bool enable_pretty_shuffle = true) {
  // Shifts the 64-bit integers within a to the right by 4 bits. This
  // effectively separates the upper and lower nibbles (4 bits) of each byte.
  // Suppose
  // a = 00000000 00000000 00000000 00000000 FEDCBA9 876543210 AAAAAAAA AAAAAAAA
  // Then
  // s = 00000000 00000000 00000000 00000000 0FEDCBA9 87654321 0AAAAAAA AAAAAAAA
  __m256i input256_shift_right = _mm256_srli_epi64(input256, 4);

  // Suppose
  // s = 00000000 00000000 00000000 00000000 0FEDCBA9 87654321 0AAAAAAA AAAAAAAA
  // a = 00000000 00000000 00000000 00000000 FEDCBA98 76543210 AAAAAAAA AAAAAAAA
  // Then
  // l = 00000000 00000000 00000000 00000000 AA0AAAAA AAAAAAAA AAAAAAAA AAAAAAAA
  // h = 00000000 00000000 00000000 00000000 FE0FDCED BACB98A9 76875465 32431021
  __m256i low = _mm256_unpacklo_epi8(input256_shift_right, input256);
  __m128i high = _mm256_castsi256_si128(
      _mm256_unpackhi_epi8(input256_shift_right, input256));

  // c = FE0FDCED BACB98A9 76875465 32431021 AA0AAAAA AAAAAAAA AAAAAAAA AAAAAAAA
  __m256i combine = _mm256_inserti128_si256(low, high, 1);

  // mask: bitmask to extract the lower 4 bits of each byte.
  // 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F
  const __m256i mask = _mm256_set1_epi8(0x0F);

  // c = FE0FDCED BACB98A9 76875465 32431021 AA0AAAAA AAAAAAAA AAAAAAAA AAAAAAAA
  //     0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F
  // d = 0E0F0C0D 0A0B0809 06070405 02030001 0A0A0A0A 0A0A0A0A 0A0A0A0A 0A0A0A0A
  // Notice that all data is in each bytes:
  // EFCD AB89 6745 2301 AA AA AA AA AA AA AA AA
  __m256i data = _mm256_and_si256(combine, mask);

  // add: will be used to offset the ASCII values of digits
  // 06060606 06060606 06060606 06060606
  const __m256i add = _mm256_set1_epi8(0x06);

  // alpha_mask: will be used to identify hex digits A-F
  // 10101010 10101010 10101010 10101010
  const __m256i alpha_mask = _mm256_set1_epi8(0x10);

  // alpha_offset: will be used to offset the ASCII values of hex digits A-F.
  // Note that 'A' - 0x0A == 0x37
  const __m256i alpha_offset = _mm256_set1_epi8(0x37);

  // d = 0E0F0C0D 0A0B0809 06070405 02030001 0A0A0A0A 0A0A0A0A 0A0A0A0A 0A0A0A0A
  // ADD 06060606 06060606 06060606 06060606 06060606 06060606 06060606 06060606
  // AND 10101010 10101010 10101010 10101010 10101010 10101010 10101010 10101010
  // SHIFT LEFT 3 bits every 64 bits, so that the most significant bit can tell
  // whether a nibble is alpha (bit 1) or digit (bit 0).
  //     80808080 80800000 00000000 00000000 80808080 80808080 80808080 80808080
  __m256i alpha = _mm256_slli_epi64(
      _mm256_and_si256(_mm256_add_epi8(data, add), alpha_mask), 3);

  // Choose 0x30 (ASCII code for '0')
  // or 0x57  (ASCII code for 'A' - 0x10)
  //     57575757 57573030 30303030 30303030 57575757 57575757 57575757 57575757
  __m256i offset =
      _mm256_blendv_epi8(_mm256_slli_epi64(add, 3), alpha_offset, alpha);

  // Now you get the ASCII index for each nibble.
  // d = 0E0F0C0D 0A0B0809 06070405 02030001 0A0A0A0A 0A0A0A0A 0A0A0A0A 0A0A0A0A
  //     37373737 37373030 30303030 30303030 37373737 37373737 37373737 37373737
  // r = 45464344 41423839 36373435 32333031 41414141 41414141 41414141 41414141
  // "EFCDAB89 67452301 AAAAAAAA AAAAAAAA"
  __m256i res = _mm256_add_epi8(data, offset);

  // "EFCDAB89 67452301 AAAAAAAA AAAAAAAA" is not really in the original input
  // sequence. pretty_shuffle is trying to fix that.
  const __m256i pretty_shuffle =
      _mm256_set_epi32(0x0e0f0c0d, 0x0a0b0809, 0x06070405, 0x02030001,
                       0x0e0f0c0d, 0x0a0b0809, 0x06070405, 0x02030001);
  __m256i res_pretty =
      enable_pretty_shuffle
          ? reverse_m256i(_mm256_shuffle_epi8(res, pretty_shuffle))
          : res;

  // Add dashes between blocks as specified in RFC-4122
  // 8-4-4-4-12
  const __m256i dash_shuffle =
      _mm256_set_epi32(0x0b0a0908, 0x07060504, 0x80030201, 0x00808080,
                       0x0d0c800b, 0x0a090880, 0x07060504, 0x03020100);
  __m256i resd = _mm256_shuffle_epi8(res_pretty, dash_shuffle);

  const __m256i dash =
      _mm256_set_epi64x(0x0000000000000000ull, 0x2d000000002d0000ull,
                        0x00002d000000002d, 0x0000000000000000ull);
  resd = _mm256_or_si256(resd, dash);

  _mm256_storeu_si256((__m256i *)mem, resd);
  *(uint16_t *)(mem + 16) = _mm256_extract_epi16(res_pretty, 7);
  *(uint32_t *)(mem + 32) = _mm256_extract_epi32(res_pretty, 7);

  // Alternative implementation:
  //   *(uint64_t *)(mem) = _mm256_extract_epi64(res, 0);
  //   *(mem + 8) = '-';
  //   *(uint32_t *)(mem + 9) = _mm256_extract_epi32(res, 2);
  //   *(mem + 13) = '-';
  //   *(uint32_t *)(mem + 14) = _mm256_extract_epi32(res, 3);
  //   *(mem + 18) = '-';
  //   *(uint32_t *)(mem + 19) = _mm256_extract_epi32(res, 4);
  //   *(mem + 23) = '-';
  //   *(uint32_t *)(mem + 24) = _mm256_extract_epi32(res, 5);
  //   *(uint64_t *)(mem + 28) = _mm256_extract_epi64(res, 3);
}

inline bool
ValidateInput(__m256i pretty_input) { // Create a mask of valid bytes
  static uint8_t kValidHex[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                                0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46};
  __m256i result = _mm256_set_epi64x(0, 0, 0, 0);
  for (const auto &hex : kValidHex) {
    __m256i valid_bytes_mask = _mm256_set1_epi8(hex);
    __m256i cmp_result = _mm256_cmpeq_epi8(pretty_input, valid_bytes_mask);
    result = _mm256_or_si256(result, cmp_result);
  }

  const __m256i one = _mm256_set_epi64x(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                                        0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF);
  return _mm256_testc_si256(result, one);
}

inline __m256i CreatePrettyInput(const char *mem) {
  // Remove dashes and pack hex ascii bytes in a 256-bits int
  const __m256i dash_shuffle =
      _mm256_set_epi32(0x80808080, 0x0f0e0d0c, 0x0b0a0908, 0x06050403,
                       0x80800f0e, 0x0c0b0a09, 0x07060504, 0x03020100);

  // input: "FEDCBA98-7654-3210-8899-AABBCCDDEEFF"
  __m256i input = _mm256_loadu_si256((__m256i *)mem);
  input = _mm256_shuffle_epi8(input, dash_shuffle);
  input = _mm256_insert_epi16(input, *(uint16_t *)(mem + 16), 7);
  input = _mm256_insert_epi32(input, *(uint32_t *)(mem + 32), 7);

  // input: After pretty shuffle.
  // 38383939 41414242 43434444 45454646 46454443 42413938 37363534 33323130
  const __m256i pretty_shuffle =
      _mm256_set_epi32(0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f,
                       0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f);
  return _mm256_shuffle_epi8(input, pretty_shuffle);
}

// Converts an UUIDv4 string representation to a 128-bits unsigned int.
// Uses SIMD via Intel's AVX2 instruction set.
inline std::tuple<uint64_t, uint64_t> stom128i(__m256i pretty_input) {
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

  // Subtract input with sub to map each ASCII to its hex
  // '0' (0x30) => 0x00
  // 'A' (0x41) => 0x11
  // 38383939 41414242 43434444 45454646 46454443 42413938 37363534 33323130
  // 30303030 30303030 30303030 30303030 30303030 30303030 30303030 30303030 SUB
  // 08080909 11111212 13131414 15151616 16151413 12110908 07060504 03020100
  __m256i a = _mm256_sub_epi8(pretty_input, sub);

  // alpha: Determine bytes are alpha
  // 0x80 means alpha
  // 0x00 means digit
  //
  // 16161515 14141313 12121111 09090808 00010203 04050607 08091112 13141516
  // 10101010 10101010 10101010 10101010 10101010 10101010 10101010 10101010 AND
  // SHIFT LEFT 3
  // 00000000 80808080 80808080 80808080 80808080 80800000 00000000 00000000
  __m256i alpha = _mm256_slli_epi64(_mm256_and_si256(a, mask), 3);

  // sub_mask: Subtraction mask. What should be subtracted from each byte.
  // 00000000 07070707 07070707 07070707 07070707 07070000 00000000 00000000
  __m256i sub_mask = _mm256_blendv_epi8(digits_offset, alpha_offset, alpha);

  // spaced_result: Almost the result, but there is a 0x0 space in between
  // 08080909 11111212 13131414 15151616 16151413 12110908 07060504 03020100
  // 0F0F0E0E 0D0D0C0C 0B0B0A0A 09090808 00010203 04050607 08090A0B 0C0D0E0F SUB
  // 08080909 0A0A0B0B 0C0C0D0D 0E0E0F0F 0F0E0D0C 0B0A0908 07060504 03020100
  __m256i spaced_result = _mm256_sub_epi8(a, sub_mask);

  // spaced_result_intermediate: Trying to remove the 0x0 space.
  // 00880099 00AA00BB 00CC00DD 00EE00FF 00FE00DC 00BA0098 00760054 00320010
  const __m256i mask_2 = _mm256_set1_epi16(0x00FF);
  __m256i spaced_result_intermediate = _mm256_and_si256(
      _mm256_or_si256(spaced_result, _mm256_srli_epi16(spaced_result, 4)),
      mask_2);

  // final_result: Packed the result in the lower 64 bits
  // high = 00000000 00000000 FEDCBA98 76543210
  // low  = 00000000 00000000 8899AABB CCDDEEFF
  const __m128i shuffle =
      _mm_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, -1, -1, -1, -1, -1, -1, -1, -1);
  __m128i high = _mm_shuffle_epi8(
      _mm256_extracti128_si256(spaced_result_intermediate, 0), shuffle);
  __m128i low = _mm_shuffle_epi8(
      _mm256_extracti128_si256(spaced_result_intermediate, 1), shuffle);

  // high = FEDCBA98 76543210
  // low = 8899AABB CCDDEEFF
  return {_mm_extract_epi64(high, 0), _mm_extract_epi64(low, 0)};
}

uint64_t convert_to_uint64(const uint8_t *array) {
  uint64_t result = 0;
  for (int i = 0; i < 8; ++i) {
    result |= (uint64_t)array[i] << (8 * (7 - i));
  }
  return result;
}

inline void ToCharsInternal(uint64_t high, uint64_t low, char *buffer) {
  __m256i input = _mm256_set_epi64x(0, 0, high, low);
  m256itos(input, buffer);
}

} // namespace

static constexpr char kHexMap[] = {"0123456789ABCDEF"};

SimdUuidV4::SimdUuidV4(const std::uint8_t (&data)[16]) {
  high_ = convert_to_uint64(data);
  low_ = convert_to_uint64(&data[8]);
}

void SimdUuidV4::ToString(std::string &result) const {
  if (result.size() != 36) {
    result.resize(36);
  }
  ToCharsInternal(high_, low_, result.data());
}

SimdUuidV4::operator std::string() const {
  std::string result(36, char());
  ToCharsInternal(high_, low_, result.data());
  return result;
}

void SimdUuidV4::ToChars(char (&buffer)[37]) const {
  ToCharsInternal(high_, low_, buffer);
  buffer[36] = '\0';
}

std::optional<SimdUuidV4> SimdUuidV4::FromString(std::string_view from) {
  if (from.size() != 36) {
    return std::nullopt;
  }

  if (from[8] != '-' || from[13] != '-' || from[18] != '-' || from[23] != '-') {
    return std::nullopt;
  }

  __m256i pretty_input = CreatePrettyInput(from.data());
  if (!ValidateInput(pretty_input)) {
    return std::nullopt;
  }
  auto [high, low] = stom128i(pretty_input);
  return SimdUuidV4(high, low);
}

size_t SimdUuidV4::hash() const {
  return high_ ^ (low_ + 0x9e3779b9 + (high_ << 6) + (high_ >> 2));
}

} // namespace andyccs

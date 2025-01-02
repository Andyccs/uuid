#include "uuid_simd.h"

#include <cstdint>
#include <immintrin.h>
#include <iostream>
#include <optional>
#include <smmintrin.h>

namespace andyccs {
namespace {

// Converts a 128-bits unsigned int to an UUIDv4 string representation.
// Uses SIMD via Intel's AVX2 instruction set.
inline void m256itos(__m256i input256, char *mem) {
  // Real world input 0xFEDCBA98 76543210 8899AABB CCDDEEFF

  // Shifts the 64-bit integers within a to the right by 4 bits. This
  // effectively separates the upper and lower nibbles (4 bits) of each byte.
  // Suppose
  // i = 00000000 00000000 00000000 00000000 FFEEDDCC BBAA9988 10325476 98BADCFE
  // Then
  // s = 0FFEEDDC CBBAA998 01032547 698BADCF 0FFEEDDC CBBAA998 01032547 698BADCF
  __m256i input256_shift_right = _mm256_srli_epi64(input256, 4);

  // Suppose
  // i = 00000000 00000000 00000000 00000000 FFEEDDCC BBAA9988 10325476 98BADCFE
  // s = 0FFEEDDC CBBAA998 01032547 698BADCF 0FFEEDDC CBBAA998 01032547 698BADCF
  // Then
  // l = 10013203 54257647 9869BA8B DCADFECF 10013203 54257647 9869BA8B DCADFECF
  // h = 00000000 00000000 00000000 00000000 FF0FEEFE DDEDCCDC BBCBAABA 99A98898
  __m256i low = _mm256_unpacklo_epi8(input256_shift_right, input256);
  __m128i high = _mm256_castsi256_si128(
      _mm256_unpackhi_epi8(input256_shift_right, input256));

  // c = FF0FEEFE DDEDCCDC BBCBAABA 99A98898 10013203 54257647 9869BA8B DCADFECF
  __m256i combine = _mm256_inserti128_si256(low, high, 1);

  // mask: bitmask to extract the lower 4 bits of each byte.
  // 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F
  const __m256i mask = _mm256_set1_epi8(0x0F);

  // c = FF0FEEFE DDEDCCDC BBCBAABA 99A98898 10013203 54257647 9869BA8B DCADFECF
  //     0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F 0F0F0F0F
  // d = 0F0F0E0E 0D0D0C0C 0B0B0A0A 09090808 00010203 04050607 08090A0B 0C0D0E0F
  // Notice that all data is in each bytes:
  // FFEE DDCC BBAA 9988 0123 456 78AB CDEF
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

  // d = 0F0F0E0E 0D0D0C0C 0B0B0A0A 09090808 00010203 04050607 08090A0B 0C0D0E0F
  // ADD 06060606 06060606 06060606 06060606 06060606 06060606 06060606 06060606
  // AND 10101010 10101010 10101010 10101010 10101010 10101010 10101010 10101010
  // SHIFT LEFT 3 bits every 64 bits, so that the most significant bit can tell
  // whether a nibble is alpha (bit 1) or digit (bit 0).
  //     80808080 80808080 80808080 00000000 00000000 00000000 00008080 80808080
  __m256i alpha = _mm256_slli_epi64(
      _mm256_and_si256(_mm256_add_epi8(data, add), alpha_mask), 3);

  // Choose 0x30 (ASCII code for '0')
  // or 0x57  (ASCII code for 'A' - 0x10)
  //     37373737 37373737 37373737 30303030 30303030 30303030 30303737 37373737
  __m256i offset =
      _mm256_blendv_epi8(_mm256_slli_epi64(add, 3), alpha_offset, alpha);

  // Now you get the ASCII index for each nibble.
  // d = 0F0F0E0E 0D0D0C0C 0B0B0A0A 09090808 00010203 04050607 08090A0B 0C0D0E0F
  //     37373737 37373737 37373737 30303030 30303030 30303030 30303737 37373737
  // r = 46464545 44444343 42424141 39393838 30313233 34353637 38394142 43444546
  // "FFEE DDCC BBAA 9988 0123 4567 89AB CDEF"
  __m256i res = _mm256_add_epi8(data, offset);

  // Add dashes between blocks so that the string is formatted as 8-4-4-4-12
  // 44444343 42424141 00393938 38000000 32330034 35363700 38394142 43444546
  const __m256i dash_shuffle =
      _mm256_set_epi32(0x0b0a0908, 0x07060504, 0x80030201, 0x00808080,
                       0x0d0c800b, 0x0a090880, 0x07060504, 0x03020100);
  __m256i resd = _mm256_shuffle_epi8(res, dash_shuffle);

  // 44444343 42424141 2D393938 382D0000 32332D34 3536372D 38394142 43444546
  // ^                                                                     ^
  // bit index 255                                               bit index 0
  // "FFEE"   "DDCC"   "BBAA"   "9988"   "0123"   "4567"   "89AB"   "CDEF"
  const __m256i dash =
      _mm256_set_epi64x(0x0000000000000000ull, 0x2d000000002d0000ull,
                        0x00002d000000002d, 0x0000000000000000ull);
  resd = _mm256_or_si256(resd, dash);

  // Reminder that the real world input is 0xFEDCBA98 76543210 8899AABB CCDDEEFF
  // By copying from bit index 0 to index 255, we get the correct string.

  _mm256_storeu_si256((__m256i *)mem, resd);
  *(uint16_t *)(mem + 16) = _mm256_extract_epi16(res, 7);
  *(uint32_t *)(mem + 32) = _mm256_extract_epi32(res, 7);

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

inline bool ValidateInput(__m256i pretty_input) {
  const __m128i allowed_char_range =
      _mm_setr_epi8('0', '9', 'A', 'F', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  // For each of the character in the second argument
  // If the character is in the range of the first argument
  // Then the corresponding bit in the result is set to 0
  // Return the smallest index of the first 1 bit.
  int cmp_lower = _mm_cmpistri(
      allowed_char_range, _mm256_extractf128_si256(pretty_input, 0),
      _SIDD_UBYTE_OPS | _SIDD_CMP_RANGES | _SIDD_NEGATIVE_POLARITY |
          _SIDD_LEAST_SIGNIFICANT);
  int cmp_higher = _mm_cmpistri(
      allowed_char_range, _mm256_extractf128_si256(pretty_input, 1),
      _SIDD_UBYTE_OPS | _SIDD_CMP_RANGES | _SIDD_NEGATIVE_POLARITY |
          _SIDD_LEAST_SIGNIFICANT);
  return cmp_lower == 16 && cmp_higher == 16;
}

inline __m256i CreateInput(const char *mem) {
  // Remove dashes and pack hex ascii bytes in a 256-bits int
  const __m256i dash_shuffle =
      _mm256_set_epi32(0x80808080, 0x0f0e0d0c, 0x0b0a0908, 0x06050403,
                       0x80800f0e, 0x0c0b0a09, 0x07060504, 0x03020100);

  // input: "FEDCBA98-7654-3210-8899-AABBCCDDEEFF"
  // 46464545 44444343 42424141 39393838 30313233 34353637 38394142 43444546
  __m256i input = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(mem));
  input = _mm256_shuffle_epi8(input, dash_shuffle);
  input = _mm256_insert_epi16(input,
                              *reinterpret_cast<const uint16_t *>(mem + 16), 7);
  input = _mm256_insert_epi32(input,
                              *reinterpret_cast<const uint32_t *>(mem + 32), 7);
  return input;
}

// Converts an UUIDv4 string representation to a 128-bits unsigned int.
// Uses SIMD via Intel's AVX2 instruction set.
inline __m128i stom128i(__m256i pretty_input) {
  // input: "FEDCBA98-7654-3210-8899-AABBCCDDEEFF"

  // Build a mask to apply a different offset to alphas and digits
  const __m256i sub = _mm256_set1_epi8(0x30);

  // mask to determine whether it is a alpha
  const __m256i mask = _mm256_set1_epi8('9');

  // 'F' -> 0x46
  // 0x46 - 0x37 = 0x0F
  const __m256i alpha_offset = _mm256_set1_epi8(0x37);

  // Digit offset
  const __m256i digits_offset = _mm256_set1_epi8('0');

  // alpha: Determine bytes are alpha
  // 0xFF means alpha
  // 0x00 means digit
  //
  // 46464545 44444343 42424141 39393838 30313233 34353637 38394142 43444546
  // 39393939 39393939 39393939 39393939 39393939 39393939 39393939 39393939 cmp
  // FFFFFFFF FFFFFFFF FFFFFFFF 00000000 00000000 00000000 0000FFFF FFFFFFFF
  __m256i alpha = _mm256_cmpgt_epi8(pretty_input, mask);

  // sub_mask: Subtraction mask. What should be subtracted from each byte.
  // 37373737 37373737 37373737 30303030 30303030 30303030 30303737 37373737
  __m256i sub_mask = _mm256_blendv_epi8(digits_offset, alpha_offset, alpha);

  // spaced_result: Almost the result, but there is a 0x0 space in between
  // 46464545 44444343 42424141 39393838 30313233 34353637 38394142 43444546
  // 37373737 37373737 37373737 30303030 30303030 30303030 30303737 37373737
  // 0F0F0E0E 0D0D0C0C 0B0B0A0A 09090808 00010203 04050607 08090A0B 0C0D0E0F
  __m256i spaced_result = _mm256_sub_epi8(pretty_input, sub_mask);

  // 0F0E0D0C 0B0A0908 0F0E0D0C 0B0A0908 00020406 080A0C0E 01030507 090B0D0F
  //                 ^3                ^2                ^1                ^0
  const __m256i odd_even_shuffle =
      _mm256_set_epi8(15, 13, 11, 9, 7, 5, 3, 1, 14, 12, 10, 8, 6, 4, 2, 0, 15,
                      13, 11, 9, 7, 5, 3, 1, 14, 12, 10, 8, 6, 4, 2, 0);
  __m256i odd_even_shuffled_result =
      _mm256_shuffle_epi8(spaced_result, odd_even_shuffle);

  // odd:    0F0E0D0C 0B0A0908 01030507 090B0D0F
  // even:   0F0E0D0C 0B0A0908 00020406 080A0C0E
  __m128i low = _mm256_extracti128_si256(odd_even_shuffled_result, 0);
  __m128i high = _mm256_extracti128_si256(odd_even_shuffled_result, 1);
  __m128i odd = _mm_unpacklo_epi64(low, high);
  __m128i even = _mm_unpackhi_epi64(low, high);

  // F0E0D0C0 B0A09080 10305070 90B0D0F0
  odd = _mm_slli_epi64(odd, 4);

  //  FFEEDDCC BBAA9988 10325476 98BADCFE
  return _mm_xor_si128(odd, even);
}

inline void uint64_to_bytes(uint64_t value, uint8_t *array) {
  for (int i = 0; i < 8; ++i) {
    array[i] = (value >> (8 * (7 - i))) & 0xFF;
  }
}

inline void ToCharsInternal(uint8_t const *data, char *buffer) {
  __m256i input = _mm256_loadu_si256((__m256i *)data);
  m256itos(input, buffer);
}

} // namespace

static constexpr char kHexMap[] = {"0123456789ABCDEF"};

SimdUuidV4::SimdUuidV4(uint64_t high, uint64_t low) {
  uint64_to_bytes(high, data_.data());
  uint64_to_bytes(low, data_.data() + 8);
}

SimdUuidV4::SimdUuidV4(const std::uint8_t (&data)[16]) {
  std::copy(data, data + 16, data_.begin());
}

void SimdUuidV4::ToString(std::string &result) const {
  if (result.size() != 36) {
    result.resize(36);
  }
  ToCharsInternal(data_.data(), result.data());
}

SimdUuidV4::operator std::string() const {
  constexpr std::string_view kDefaultString =
      "012345678901234567890123456789012345";
  std::string result(kDefaultString);
  ToCharsInternal(data_.data(), result.data());
  return result;
}

void SimdUuidV4::ToChars(char (&buffer)[37]) const {
  ToCharsInternal(data_.data(), buffer);
  buffer[36] = '\0';
}

std::optional<SimdUuidV4> SimdUuidV4::FromString(std::string_view from) {
  if (from.size() != 36) {
    return std::nullopt;
  }

  if (from[8] != '-' || from[13] != '-' || from[18] != '-' || from[23] != '-') {
    return std::nullopt;
  }

  __m256i pretty_input = CreateInput(from.data());
  if (!ValidateInput(pretty_input)) {
    return std::nullopt;
  }
  __m128i result_i = stom128i(pretty_input);
  std::array<uint8_t, 16> result;
  _mm_storeu_si128(reinterpret_cast<__m128i *>(result.data()), result_i);
  return SimdUuidV4(result);
}

size_t SimdUuidV4::hash() const {
  std::string result;
  ToString(result);
  return std::hash<std::string>()(std::move(result));
}

} // namespace andyccs

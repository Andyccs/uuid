#include "uuid_basic.h"

#include <cctype>
#include <cmath>
#include <cstring>
#include <optional>

namespace andyccs {
namespace {

struct LookupTable1 {
  uint8_t c[71];

  constexpr LookupTable1() : c{} {
    c['0'] = 0 << 4;
    c['1'] = 1 << 4;
    c['2'] = 2 << 4;
    c['3'] = 3 << 4;
    c['4'] = 4 << 4;
    c['5'] = 5 << 4;
    c['6'] = 6 << 4;
    c['7'] = 7 << 4;
    c['8'] = 8 << 4;
    c['9'] = 9 << 4;
    c['A'] = 0xA << 4;
    c['B'] = 0xB << 4;
    c['C'] = 0xC << 4;
    c['D'] = 0xD << 4;
    c['E'] = 0xE << 4;
    c['F'] = 0xF << 4;
  }
  constexpr uint8_t operator[](char ch) const { return c[ch]; }
};

struct LookupTable2 {
  uint8_t c[71];

  constexpr LookupTable2() : c{} {
    c['0'] = 0;
    c['1'] = 1;
    c['2'] = 2;
    c['3'] = 3;
    c['4'] = 4;
    c['5'] = 5;
    c['6'] = 6;
    c['7'] = 7;
    c['8'] = 8;
    c['9'] = 9;
    c['A'] = 0xA;
    c['B'] = 0xB;
    c['C'] = 0xC;
    c['D'] = 0xD;
    c['E'] = 0xE;
    c['F'] = 0xF;
  }
  constexpr uint8_t operator[](char ch) const { return c[ch]; }
};

constexpr LookupTable1 kLookupTable1;
constexpr LookupTable2 kLookupTable2;

inline uint8_t HexToVal(const char &c1, const char &c2) {
  return kLookupTable1[c1] | kLookupTable2[c2];
}

inline bool IsValid(const char &c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

inline void uint64_to_bytes(uint64_t value, uint8_t *array) {
  for (int i = 0; i < 8; ++i) {
    array[i] = (value >> (8 * (7 - i))) & 0xFF;
  }
}

inline bool ConvertStringRangeToBytes(std::string_view from, size_t start,
                                      size_t end, size_t data_start_index,
                                      uint8_t *data) {
  for (int i = start; i < end; i += 2) {
    const char &c1 = from[i];
    const char &c2 = from[i + 1];
    if (!IsValid(c1) || !IsValid(c2)) {
      return false;
    }
    data[data_start_index++] = HexToVal(c1, c2);
  }
  return true;
}

inline void ToCharsInternal(const std::array<std::uint8_t, 16> &data,
                            char *out) {
  constexpr char const *kHexMap = "0123456789ABCDEF-";
  for (std::size_t i = 0; i < 16; ++i) {
    std::uint8_t ch = data[i];

    *out++ = kHexMap[ch >> 4];
    *out++ = kHexMap[ch & 0x0F];

    if (i == 3 || i == 5 || i == 7 || i == 9) {
      *out++ = kHexMap[16];
    }
  }
}

} // namespace

BasicUuid::BasicUuid(uint64_t high, uint64_t low) {
  uint64_to_bytes(high, data_.data());
  uint64_to_bytes(low, data_.data() + 8);
}

BasicUuid::BasicUuid(const std::uint8_t (&data)[16]) {
  std::copy(data, data + 16, data_.begin());
}

void BasicUuid::ToString(std::string &result) const {
  if (result.size() != 36) {
    result.resize(36);
  }
  ToCharsInternal(data_, result.data());
}

void BasicUuid::ToChars(char (&buffer)[37]) const {
  ToCharsInternal(data_, buffer);
  buffer[36] = '\0';
}

BasicUuid::operator std::string() const {
  constexpr std::string_view kDefaultString =
      "012345678901234567890123456789012345";
  std::string result(kDefaultString);
  ToCharsInternal(data_, result.data());
  return result;
}

std::optional<BasicUuid> BasicUuid::FromString(std::string_view from) {
  if (from.size() != 36) {
    return std::nullopt;
  }

  if (from[8] != '-' || from[13] != '-' || from[18] != '-' || from[23] != '-') {
    return std::nullopt;
  }
  uint8_t data[16];
  if (!ConvertStringRangeToBytes(from, 0, 8, 0, data) ||
      !ConvertStringRangeToBytes(from, 9, 13, 4, data) ||
      !ConvertStringRangeToBytes(from, 14, 18, 6, data) ||
      !ConvertStringRangeToBytes(from, 19, 23, 8, data) ||
      !ConvertStringRangeToBytes(from, 24, 36, 10, data)) {
    return std::nullopt;
  }
  return BasicUuid(data);
}

size_t BasicUuid::hash() const {
  std::string result;
  ToString(result);
  return std::hash<std::string>()(std::move(result));
}

} // namespace andyccs

#include "uuid_basic.h"

#include <cctype>
#include <cmath>
#include <cstring>
#include <optional>

namespace andyccs {
namespace {

inline uint8_t HexToVal(const char &c) {
  return std::isdigit(c) ? (c - '0') : (c - 'A' + 10);
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
    data[data_start_index++] = (HexToVal(c1) << 4) | HexToVal(c2);
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

BasicUuidV4::BasicUuidV4(uint64_t high, uint64_t low) {
  uint64_to_bytes(high, data_.data());
  uint64_to_bytes(low, data_.data() + 8);
}

BasicUuidV4::BasicUuidV4(const std::uint8_t (&data)[16]) {
  std::copy(data, data + 16, data_.begin());
}

void BasicUuidV4::ToString(std::string &result) const {
  if (result.size() != 36) {
    result.resize(36);
  }
  ToCharsInternal(data_, result.data());
}

void BasicUuidV4::ToChars(char (&buffer)[37]) const {
  ToCharsInternal(data_, buffer);
  buffer[36] = '\0';
}

BasicUuidV4::operator std::string() const {
  std::string result;
  ToString(result);
  return result;
}

std::optional<BasicUuidV4> BasicUuidV4::FromString(std::string_view from) {
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
  return BasicUuidV4(data);
}

size_t BasicUuidV4::hash() const {
  std::string result;
  ToString(result);
  return std::hash<std::string>()(std::move(result));
}

} // namespace andyccs

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

} // namespace

static constexpr char kHexMap[] = {"0123456789ABCDEF"};

BasicUuidV4::BasicUuidV4(uint64_t high, uint64_t low) {
  uint64_to_bytes(high, data_.data());
  uint64_to_bytes(low, data_.data() + 8);
}

BasicUuidV4::BasicUuidV4(const std::uint8_t (&data)[16]) {
  std::copy(data, data + 16, data_.begin());
}

void BasicUuidV4::ToString(std::string &result) const {
  result.resize(36);
  result[0] = kHexMap[data_[0] >> 4];
  result[1] = kHexMap[data_[0] & 0xF];
  result[2] = kHexMap[data_[1] >> 4];
  result[3] = kHexMap[data_[1] & 0xF];
  result[4] = kHexMap[data_[2] >> 4];
  result[5] = kHexMap[data_[2] & 0xF];
  result[6] = kHexMap[data_[3] >> 4];
  result[7] = kHexMap[data_[3] & 0xF];
  result[8] = '-';
  result[9] = kHexMap[data_[4] >> 4];
  result[10] = kHexMap[data_[4] & 0xF];
  result[11] = kHexMap[data_[5] >> 4];
  result[12] = kHexMap[data_[5] & 0xF];
  result[13] = '-';
  result[14] = kHexMap[data_[6] >> 4];
  result[15] = kHexMap[data_[6] & 0xF];
  result[16] = kHexMap[data_[7] >> 4];
  result[17] = kHexMap[data_[7] & 0xF];
  result[18] = '-';

  result[19] = kHexMap[data_[8] >> 4];
  result[20] = kHexMap[data_[8] & 0xF];
  result[21] = kHexMap[data_[9] >> 4];
  result[22] = kHexMap[data_[9] & 0xF];
  result[23] = '-';
  result[24] = kHexMap[data_[10] >> 4];
  result[25] = kHexMap[data_[10] & 0xF];
  result[26] = kHexMap[data_[11] >> 4];
  result[27] = kHexMap[data_[11] & 0xF];
  result[28] = kHexMap[data_[12] >> 4];
  result[29] = kHexMap[data_[12] & 0xF];
  result[30] = kHexMap[data_[13] >> 4];
  result[31] = kHexMap[data_[13] & 0xF];
  result[32] = kHexMap[data_[14] >> 4];
  result[33] = kHexMap[data_[14] & 0xF];
  result[34] = kHexMap[data_[15] >> 4];
  result[35] = kHexMap[data_[15] & 0xF];
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

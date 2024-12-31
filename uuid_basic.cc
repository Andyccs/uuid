#include "uuid_basic.h"

#include <cctype>
#include <cmath>
#include <cstring>
#include <optional>

namespace andyccs {
namespace {

inline uint64_t HexToVal(const char &c) {
  return std::isdigit(c) ? (c - '0') : (c - 'A' + 10);
}

inline bool IsValid(const char &c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

uint64_t convert_to_uint64(const uint8_t *array) {
  uint64_t result = 0;
  for (int i = 0; i < 8; ++i) {
    result |= (uint64_t)array[i] << (8 * (7 - i));
  }
  return result;
}

} // namespace

static constexpr char kHexMap[] = {"0123456789ABCDEF"};

BasicUuidV4::BasicUuidV4(const std::uint8_t (&data)[16]) {
  high_ = convert_to_uint64(data);
  low_ = convert_to_uint64(&data[8]);
}

void BasicUuidV4::ToString(std::string &result) const {
  result.resize(36);
  result[0] = kHexMap[(high_ >> 60) & 0xF];
  result[1] = kHexMap[(high_ >> 56) & 0xF];
  result[2] = kHexMap[(high_ >> 52) & 0xF];
  result[3] = kHexMap[(high_ >> 48) & 0xF];
  result[4] = kHexMap[(high_ >> 44) & 0xF];
  result[5] = kHexMap[(high_ >> 40) & 0xF];
  result[6] = kHexMap[(high_ >> 36) & 0xF];
  result[7] = kHexMap[(high_ >> 32) & 0xF];
  result[8] = '-';
  result[9] = kHexMap[(high_ >> 28) & 0xF];
  result[10] = kHexMap[(high_ >> 24) & 0xF];
  result[11] = kHexMap[(high_ >> 20) & 0xF];
  result[12] = kHexMap[(high_ >> 16) & 0xF];
  result[13] = '-';
  result[14] = kHexMap[(high_ >> 12) & 0xF];
  result[15] = kHexMap[(high_ >> 8) & 0xF];
  result[16] = kHexMap[(high_ >> 4) & 0xF];
  result[17] = kHexMap[high_ & 0xF];
  result[18] = '-';

  result[19] = kHexMap[(low_ >> 60) & 0xF];
  result[20] = kHexMap[(low_ >> 56) & 0xF];
  result[21] = kHexMap[(low_ >> 52) & 0xF];
  result[22] = kHexMap[(low_ >> 48) & 0xF];
  result[23] = '-';
  result[24] = kHexMap[(low_ >> 44) & 0xF];
  result[25] = kHexMap[(low_ >> 40) & 0xF];
  result[26] = kHexMap[(low_ >> 36) & 0xF];
  result[27] = kHexMap[(low_ >> 32) & 0xF];
  result[28] = kHexMap[(low_ >> 28) & 0xF];
  result[29] = kHexMap[(low_ >> 24) & 0xF];
  result[30] = kHexMap[(low_ >> 20) & 0xF];
  result[31] = kHexMap[(low_ >> 16) & 0xF];
  result[32] = kHexMap[(low_ >> 12) & 0xF];
  result[33] = kHexMap[(low_ >> 8) & 0xF];
  result[34] = kHexMap[(low_ >> 4) & 0xF];
  result[35] = kHexMap[low_ & 0xF];
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
  for (int i = 0; i < from.size(); ++i) {
    if (i == 8 || i == 13 || i == 18 || i == 23) {
      continue;
    }
    if (!IsValid(from[i])) {
      return std::nullopt;
    }
  }

  uint64_t high = (HexToVal(from[0]) << 60) | (HexToVal(from[1]) << 56) |
                  (HexToVal(from[2]) << 52) | (HexToVal(from[3]) << 48) |
                  (HexToVal(from[4]) << 44) | (HexToVal(from[5]) << 40) |
                  (HexToVal(from[6]) << 36) | (HexToVal(from[7]) << 32) |
                  (HexToVal(from[9]) << 28) | (HexToVal(from[10]) << 24) |
                  (HexToVal(from[11]) << 20) | (HexToVal(from[12]) << 16) |
                  (HexToVal(from[14]) << 12) | (HexToVal(from[15]) << 8) |
                  (HexToVal(from[16]) << 4) | (HexToVal(from[17]));

  uint64_t low = (HexToVal(from[19]) << 60) | (HexToVal(from[20]) << 56) |
                 (HexToVal(from[21]) << 52) | (HexToVal(from[22]) << 48) |
                 (HexToVal(from[24]) << 44) | (HexToVal(from[25]) << 40) |
                 (HexToVal(from[26]) << 36) | (HexToVal(from[27]) << 32) |
                 (HexToVal(from[28]) << 28) | (HexToVal(from[29]) << 24) |
                 (HexToVal(from[30]) << 20) | (HexToVal(from[31]) << 16) |
                 (HexToVal(from[32]) << 12) | (HexToVal(from[33]) << 8) |
                 (HexToVal(from[34]) << 4) | (HexToVal(from[35]));

  return BasicUuidV4(high, low);
}

size_t BasicUuidV4::hash() const {
  return high_ ^ (low_ + 0x9e3779b9 + (high_ << 6) + (high_ >> 2));
}

} // namespace andyccs

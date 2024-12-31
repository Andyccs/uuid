#include "uuid_simd.h"

#include <gtest/gtest.h>
#include <random>

namespace andyccs {

TEST(SimdUuidV4, CreateDefault) {
  SimdUuidV4 uuid;
  EXPECT_EQ(std::string(uuid), "00000000-0000-0000-0000-000000000000");
}

TEST(SimdUuidV4, CreateConstExpr) {
  constexpr SimdUuidV4 uuid =
      SimdUuidV4(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  EXPECT_EQ(std::string(uuid), "FEDCBA98-7654-3210-8899-AABBCCDDEEFF");
}

TEST(SimdUuidV4, FromString) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  std::optional<SimdUuidV4> uuid = SimdUuidV4::FromString(from);
  ASSERT_TRUE(uuid.has_value());
  EXPECT_EQ(std::string(*uuid), from);
}

TEST(SimdUuidV4, FromStringInvalidChar) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  for (int i = 0; i < 36; ++i) {
    std::string from_invalid_hex = from;
    from_invalid_hex[i] = 'R';
    EXPECT_FALSE(SimdUuidV4::FromString(from_invalid_hex).has_value());
  }
}

TEST(BasicUuidV4, FromStringInvalidDash) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  for (int i = 0; i < 36; ++i) {
    if (i == 8 || i == 13 || i == 18 || i == 23) {
      continue;
    }
    std::string from_invalid_dash = from;
    from_invalid_dash[i] = '-';
    EXPECT_FALSE(SimdUuidV4::FromString(from_invalid_dash).has_value());
  }
}

TEST(SimdUuidV4, FromStringInvalidLowerCase) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  for (int i = 0; i < 36; ++i) {
    std::string from_invalid_dash = from;
    from_invalid_dash[i] = 'a';
    EXPECT_FALSE(SimdUuidV4::FromString(from_invalid_dash).has_value());
  }
}

TEST(SimdUuidV4Generator, GenerateUUIDUsingMt199937) {
  SimdUuidV4Generator<std::mt19937> generator;
  SimdUuidV4 uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, SimdUuidV4());
}

TEST(SimdUuidV4Generator, GenerateUUIDUsingMt199937_64) {
  SimdUuidV4Generator<std::mt19937_64> generator;
  SimdUuidV4 uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, SimdUuidV4());
}

} // namespace andyccs
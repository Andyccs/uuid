#include "uuid_simd.h"

#include <gtest/gtest.h>
#include <random>

namespace andyccs {

TEST(SimdUuidV4, CreateDefault) {
  SimdUuidV4 uuid;
  EXPECT_EQ(std::string(uuid), "00000000-0000-0000-0000-000000000000");
}

TEST(BasicUuidV4, CreateHighLow) {
  SimdUuidV4 uuid = SimdUuidV4(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  EXPECT_EQ(std::string(uuid), "FEDCBA98-7654-3210-8899-AABBCCDDEEFF");
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

TEST(BasicUuidV4, FromData) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  SimdUuidV4 uuid(data);
  EXPECT_EQ(std::string(uuid), from);
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

TEST(BasicUuidV4, HashNoCollision) {
  SimdUuidV4 uuid_1 = SimdUuidV4(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  SimdUuidV4 uuid_2 = SimdUuidV4(0xFEDCBA9876543210, 0x8899AABBCCDDEEFE);
  EXPECT_NE(uuid_1.hash(), uuid_2.hash());
}

TEST(SimdUuidV4, HashType) {
  SimdUuidV4 uuid = SimdUuidV4(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  std::unordered_map<SimdUuidV4, std::string> my_map = {{uuid, "Hello World"}};
  EXPECT_EQ(my_map[uuid], "Hello World");
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

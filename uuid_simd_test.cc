#include "uuid_simd.h"

#include <gtest/gtest.h>
#include <random>

namespace andyccs {

TEST(SimdUuid, CreateDefault) {
  SimdUuid uuid;
  EXPECT_EQ(std::string(uuid), "00000000-0000-0000-0000-000000000000");
}

TEST(SimdUuid, CreateHighLow) {
  SimdUuid uuid = SimdUuid(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  EXPECT_EQ(std::string(uuid), "FEDCBA98-7654-3210-8899-AABBCCDDEEFF");
}

TEST(SimdUuid, FromData) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  SimdUuid uuid(data);
  EXPECT_EQ(std::string(uuid), from);
}

TEST(SimdUuid, CreateConstExpr) {
  constexpr std::array<std::uint8_t, 16> data = {
      0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
      0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  constexpr SimdUuid uuid(data);
  EXPECT_EQ(std::string(uuid), "6BBBB416-EDC3-405F-A86D-231D5800235E");
}

TEST(SimdUuid, Copy) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  SimdUuid uuid_1(data);
  SimdUuid uuid_2(uuid_1);
  EXPECT_EQ(uuid_1, uuid_2);

  SimdUuid uuid_3 = uuid_1;
  EXPECT_EQ(uuid_1, uuid_3);
}

TEST(SimdUuid, Move) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  SimdUuid uuid_1(data);
  SimdUuid uuid_2(std::move(uuid_1));
  EXPECT_EQ(uuid_1, SimdUuid());
  EXPECT_EQ(uuid_2, SimdUuid(data));

  SimdUuid uuid_3(data);
  SimdUuid uuid_4(data);
  uuid_4 = std::move(uuid_3);
  EXPECT_EQ(uuid_3, SimdUuid());
  EXPECT_EQ(uuid_4, SimdUuid(data));
}

TEST(SimdUuid, StringOperator) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  SimdUuid uuid(data);
  EXPECT_EQ(std::string(uuid), "6BBBB416-EDC3-405F-A86D-231D5800235E");
}

TEST(SimdUuid, ToString) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  SimdUuid uuid(data);

  std::string result;
  uuid.ToString(result);
  EXPECT_EQ(result, "6BBBB416-EDC3-405F-A86D-231D5800235E");
}

TEST(SimdUuid, ToChars) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  SimdUuid uuid(data);

  char result[37];
  uuid.ToChars(result);
  EXPECT_EQ(std::string(result), "6BBBB416-EDC3-405F-A86D-231D5800235E");
}

TEST(SimdUuid, FromString) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  std::optional<SimdUuid> uuid = SimdUuid::FromString(from);
  ASSERT_TRUE(uuid.has_value());
  EXPECT_EQ(std::string(*uuid), from);
}

TEST(SimdUuid, FromStringInvalidChar) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  for (int i = 0; i < 36; ++i) {
    std::string from_invalid_hex = from;
    from_invalid_hex[i] = 'R';
    EXPECT_FALSE(SimdUuid::FromString(from_invalid_hex).has_value());
  }
}

TEST(SimdUuid, FromStringInvalidDash) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  for (int i = 0; i < 36; ++i) {
    if (i == 8 || i == 13 || i == 18 || i == 23) {
      continue;
    }
    std::string from_invalid_dash = from;
    from_invalid_dash[i] = '-';
    EXPECT_FALSE(SimdUuid::FromString(from_invalid_dash).has_value());
  }
}

TEST(SimdUuid, FromStringInvalidLowerCase) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  for (int i = 0; i < 36; ++i) {
    std::string from_invalid_dash = from;
    from_invalid_dash[i] = 'a';
    EXPECT_FALSE(SimdUuid::FromString(from_invalid_dash).has_value());
  }
}

TEST(SimdUuid, HashNoCollision) {
  SimdUuid uuid_1 = SimdUuid(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  SimdUuid uuid_2 = SimdUuid(0xFEDCBA9876543210, 0x8899AABBCCDDEEFE);
  EXPECT_NE(uuid_1.hash(), uuid_2.hash());
}

TEST(SimdUuid, HashType) {
  SimdUuid uuid = SimdUuid(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  std::unordered_map<SimdUuid, std::string> my_map = {{uuid, "Hello World"}};
  EXPECT_EQ(my_map[uuid], "Hello World");
}

TEST(SimdUuidGenerator, GenerateUUIDUsingMt199937) {
  SimdUuidGenerator<std::mt19937> generator;
  SimdUuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, SimdUuid());
}

TEST(SimdUuidGenerator, GenerateUUIDUsingMt199937_64) {
  SimdUuidGenerator<std::mt19937_64> generator;
  SimdUuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, SimdUuid());
}

} // namespace andyccs

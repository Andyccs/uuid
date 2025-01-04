#include "uuid_basic.h"

#include <gtest/gtest.h>
#include <random>

namespace andyccs {

TEST(BasicUuid, CreateDefault) {
  BasicUuid uuid;
  EXPECT_EQ(std::string(uuid), "00000000-0000-0000-0000-000000000000");
}

TEST(BasicUuid, CreateHighLow) {
  BasicUuid uuid = BasicUuid(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  EXPECT_EQ(std::string(uuid), "FEDCBA98-7654-3210-8899-AABBCCDDEEFF");
}

TEST(BasicUuid, FromData) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  BasicUuid uuid(data);
  EXPECT_EQ(std::string(uuid), from);
}

TEST(BasicUuid, CreateConstExpr) {
  constexpr std::array<std::uint8_t, 16> data = {
      0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
      0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  constexpr BasicUuid uuid(data);
  EXPECT_EQ(std::string(uuid), "6BBBB416-EDC3-405F-A86D-231D5800235E");
}

TEST(BasicUuid, Copy) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  BasicUuid uuid_1(data);
  BasicUuid uuid_2(uuid_1);
  EXPECT_EQ(uuid_1, uuid_2);

  BasicUuid uuid_3 = uuid_1;
  EXPECT_EQ(uuid_1, uuid_3);
}

TEST(BasicUuid, Move) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  BasicUuid uuid_1(data);
  BasicUuid uuid_2(std::move(uuid_1));
  EXPECT_EQ(uuid_1, BasicUuid());
  EXPECT_EQ(uuid_2, BasicUuid(data));

  BasicUuid uuid_3(data);
  BasicUuid uuid_4(data);
  uuid_4 = std::move(uuid_3);
  EXPECT_EQ(uuid_3, BasicUuid());
  EXPECT_EQ(uuid_4, BasicUuid(data));
}

TEST(BasicUuid, StringOperator) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  BasicUuid uuid(data);
  EXPECT_EQ(std::string(uuid), "6BBBB416-EDC3-405F-A86D-231D5800235E");
}

TEST(BasicUuid, ToString) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  BasicUuid uuid(data);

  std::string result;
  uuid.ToString(result);
  EXPECT_EQ(result, "6BBBB416-EDC3-405F-A86D-231D5800235E");
}

TEST(BasicUuid, ToChars) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  BasicUuid uuid(data);

  char result[37];
  uuid.ToChars(result);
  EXPECT_EQ(std::string(result), "6BBBB416-EDC3-405F-A86D-231D5800235E");
}

TEST(BasicUuid, FromString) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  std::optional<BasicUuid> uuid = BasicUuid::FromString(from);
  ASSERT_TRUE(uuid.has_value());
  EXPECT_EQ(std::string(*uuid), from);
}

TEST(BasicUuid, FromStringInvalidChar) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  for (int i = 0; i < 36; ++i) {
    std::string from_invalid_hex = from;
    from_invalid_hex[i] = 'R';
    EXPECT_FALSE(BasicUuid::FromString(from_invalid_hex).has_value());
  }
}

TEST(BasicUuid, FromStringInvalidDash) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  for (int i = 0; i < 36; ++i) {
    if (i == 8 || i == 13 || i == 18 || i == 23) {
      continue;
    }
    std::string from_invalid_dash = from;
    from_invalid_dash[i] = '-';
    EXPECT_FALSE(BasicUuid::FromString(from_invalid_dash).has_value());
  }
}

TEST(BasicUuid, FromStringInvalidLowerCase) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  for (int i = 0; i < 36; ++i) {
    std::string from_invalid_dash = from;
    from_invalid_dash[i] = 'a';
    EXPECT_FALSE(BasicUuid::FromString(from_invalid_dash).has_value());
  }
}

TEST(BasicUuid, HashNoCollision) {
  BasicUuid uuid_1 = BasicUuid(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  BasicUuid uuid_2 = BasicUuid(0xFEDCBA9876543210, 0x8899AABBCCDDEEFE);
  EXPECT_NE(uuid_1.hash(), uuid_2.hash());
}

TEST(BasicUuid, HashType) {
  BasicUuid uuid = BasicUuid(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  std::unordered_map<BasicUuid, std::string> my_map = {{uuid, "Hello World"}};
  EXPECT_EQ(my_map[uuid], "Hello World");
}

TEST(BasicUuidGenerator, GenerateUUIDUsingMt199937) {
  BasicUuidGenerator<std::mt19937> generator;
  BasicUuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, BasicUuid());
}

TEST(BasicUuidGenerator, GenerateUUIDUsingMt199937_64) {
  BasicUuidGenerator<std::mt19937_64> generator;
  BasicUuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, BasicUuid());
}

} // namespace andyccs

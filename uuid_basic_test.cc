#include "uuid_basic.h"

#include <gtest/gtest.h>
#include <random>

namespace andyccs {

TEST(BasicUuidV4, CreateDefault) {
  BasicUuidV4 uuid;
  EXPECT_EQ(std::string(uuid), "00000000-0000-0000-0000-000000000000");
}

TEST(BasicUuidV4, CreateHighLow) {
  BasicUuidV4 uuid = BasicUuidV4(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  EXPECT_EQ(std::string(uuid), "FEDCBA98-7654-3210-8899-AABBCCDDEEFF");
}

TEST(BasicUuidV4, FromData) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  BasicUuidV4 uuid(data);
  EXPECT_EQ(std::string(uuid), from);
}

TEST(BasicUuidV4, CreateConstExpr) {
  constexpr std::array<std::uint8_t, 16> data = {
      0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
      0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  constexpr BasicUuidV4 uuid(data);
  EXPECT_EQ(std::string(uuid), "6BBBB416-EDC3-405F-A86D-231D5800235E");
}

TEST(BasicUuidV4, Copy) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  BasicUuidV4 uuid_1(data);
  BasicUuidV4 uuid_2(uuid_1);
  EXPECT_EQ(uuid_1, uuid_2);

  BasicUuidV4 uuid_3 = uuid_1;
  EXPECT_EQ(uuid_1, uuid_3);
}

TEST(BasicUuidV4, Move) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  BasicUuidV4 uuid_1(data);
  BasicUuidV4 uuid_2(std::move(uuid_1));
  EXPECT_EQ(uuid_1, BasicUuidV4());
  EXPECT_EQ(uuid_2, BasicUuidV4(data));

  BasicUuidV4 uuid_3(data);
  BasicUuidV4 uuid_4(data);
  uuid_4 = std::move(uuid_3);
  EXPECT_EQ(uuid_3, BasicUuidV4());
  EXPECT_EQ(uuid_4, BasicUuidV4(data));
}

TEST(BasicUuidV4, StringOperator) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  BasicUuidV4 uuid(data);
  EXPECT_EQ(std::string(uuid), "6BBBB416-EDC3-405F-A86D-231D5800235E");
}

TEST(BasicUuidV4, ToString) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  BasicUuidV4 uuid(data);

  std::string result;
  uuid.ToString(result);
  EXPECT_EQ(result, "6BBBB416-EDC3-405F-A86D-231D5800235E");
}

TEST(BasicUuidV4, ToChars) {
  std::uint8_t data[16] = {0x6B, 0xBB, 0xB4, 0x16, 0xED, 0xC3, 0x40, 0x5F,
                           0xA8, 0x6D, 0x23, 0x1D, 0x58, 0x0,  0x23, 0x5E};
  BasicUuidV4 uuid(data);

  char result[37];
  uuid.ToChars(result);
  EXPECT_EQ(std::string(result), "6BBBB416-EDC3-405F-A86D-231D5800235E");
}

TEST(BasicUuidV4, FromString) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  std::optional<BasicUuidV4> uuid = BasicUuidV4::FromString(from);
  ASSERT_TRUE(uuid.has_value());
  EXPECT_EQ(std::string(*uuid), from);
}

TEST(BasicUuidV4, FromStringInvalidChar) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  for (int i = 0; i < 36; ++i) {
    std::string from_invalid_hex = from;
    from_invalid_hex[i] = 'R';
    EXPECT_FALSE(BasicUuidV4::FromString(from_invalid_hex).has_value());
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
    EXPECT_FALSE(BasicUuidV4::FromString(from_invalid_dash).has_value());
  }
}

TEST(BasicUuidV4, FromStringInvalidLowerCase) {
  std::string from = "6BBBB416-EDC3-405F-A86D-231D5800235E";
  for (int i = 0; i < 36; ++i) {
    std::string from_invalid_dash = from;
    from_invalid_dash[i] = 'a';
    EXPECT_FALSE(BasicUuidV4::FromString(from_invalid_dash).has_value());
  }
}

TEST(BasicUuidV4, HashNoCollision) {
  BasicUuidV4 uuid_1 = BasicUuidV4(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  BasicUuidV4 uuid_2 = BasicUuidV4(0xFEDCBA9876543210, 0x8899AABBCCDDEEFE);
  EXPECT_NE(uuid_1.hash(), uuid_2.hash());
}

TEST(BasicUuidV4, HashType) {
  BasicUuidV4 uuid = BasicUuidV4(0xFEDCBA9876543210, 0x8899AABBCCDDEEFF);
  std::unordered_map<BasicUuidV4, std::string> my_map = {{uuid, "Hello World"}};
  EXPECT_EQ(my_map[uuid], "Hello World");
}

TEST(BasicUuidV4Generator, GenerateUUIDUsingMt199937) {
  BasicUuidV4Generator<std::mt19937> generator;
  BasicUuidV4 uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, BasicUuidV4());
}

TEST(BasicUuidV4Generator, GenerateUUIDUsingMt199937_64) {
  BasicUuidV4Generator<std::mt19937_64> generator;
  BasicUuidV4 uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, BasicUuidV4());
}

} // namespace andyccs

#include "uuid_generator.h"

#include <gtest/gtest.h>
#include <random>

#include "uuid_basic.h"
#include "uuid_simd.h"

namespace andyccs {

TEST(UuidGenerator, GenerateDefaultUUID) {
  UuidGenerator generator;
  Uuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, Uuid());
}

TEST(UuidGenerator, Copyable) {
  UuidGenerator<std::mt19937, BasicUuid, false> generator;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
  UuidGenerator<std::mt19937, BasicUuid, false> copy1(generator);
  UuidGenerator<std::mt19937, BasicUuid, false> copy2 = generator;
#pragma clang diagnostic pop
}

TEST(UuidGenerator, Moveable) {
  UuidGenerator<std::mt19937, BasicUuid, false> generator;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
  UuidGenerator<std::mt19937, BasicUuid, false> move1(std::move(generator));
  UuidGenerator<std::mt19937, BasicUuid, false> move2 = std::move(generator);
#pragma clang diagnostic pop
}

TEST(UuidGenerator, GenerateBasicUUIDUsingMt199937) {
  UuidGenerator<std::mt19937, BasicUuid, false> generator;
  BasicUuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, BasicUuid());
}

TEST(UuidGenerator, GenerateBasicUUIDUsingMt199937_64) {
  UuidGenerator<std::mt19937_64, BasicUuid, false> generator;
  BasicUuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, BasicUuid());
}

TEST(UuidGenerator, GenerateSimdUUIDUsingMt199937) {
  UuidGenerator<std::mt19937, SimdUuid, false> generator;
  SimdUuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, SimdUuid());
}

TEST(UuidGenerator, GenerateSimdUUIDUsingMt199937_64) {
  UuidGenerator<std::mt19937_64, SimdUuid, false> generator;
  SimdUuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, SimdUuid());
}

// The following codes are not compiled because the UuidGenerator is not
// copyable or moveable when ThreadSafe is true.
#if false
TEST(UuidGenerator, NotCopyableThreadSafe) {
  UuidGenerator<std::mt19937, BasicUuid, true> generator;
  UuidGenerator<std::mt19937, BasicUuid, true> copy1(generator);
  UuidGenerator<std::mt19937, BasicUuid, true> copy2 = generator;
}

TEST(UuidGenerator, NotMoveableThreadSafe) {
  UuidGenerator<std::mt19937, BasicUuid, true> generator;
  UuidGenerator<std::mt19937, BasicUuid, true> move1(std::move(generator));
  UuidGenerator<std::mt19937, BasicUuid, true> move2 = std::move(generator);
}
#endif

TEST(UuidGenerator, GenerateBasicUUIDUsingMt199937ThreadSafe) {
  UuidGenerator<std::mt19937, BasicUuid, true> generator;
  BasicUuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, BasicUuid());
}

TEST(UuidGenerator, GenerateBasicUUIDUsingMt199937_64ThreadSafe) {
  UuidGenerator<std::mt19937_64, BasicUuid, true> generator;
  BasicUuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, BasicUuid());
}

TEST(UuidGenerator, GenerateSimdUUIDUsingMt199937ThreadSafe) {
  UuidGenerator<std::mt19937, SimdUuid, true> generator;
  SimdUuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, SimdUuid());
}

TEST(UuidGenerator, GenerateSimdUUIDUsingMt199937_64ThreadSafe) {
  UuidGenerator<std::mt19937_64, SimdUuid, true> generator;
  SimdUuid uuid = generator.GenerateUuid();
  EXPECT_NE(uuid, SimdUuid());
}

} // namespace andyccs

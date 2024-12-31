#ifndef ANDYCCS_UUID_BASIC_H
#define ANDYCCS_UUID_BASIC_H

#include <cstdint>
#include <cstdlib>
#include <optional>
#include <random>
#include <string>

namespace andyccs {

// UUID V4 has a total of 32 characters.
// Each character represents a hex value (4 bits).
// So there is a total of 32 * 4 = 128 bits of information.
//
// Example UUID V4:
// F448CB35-C484-45F2-B762-2A19E4E96ED2
class BasicUuidV4 {
public:
  // Default BasicUuidV4
  constexpr BasicUuidV4() = default;
  constexpr BasicUuidV4(uint64_t high, uint64_t low) : high_(high), low_(low) {}

  // Copyable
  BasicUuidV4(const BasicUuidV4 &other) = default;
  BasicUuidV4 &operator=(const BasicUuidV4 &other) = default;

  // Moveable
  BasicUuidV4(BasicUuidV4 &&other) = default;
  BasicUuidV4 &operator=(BasicUuidV4 &&other) = default;

  // Convert BasicUuidV4 to UUID V4 string.
  operator std::string() const;

  // Convert BasicUuidV4 to UUID V4 string and return the result in the provided
  // string ref.
  void ToString(std::string &result) const;

  // Create BasicUuidV4 from UUID V4 string.
  // The UUID V4 string must be in uppercase.
  static std::optional<BasicUuidV4> FromString(std::string_view from);

  bool operator==(const BasicUuidV4 &other) const {
    return high_ == other.high_ && low_ == other.low_;
  }

  bool operator!=(const BasicUuidV4 &other) const { return !(*this == other); }

private:
  uint64_t high_ = 0;
  uint64_t low_ = 0;
};

template <typename RNG> class BasicUuidV4Generator {
public:
  BasicUuidV4Generator()
      : generator_(RNG(std::random_device()())),
        distribution_(std::numeric_limits<uint64_t>::min(),
                      std::numeric_limits<uint64_t>::max()) {}

  // Copyable
  BasicUuidV4Generator(const BasicUuidV4Generator &other) = default;
  BasicUuidV4Generator &operator=(const BasicUuidV4Generator &other) = default;

  // Moveable
  BasicUuidV4Generator(BasicUuidV4Generator &&other) = default;
  BasicUuidV4Generator &operator=(BasicUuidV4Generator &&other) = default;

  // Not thread safe.
  BasicUuidV4 GenerateUuid() {
    return BasicUuidV4(distribution_(generator_), distribution_(generator_));
  }

private:
  RNG generator_;
  std::uniform_int_distribution<uint64_t> distribution_;
};

} // namespace andyccs

#endif // ANDYCCS_UUID_BASIC_H

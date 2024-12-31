#ifndef ANDYCCS_UUID_SIMD_H
#define ANDYCCS_UUID_SIMD_H

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
class SimdUuidV4 {
public:
  // Default SimdUuidV4
  constexpr SimdUuidV4() = default;
  constexpr SimdUuidV4(uint64_t high, uint64_t low) : high_(high), low_(low) {}

  // Copyable
  SimdUuidV4(const SimdUuidV4 &other) = default;
  SimdUuidV4 &operator=(const SimdUuidV4 &other) = default;

  // Moveable
  SimdUuidV4(SimdUuidV4 &&other) = default;
  SimdUuidV4 &operator=(SimdUuidV4 &&other) = default;

  // Convert SimdUuidV4 to UUID V4 string.
  operator std::string() const;

  // Convert SimdUuidV4 to UUID V4 string and return the result in the provided
  // string ref.
  void ToString(std::string &result) const;

  // Create SimdUuidV4 from UUID V4 string.
  // The UUID V4 string must be in uppercase.
  static std::optional<SimdUuidV4> FromString(std::string_view from);

  bool operator==(const SimdUuidV4 &other) const {
    return high_ == other.high_ && low_ == other.low_;
  }

  bool operator!=(const SimdUuidV4 &other) const { return !(*this == other); }

private:
  uint64_t high_ = 0;
  uint64_t low_ = 0;
};

template <typename RNG> class SimdUuidV4Generator {
public:
  SimdUuidV4Generator()
      : generator_(RNG(std::random_device()())),
        distribution_(std::numeric_limits<uint64_t>::min(),
                      std::numeric_limits<uint64_t>::max()) {}

  // Copyable
  SimdUuidV4Generator(const SimdUuidV4Generator &other) = default;
  SimdUuidV4Generator &operator=(const SimdUuidV4Generator &other) = default;

  // Moveable
  SimdUuidV4Generator(SimdUuidV4Generator &&other) = default;
  SimdUuidV4Generator &operator=(SimdUuidV4Generator &&other) = default;

  // Not thread safe.
  SimdUuidV4 GenerateUuid() {
    return SimdUuidV4(distribution_(generator_), distribution_(generator_));
  }

private:
  RNG generator_;
  std::uniform_int_distribution<uint64_t> distribution_;
};

} // namespace andyccs

#endif // ANDYCCS_UUID_SIMD_H

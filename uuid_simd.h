#ifndef ANDYCCS_UUID_SIMD_H
#define ANDYCCS_UUID_SIMD_H

#include <cstdint>
#include <cstdlib>
#include <optional>
#include <random>
#include <string>

namespace andyccs {

// SimdUuidV4 represents a UUID (Universally Unique Identifier) with 32
// hexadecimal characters ('0' to '9' and 'A' to 'F') and 4 dashes, formatted as
// follows:
//
// XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
//
// Each hexadecimal character represents 4 bits, resulting in a total of 32 * 4
// = 128 bits. SimdUuidV4 stores these 128 bits as two 64-bit unsigned integers,
// referred to as 'high' and 'low'.
//
// Example SimdUuidV4:
// - high = 0xF448CB35C48445F2
// - low  = 0xB7622A19E4E96ED2
// - String representation: "F448CB35-C484-45F2-B762-2A19E4E96ED2"
//
// Note: SimdUuidV4 is a simplified version of UUID V4 and does not comply with
// RFC 4122 and RFC 9562. It does not set the version and variant fields as
// specified in these RFCs.
//
// This implementation utilizes SIMD (Single Instruction, Multiple Data)
// instructions to significantly improve performance. Parsing a UUID from a
// string is 9 times faster, and creating a UUID string is 3 times faster
// compared to BasicUuidV4.
class SimdUuidV4 {
public:
  // Default constructor for SimdUuidV4
  constexpr SimdUuidV4() = default;
  constexpr SimdUuidV4(uint64_t high, uint64_t low) : high_(high), low_(low) {}
  SimdUuidV4(const std::uint8_t (&data)[16]);

  // Copy constructor and assignment operator
  SimdUuidV4(const SimdUuidV4 &other) = default;
  SimdUuidV4 &operator=(const SimdUuidV4 &other) = default;

  // Move constructor and assignment operator
  SimdUuidV4(SimdUuidV4 &&other) = default;
  SimdUuidV4 &operator=(SimdUuidV4 &&other) = default;

  // Convert SimdUuidV4 to UUID V4 string.
  // This function is marked explicit to prevent accidental conversion to
  // string.
  explicit operator std::string() const;

  // Convert SimdUuidV4 to UUID V4 string and store the result in the provided
  // string reference.
  void ToString(std::string &result) const;

  // Create SimdUuidV4 from a UUID V4 string.
  // The UUID V4 string must be in uppercase.
  static std::optional<SimdUuidV4> FromString(std::string_view from);

  // Equality operators
  bool operator==(const SimdUuidV4 &other) const {
    return high_ == other.high_ && low_ == other.low_;
  }

  bool operator!=(const SimdUuidV4 &other) const { return !(*this == other); }

  // Compute hash value for SimdUuidV4
  size_t hash() const;

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

template <> struct std::hash<andyccs::SimdUuidV4> {
  size_t operator()(const andyccs::SimdUuidV4 &uuid) const noexcept {
    return uuid.hash();
  }
};

#endif // ANDYCCS_UUID_SIMD_H

#ifndef ANDYCCS_UUID_SIMD_H
#define ANDYCCS_UUID_SIMD_H

#include <array>
#include <cstdint>
#include <cstdlib>
#include <optional>
#include <random>
#include <string>

namespace andyccs {

// SimdUuid represents a UUID (Universally Unique Identifier) with 32
// hexadecimal characters ('0' to '9' and 'A' to 'F') and 4 dashes, formatted as
// follows:
//
// XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
//
// Each hexadecimal character represents 4 bits, resulting in a total of 32 * 4
// = 128 bits.
//
// Example SimdUuid:
// - data = 0xF448CB35C48445F2B7622A19E4E96ED2
// - String representation: "F448CB35-C484-45F2-B762-2A19E4E96ED2"
//
// Note: SimdUuid is a simplified version of UUID V4 and does not comply with
// RFC 9562. It does not set the version and variant fields as specified in
// these RFCs.
//
// This implementation utilizes SIMD (Single Instruction, Multiple Data)
// instructions to significantly improve performance. Parsing a UUID from a
// string is 9 times faster, and creating a UUID string is 3 times faster
// compared to BasicUuid.
class SimdUuid {
public:
  // Default constructor for SimdUuid
  SimdUuid() = default;

  // Construct SimdUuid from two uint64_t. Example:
  // - high = 0xF448CB35C48445F2
  // - low  = 0xB7622A19E4E96ED2
  // - String representation: "F448CB35-C484-45F2-B762-2A19E4E96ED2"
  SimdUuid(uint64_t high, uint64_t low);

  // Construct BasicUuidV4 from a 16-byte array. Example:
  // - data = {
  //     "0xF4", "0x48", "0xCB", "0x35", "0xC4", "0x84", "0x45", "0xF2",
  //     "0xB7", "0x62", "0x2A", "0x19", "0xE4", "0xE9", "0x6E", "0xD2"
  //   }
  // - high = 0xF448CB35C48445F2
  // - low  = 0xB7622A19E4E96ED2
  // - String representation: "F448CB35-C484-45F2-B762-2A19E4E96ED2"
  SimdUuid(const std::uint8_t (&data)[16]);

  // Same as above, but with std::array instead of C-style array
  constexpr SimdUuid(std::array<std::uint8_t, 16> data) : data_(data) {}

  // Copy constructor and assignment operator
  SimdUuid(const SimdUuid &other) = default;
  SimdUuid &operator=(const SimdUuid &other) = default;

  // Move constructor and assignment operator
  SimdUuid(SimdUuid &&other) {
    if (this != &other) {
      data_ = std::move(other.data_);
      other.data_ = {0};
    }
  }
  SimdUuid &operator=(SimdUuid &&other) {
    if (this != &other) {
      data_ = std::move(other.data_);
      other.data_ = {0};
    }
    return *this;
  }

  // Convert SimdUuid to UUID V4 string.
  // This function is marked explicit to prevent accidental conversion to
  // string.
  explicit operator std::string() const;

  // Convert SimdUuid to UUID V4 string and store the result in the provided
  // string reference.
  void ToString(std::string &result) const;

  // Convert BasicUuidV4 to UUID V4 string and store the result in the provided
  // buffer.
  void ToChars(char (&buffer)[37]) const;

  // Create SimdUuid from a UUID V4 string.
  // The UUID V4 string must be in uppercase.
  static std::optional<SimdUuid> FromString(std::string_view from);

  // Equality operators
  bool operator==(const SimdUuid &other) const { return data_ == other.data_; }

  bool operator!=(const SimdUuid &other) const { return !(*this == other); }

  // Compute hash value for SimdUuid
  size_t hash() const;

private:
  std::array<std::uint8_t, 16> data_ = {0};
};

template <typename RNG> class SimdUuidGenerator {
public:
  SimdUuidGenerator()
      : generator_(RNG(std::random_device()())),
        distribution_(std::numeric_limits<uint64_t>::min(),
                      std::numeric_limits<uint64_t>::max()) {}

  // Copyable
  SimdUuidGenerator(const SimdUuidGenerator &other) = default;
  SimdUuidGenerator &operator=(const SimdUuidGenerator &other) = default;

  // Moveable
  SimdUuidGenerator(SimdUuidGenerator &&other) = default;
  SimdUuidGenerator &operator=(SimdUuidGenerator &&other) = default;

  // Not thread safe.
  SimdUuid GenerateUuid() {
    std::array<uint8_t, 16> data;
    *reinterpret_cast<uint64_t *>(data.data()) = distribution_(generator_);
    *reinterpret_cast<uint64_t *>(data.data() + 8) = distribution_(generator_);
    return SimdUuid(data);
  }

private:
  RNG generator_;
  std::uniform_int_distribution<uint64_t> distribution_;
};

} // namespace andyccs

template <> struct std::hash<andyccs::SimdUuid> {
  size_t operator()(const andyccs::SimdUuid &uuid) const noexcept {
    return uuid.hash();
  }
};

#endif // ANDYCCS_UUID_SIMD_H

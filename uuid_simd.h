#ifndef ANDYCCS_UUID_SIMD_H
#define ANDYCCS_UUID_SIMD_H

#include <array>
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
// = 128 bits.
//
// Example SimdUuidV4:
// - data = 0xF448CB35C48445F2B7622A19E4E96ED2
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
  SimdUuidV4() = default;

  // Construct SimdUuidV4 from two uint64_t. Example:
  // - high = 0xF448CB35C48445F2
  // - low  = 0xB7622A19E4E96ED2
  // - String representation: "F448CB35-C484-45F2-B762-2A19E4E96ED2"
  SimdUuidV4(uint64_t high, uint64_t low);

  // Construct BasicUuidV4 from a 16-byte array. Example:
  // - data = {
  //     "0xF4", "0x48", "0xCB", "0x35", "0xC4", "0x84", "0x45", "0xF2",
  //     "0xB7", "0x62", "0x2A", "0x19", "0xE4", "0xE9", "0x6E", "0xD2"
  //   }
  // - high = 0xF448CB35C48445F2
  // - low  = 0xB7622A19E4E96ED2
  // - String representation: "F448CB35-C484-45F2-B762-2A19E4E96ED2"
  SimdUuidV4(const std::uint8_t (&data)[16]);

  // Same as above, but with std::array instead of C-style array
  constexpr SimdUuidV4(std::array<std::uint8_t, 16> data) : data_(data) {}

  // Copy constructor and assignment operator
  SimdUuidV4(const SimdUuidV4 &other) = default;
  SimdUuidV4 &operator=(const SimdUuidV4 &other) = default;

  // Move constructor and assignment operator
  SimdUuidV4(SimdUuidV4 &&other) {
    if (this != &other) {
      data_ = std::move(other.data_);
      other.data_ = {0};
    }
  }
  SimdUuidV4 &operator=(SimdUuidV4 &&other) {
    if (this != &other) {
      data_ = std::move(other.data_);
      other.data_ = {0};
    }
    return *this;
  }

  // Convert SimdUuidV4 to UUID V4 string.
  // This function is marked explicit to prevent accidental conversion to
  // string.
  explicit operator std::string() const;

  // Convert SimdUuidV4 to UUID V4 string and store the result in the provided
  // string reference.
  void ToString(std::string &result) const;

  // Convert BasicUuidV4 to UUID V4 string and store the result in the provided
  // buffer.
  void ToChars(char (&buffer)[37]) const;

  // Create SimdUuidV4 from a UUID V4 string.
  // The UUID V4 string must be in uppercase.
  static std::optional<SimdUuidV4> FromString(std::string_view from);

  // Equality operators
  bool operator==(const SimdUuidV4 &other) const {
    return data_ == other.data_;
  }

  bool operator!=(const SimdUuidV4 &other) const { return !(*this == other); }

  // Compute hash value for SimdUuidV4
  size_t hash() const;

private:
  std::array<std::uint8_t, 16> data_ = {0};
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
    std::array<uint8_t, 16> data;
    *reinterpret_cast<uint64_t *>(data.data()) = distribution_(generator_);
    *reinterpret_cast<uint64_t *>(data.data() + 8) = distribution_(generator_);
    return SimdUuidV4(data);
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

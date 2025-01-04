#ifndef ANDYCCS_UUID_BASIC_H
#define ANDYCCS_UUID_BASIC_H

#include <array>
#include <cstdint>
#include <cstdlib>
#include <optional>
#include <random>
#include <string>

namespace andyccs {

// BasicUuid represents a UUID (Universally Unique Identifier) with 32
// hexadecimal characters ('0' to '9' and 'A' to 'F') and 4 dashes, formatted as
// follows:
//
// XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
//
// Each hexadecimal character represents 4 bits, resulting in a total of 32 * 4
// = 128 bits.
//
// Example BasicUuid:
// - data = 0xF448CB35C48445F2B7622A19E4E96ED2
// - String representation: "F448CB35-C484-45F2-B762-2A19E4E96ED2"
//
// Note: BasicUuid is a simplified version of UUID V4 and does not comply with
// RFC 9562. It does not set the version and variant fields as specified in
// these RFCs.
class BasicUuid {
public:
  // Default constructor for BasicUuid
  BasicUuid() = default;

  // Construct BasicUuid from two uint64_t. Example:
  // - high = 0xF448CB35C48445F2
  // - low  = 0xB7622A19E4E96ED2
  // - String representation: "F448CB35-C484-45F2-B762-2A19E4E96ED2"
  BasicUuid(uint64_t high, uint64_t low);

  // Construct BasicUuid from a 16-byte array. Example:
  // - data = {
  //     "0xF4", "0x48", "0xCB", "0x35", "0xC4", "0x84", "0x45", "0xF2",
  //     "0xB7", "0x62", "0x2A", "0x19", "0xE4", "0xE9", "0x6E", "0xD2"
  //   }
  // - high = 0xF448CB35C48445F2
  // - low  = 0xB7622A19E4E96ED2
  // - String representation: "F448CB35-C484-45F2-B762-2A19E4E96ED2"
  BasicUuid(const std::uint8_t (&data)[16]);

  // Same as above, but with std::array instead of C-style array
  constexpr BasicUuid(std::array<std::uint8_t, 16> data) : data_(data) {}

  // Copy constructor and assignment operator
  BasicUuid(const BasicUuid &other) = default;
  BasicUuid &operator=(const BasicUuid &other) = default;

  // Move constructor and assignment operator
  BasicUuid(BasicUuid &&other) {
    if (this != &other) {
      data_ = std::move(other.data_);
      other.data_ = {0};
    }
  }
  BasicUuid &operator=(BasicUuid &&other) {
    if (this != &other) {
      data_ = std::move(other.data_);
      other.data_ = {0};
    }
    return *this;
  }

  // Convert BasicUuid to UUID V4 string.
  // This function is marked explicit to prevent accidental conversion to
  // string.
  explicit operator std::string() const;

  // Convert BasicUuid to UUID V4 string and store the result in the provided
  // string reference.
  void ToString(std::string &result) const;

  // Convert BasicUuid to UUID V4 string and store the result in the provided
  // buffer.
  void ToChars(char (&buffer)[37]) const;

  // Create BasicUuid from a UUID V4 string.
  // The UUID V4 string must be in uppercase.
  static std::optional<BasicUuid> FromString(std::string_view from);

  // Equality operators
  bool operator==(const BasicUuid &other) const { return data_ == other.data_; }

  bool operator!=(const BasicUuid &other) const { return !(*this == other); }

  // Compute hash value for BasicUuid
  size_t hash() const;

private:
  std::array<std::uint8_t, 16> data_ = {0};
};

template <typename RNG> class BasicUuidGenerator {
public:
  // Constructor initializes the random number generator and distribution
  BasicUuidGenerator()
      : generator_(RNG(std::random_device()())),
        distribution_(std::numeric_limits<uint64_t>::min(),
                      std::numeric_limits<uint64_t>::max()) {}

  // Copy constructor and assignment operator
  BasicUuidGenerator(const BasicUuidGenerator &other) = default;
  BasicUuidGenerator &operator=(const BasicUuidGenerator &other) = default;

  // Move constructor and assignment operator
  BasicUuidGenerator(BasicUuidGenerator &&other) = default;
  BasicUuidGenerator &operator=(BasicUuidGenerator &&other) = default;

  // Generate a new BasicUuid. Note: This function is not thread-safe.
  BasicUuid GenerateUuid() {
    std::array<uint8_t, 16> data;
    *(uint64_t *)data.data() = distribution_(generator_);
    *(uint64_t *)(data.data() + 8) = distribution_(generator_);
    return BasicUuid(data);
  }

private:
  RNG generator_;
  std::uniform_int_distribution<uint64_t> distribution_;
};

} // namespace andyccs

// Specialization of std::hash for BasicUuid
template <> struct std::hash<andyccs::BasicUuid> {
  size_t operator()(const andyccs::BasicUuid &uuid) const noexcept {
    return uuid.hash();
  }
};

#endif // ANDYCCS_UUID_BASIC_H
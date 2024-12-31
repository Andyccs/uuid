#ifndef ANDYCCS_UUID_BASIC_H
#define ANDYCCS_UUID_BASIC_H

#include <cstdint>
#include <cstdlib>
#include <optional>
#include <random>
#include <string>

namespace andyccs {

// BasicUuidV4 represents a UUID (Universally Unique Identifier) with 32
// hexadecimal characters ('0' to '9' and 'A' to 'F') and 4 dashes, formatted as
// follows:
//
// XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
//
// Each hexadecimal character represents 4 bits, resulting in a total of 32 * 4
// = 128 bits. BasicUuidV4 stores these 128 bits as two 64-bit unsigned
// integers, referred to as 'high' and 'low'.
//
// Example BasicUuidV4:
// - high = 0xF448CB35C48445F2
// - low  = 0xB7622A19E4E96ED2
// - String representation: "F448CB35-C484-45F2-B762-2A19E4E96ED2"
//
// Note: BasicUuidV4 is a simplified version of UUID V4 and does not comply with
// RFC 4122 and RFC 9562. It does not set the version and variant fields as
// specified in these RFCs.
class BasicUuidV4 {
public:
  // Default constructor for BasicUuidV4
  constexpr BasicUuidV4() = default;
  constexpr BasicUuidV4(uint64_t high, uint64_t low) : high_(high), low_(low) {}

  // Copy constructor and assignment operator
  BasicUuidV4(const BasicUuidV4 &other) = default;
  BasicUuidV4 &operator=(const BasicUuidV4 &other) = default;

  // Move constructor and assignment operator
  BasicUuidV4(BasicUuidV4 &&other) = default;
  BasicUuidV4 &operator=(BasicUuidV4 &&other) = default;

  // Convert BasicUuidV4 to UUID V4 string.
  // This function is marked explicit to prevent accidental conversion to
  // string.
  explicit operator std::string() const;

  // Convert BasicUuidV4 to UUID V4 string and store the result in the provided
  // string reference.
  void ToString(std::string &result) const;

  // Create BasicUuidV4 from a UUID V4 string.
  // The UUID V4 string must be in uppercase.
  static std::optional<BasicUuidV4> FromString(std::string_view from);

  // Equality operators
  bool operator==(const BasicUuidV4 &other) const {
    return high_ == other.high_ && low_ == other.low_;
  }

  bool operator!=(const BasicUuidV4 &other) const { return !(*this == other); }

  // Compute hash value for BasicUuidV4
  size_t hash() const;

private:
  uint64_t high_ = 0;
  uint64_t low_ = 0;
};

template <typename RNG> class BasicUuidV4Generator {
public:
  // Constructor initializes the random number generator and distribution
  BasicUuidV4Generator()
      : generator_(RNG(std::random_device()())),
        distribution_(std::numeric_limits<uint64_t>::min(),
                      std::numeric_limits<uint64_t>::max()) {}

  // Copy constructor and assignment operator
  BasicUuidV4Generator(const BasicUuidV4Generator &other) = default;
  BasicUuidV4Generator &operator=(const BasicUuidV4Generator &other) = default;

  // Move constructor and assignment operator
  BasicUuidV4Generator(BasicUuidV4Generator &&other) = default;
  BasicUuidV4Generator &operator=(BasicUuidV4Generator &&other) = default;

  // Generate a new BasicUuidV4. Note: This function is not thread-safe.
  BasicUuidV4 GenerateUuid() {
    return BasicUuidV4(distribution_(generator_), distribution_(generator_));
  }

private:
  RNG generator_;
  std::uniform_int_distribution<uint64_t> distribution_;
};

} // namespace andyccs

// Specialization of std::hash for BasicUuidV4
template <> struct std::hash<andyccs::BasicUuidV4> {
  size_t operator()(const andyccs::BasicUuidV4 &uuid) const noexcept {
    return uuid.hash();
  }
};

#endif // ANDYCCS_UUID_BASIC_H
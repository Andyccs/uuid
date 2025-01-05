#ifndef ANDYCCS_UUID_GENERATOR_H
#define ANDYCCS_UUID_GENERATOR_H

#if _WIN64 || (__GNUC__ && __x86_64__)
#define ANDYCCS_IS_64_BITS
#endif

#if __AVX2__
#define ANDYCCS_HAS_AVX2
#endif

#include <mutex>
#include <random>
#include <variant>

#include "uuid_basic.h"
#ifdef ANDYCCS_HAS_AVX2
#include "uuid_simd.h"
#endif // __AVX2__

namespace andyccs {

using DefaultRNG =
#ifdef ANDYCCS_IS_64_BITS
    std::mt19937_64
#else
    std::mt19337
#endif
    ;

using Uuid =
#ifdef ANDYCCS_HAS_AVX2
    SimdUuid
#else
    BasicUuid
#endif
    ;

template <class RNG = DefaultRNG, class UuidT = Uuid, bool ThreadSafe = true>
class UuidGenerator {
public:
  UuidGenerator()
      : generator_(RNG(std::random_device()())),
        distribution_(std::numeric_limits<uint64_t>::min(),
                      std::numeric_limits<uint64_t>::max()) {}

  // Copyable. Only available when ThreadSafe is false.
  UuidGenerator(const UuidGenerator &other)
    requires(!ThreadSafe)
  = default;
  UuidGenerator &operator=(const UuidGenerator &other)
    requires(!ThreadSafe)
  = default;

  // Moveable
  UuidGenerator(UuidGenerator &&other)
    requires(!ThreadSafe)
  = default;
  UuidGenerator &operator=(UuidGenerator &&other)
    requires(!ThreadSafe)
  = default;

  UuidT GenerateUuid() {
    if constexpr (ThreadSafe) {
      std::lock_guard<std::mutex> lock(mutex_);
    }
    std::array<uint8_t, 16> data;
    *reinterpret_cast<uint64_t *>(data.data()) = distribution_(generator_);
    *reinterpret_cast<uint64_t *>(data.data() + 8) = distribution_(generator_);
    return UuidT(data);
  }

private:
  RNG generator_;
  std::uniform_int_distribution<uint64_t> distribution_;

  // Conditional creation.
  // The size of UuidGenerator is 2520 bytes without this field.
  // With this field, the size of UuidGenerator is 2528 bytes when ThreadSafe is
  // false. If ThreadSafe is true, it requires an additional 32 bytes, making
  // the total size 2560 bytes.
  std::conditional_t<ThreadSafe, std::mutex, std::monostate> mutex_;
};

} // namespace andyccs

#endif // ANDYCCS_UUID_GENERATOR_H

#ifndef ANDYCCS_UUID_BENCHMARK_UTILS_H
#define ANDYCCS_UUID_BENCHMARK_UTILS_H

#include <random>

namespace andyccs {

// Generate random data each time to prevent the compiler from optimizing the
// code to merely load and store constant values in the char array. This
// ensures that the benchmark accurately measures the performance of the
// to_chars function.
template <typename T> void GenerateRandomData(T &data) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<std::uint8_t> dis(0, 255);
  for (auto &byte : data) {
    byte = dis(gen);
  }
}

} // namespace andyccs

#endif // ANDYCCS_UUID_BENCHMARK_UTILS_H
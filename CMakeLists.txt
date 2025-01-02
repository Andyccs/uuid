cmake_minimum_required(VERSION 3.15)

project(Andyccs VERSION 1.0)

#### andyccs_compiler_flags

add_library(andyccs_compiler_flags INTERFACE)
target_compile_features(andyccs_compiler_flags INTERFACE cxx_std_23)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(andyccs_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

#### General configurations

# Keep the assembly file around for inspection.
# -mavx2 is to enable AVX2 instruction set.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -save-temps")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps -mavx2")

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(AndyccsConfig.h.in AndyccsConfig.h)

#### Dependencies

include(FetchContent)

# GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

# GoogleBenchmark
FetchContent_Declare(
  googlebenchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.9.1.zip
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(googlebenchmark)

#### Local libraries

# benchmark utils library
add_library(uuid_benchmark_utils uuid_benchmark_utils.h)
set_target_properties(uuid_benchmark_utils PROPERTIES LINKER_LANGUAGE CXX)

# add the uuid_basic library
add_library(uuid_basic uuid_basic.h uuid_basic.cc)
add_executable(uuid_basic_test uuid_basic_test.cc)
target_link_libraries(uuid_basic_test uuid_basic GTest::gtest_main andyccs_compiler_flags)
gtest_discover_tests(uuid_basic_test)

add_executable(uuid_basic_benchmark_test uuid_basic_benchmark_test.cc)
target_link_libraries(uuid_basic_benchmark_test uuid_basic uuid_benchmark_utils benchmark::benchmark andyccs_compiler_flags)

# add the uuid_simd library
add_library(uuid_simd uuid_simd.h uuid_simd.cc)
add_executable(uuid_simd_test uuid_simd_test.cc)
target_link_libraries(uuid_simd_test uuid_simd GTest::gtest_main andyccs_compiler_flags)
gtest_discover_tests(uuid_simd_test)

add_executable(uuid_simd_benchmark_test uuid_simd_benchmark_test.cc)
target_link_libraries(uuid_simd_benchmark_test uuid_simd uuid_benchmark_utils benchmark::benchmark andyccs_compiler_flags)

# benchmark of other libraries
FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.87.0/boost-1.87.0-cmake.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set(BOOST_INCLUDE_LIBRARIES uuid)
FetchContent_MakeAvailable(Boost)

add_library(yesmey_uuid vendor/yesmey/UUID.h)
set_target_properties(yesmey_uuid PROPERTIES LINKER_LANGUAGE CXX)

add_executable(uuid_benchmark_test uuid_benchmark_test.cc)
target_link_libraries(uuid_benchmark_test uuid_benchmark_utils yesmey_uuid benchmark::benchmark Boost::uuid andyccs_compiler_flags)

#### Executable

# add the executable
add_executable(Andyccs main.cc)
target_link_libraries(Andyccs PUBLIC uuid_basic uuid_simd yesmey_uuid Boost::uuid andyccs_compiler_flags)

# add the binary tree to the search path for include files
# so that we will find AndyccsConfig.h
target_include_directories(Andyccs PUBLIC "${PROJECT_BINARY_DIR}")

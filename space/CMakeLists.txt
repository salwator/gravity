cmake_minimum_required (VERSION 2.6)
project (Gravity)

SET (CMAKE_CXX_COMPILER   "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS      "-Wall -std=c++11 -O3")

include_directories("../Include")
include_directories("../gtest-1.7.0")

add_library(space SHARED Vector.cpp Planet.cpp)

# TESTING

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(SpaceUT space_ut.cpp)
target_link_libraries(SpaceUT space gtest gtest_main)

# add_test(SimulationTest SimulationUT)


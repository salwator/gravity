cmake_minimum_required (VERSION 2.6)
project (Gravity)

SET (CMAKE_CXX_COMPILER   "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS      "-Wall -std=c++11 -O3")

include_directories("../Include")
include_directories("../gtest-1.7.0")

add_library(simulate SHARED simulate.cpp)

# TESTING

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(SimulationUT simulation_ut.cpp)
target_link_libraries(SimulationUT simulate gtest gtest_main)

add_test(SimulationTest SimulationUT)


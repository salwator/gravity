cmake_minimum_required (VERSION 2.6)
project (Gravity)

SET (CMAKE_C_COMPILER   "/usr/bin/clang")
SET (CMAKE_C_FLAGS      "-Wall -std=c99")

SET (CMAKE_CXX_COMPILER   "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS      "-Wall -std=c++11 -O3")


# LIBS

include_directories(Include)

add_library(simulate SHARED simulate.cpp)

# MAIN

add_executable(Gravity gravity.cpp)
target_link_libraries(Gravity simulate)

# TESTING

ADD_SUBDIRECTORY(gtest-1.7.0)

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(GravityUT gravity_ut.cpp)
target_link_libraries(GravityUT simulate gtest gtest_main)

add_test(SimulationTest GravityUT)



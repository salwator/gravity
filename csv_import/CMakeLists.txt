cmake_minimum_required (VERSION 2.6)
project (Gravity)

SET (CMAKE_CXX_COMPILER   "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS      "-Wall -std=c++11 -O3")

include_directories("../space")

add_library(csv_import SHARED csv_import.cpp)

# TESTING

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(CsvImportUT csv_import_ut.cpp)
target_link_libraries(CsvImportUT space csv_import gtest gtest_main)

add_test(CsvImportTest CsvImportUT)


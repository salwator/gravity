cmake_minimum_required (VERSION 2.6)
project (Gravity)

SET (CMAKE_C_COMPILER   "/usr/bin/clang")
SET (CMAKE_C_FLAGS      "-Wall -std=c99")

SET (CMAKE_CXX_COMPILER   "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS      "-Wall -std=c++11 -O3")


# LIBS

include_directories(Include)

ADD_SUBDIRECTORY(simulation)
ADD_SUBDIRECTORY(csv_import)

# MAIN

add_executable(Gravity gravity.cpp)
target_link_libraries(Gravity simulate)

# TESTING

enable_testing()

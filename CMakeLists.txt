cmake_minimum_required (VERSION 2.6)
project (Gravity)

SET (CMAKE_C_FLAGS      "-Wall -std=c99")
SET (CMAKE_CXX_FLAGS      "-Wall -std=c++11 -O3")

# GTEST + GMOCK
IF (DEFINED ENV{GTEST_PATH})
      SET(GTEST_PATH ENV{GTEST_PATH})
  ELSE()
        SET(GTEST_PATH ../gmock-1.7.0/gtest)
    ENDIF (DEFINED ENV{GTEST_PATH})

IF (DEFINED ENV{GMOCK_PATH})
  SET(GMOCK_PATH ENV{GMOCK_PATH})
ELSE()
    SET(GMOCK_PATH ../gmock-1.7.0)
ENDIF (DEFINED ENV{GMOCK_PATH})

# LIBS

include_directories(Include)
include_directories(space)
include_directories(simulation)
include_directories("external/glfw/include")

ADD_SUBDIRECTORY(space)
ADD_SUBDIRECTORY(simulation)
ADD_SUBDIRECTORY(csv_import)
ADD_SUBDIRECTORY("external/glfw")

# MAIN

add_executable(Gravity gravity.cpp GlViz.cpp)
target_link_libraries(Gravity space newton_simulator csv_import)
target_link_libraries(Gravity glfw ${GLFW_LIBRARIES})

# TESTING

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

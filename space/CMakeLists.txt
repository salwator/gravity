add_library(space SHARED Vector.cpp Planet.cpp)

# TESTING

add_executable(SpaceUT space_ut.cpp)
target_link_libraries(SpaceUT space gtest gtest_main)

# add_test(SimulationTest SimulationUT)


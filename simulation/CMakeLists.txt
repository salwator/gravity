add_library(simulate SHARED simulate.cpp)

# TESTING

add_executable(SimulationUT simulation_ut.cpp)
target_link_libraries(SimulationUT simulate space gtest gtest_main)

add_test(SimulationTest SimulationUT)


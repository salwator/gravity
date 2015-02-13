add_library(simulate SHARED simulate.cpp)
add_library(newton_simulator SHARED newton_simulator.cpp)

target_link_libraries(newton_simulator simulate)

# TESTING

add_library(simulate_mock SHARED simulate_mock.cpp)

add_executable(SimulationUT simulation_ut.cpp)

target_link_libraries(SimulationUT simulate space gtest gtest_main)

add_test(SimulationTest SimulationUT)

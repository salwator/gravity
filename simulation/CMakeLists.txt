add_library(simulate SHARED simulate.cpp)
add_library(newton_simulator SHARED newton_simulator.cpp)

# TESTING

add_library(simulate_mock SHARED simulate_mock.cpp)

add_executable(SimulationUT simulation_ut.cpp)
add_executable(NewtonSimulatorUT newton_simulator_ut.cpp)

target_link_libraries(SimulationUT simulate space gtest gtest_main)
target_link_libraries(NewtonSimulatorUT newton_simulator simulate_mock space gtest gtest_main gmock)

add_test(SimulationTest SimulationUT)
add_test(SimulationTest NewtonSimulatorUT)

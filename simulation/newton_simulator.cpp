#include <iostream>
#include <thread>
#include "newton_simulator.h"

NewtonSimulator::NewtonSimulator(Planets planets, double dt)
    :
    planets(planets),
    dt(dt)
{}

void NewtonSimulator::simulate(units::base_time simulation_step_time)
{
    for(auto t = units::base_time(0); t < simulation_step_time; t+=dt)
    {
        auto num_of_threads = std::thread::hardware_concurrency();
        if(num_of_threads < 2)
            planets = ::simulate(planets, dt);
        else
            planets = ::simulate_threaded(planets, dt, num_of_threads);

        total_time += dt;
    }
}

const Planets & NewtonSimulator::result()
{
    return planets;
}

units::base_time NewtonSimulator::time()
{
    return total_time;
}


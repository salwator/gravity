#include <iostream>
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
        planets = ::simulate(planets, dt);
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


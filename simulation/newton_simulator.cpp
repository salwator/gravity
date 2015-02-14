#include <iostream>
#include "newton_simulator.h"

NewtonSimulator::NewtonSimulator(Planets planets, double dt)
    :
    planets(planets),
    dt(dt)
{}

void NewtonSimulator::simulate(double simulation_step_time)
{
    for(auto t = 0.0; t < simulation_step_time; t+=dt)
    {
        planets = ::simulate(planets, dt);
        total_time += dt;
    }
}

const Planets & NewtonSimulator::result()
{
    return planets;
}

double NewtonSimulator::time()
{
    return total_time;
}


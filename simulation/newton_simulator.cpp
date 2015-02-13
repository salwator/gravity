#include <iostream>
#include "newton_simulator.h"

NewtonSimulator::NewtonSimulator(Planets planets, double dt)
    :
    planets(planets),
    dt(dt)
{}

void NewtonSimulator::simulate(int iterations)
{
    for(auto i = 0; i < iterations; i++)
        planets = ::simulate(planets, dt);
}

const Planets & NewtonSimulator::result()
{
    return planets;
}



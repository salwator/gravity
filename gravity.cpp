#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>

#include "space.h"
#include "newton_simulator.h"

namespace
{

Planet on_others_orbit(Planet& central, double distance, double mass)
{
    return Planet(mass, Vector(central.x, central.y + distance),
                  Vector(sqrt((G*central.mass) / distance) + central.vx,
                         central.vy));
}

Planets sample_planets()
{
    auto planets = Planets();
    planets.push_back(Planet(20000000,Vector(0,0),Vector(0,0)));
    planets.push_back(on_others_orbit(planets[0], 1000, 10000));
    planets.push_back(on_others_orbit(planets[1], 20, 10));
    return planets;
}

void print_results(ISimulator & simulation)
{
    for(const auto & planet : simulation.result())
    {
        std::cout << planet.x << "," << planet.y << ",";
    }

    std::cout << std::endl;
}

}

int main()
{
    if(not glfwInit())
        exit(EXIT_FAILURE);

    const auto time_delta = 10;
    const auto simulation_steps = 10;
    const auto verbose = false;


    auto simulation = NewtonSimulator(sample_planets(), time_delta);

    for(auto i = 0; i < 100000; i++)
    {
        simulation.simulate(simulation_steps);

        if(verbose)
            print_results(simulation);
    }

}

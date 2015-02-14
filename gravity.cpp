#include <iostream>
#include <cmath>
#include <ratio>
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
    using namespace units;

    auto planets = Planets();
    planets.push_back(Planet(sun_mass,Vector(0,0),Vector(0,0)));
    planets.push_back(on_others_orbit(planets[0], M, au));
    planets.push_back(on_others_orbit(planets[1], 0.0123 * M, 300000 * kilometer));
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

    const auto time_delta = units::minute;
    const auto print_interval = units::day;
    const auto simulation_time = units::year;
    const auto verbose = true;


    auto simulation = NewtonSimulator(sample_planets(), time_delta);

    while(simulation.time() < simulation_time)
    {
        simulation.simulate(print_interval);

        if(verbose)
            print_results(simulation);
    }

}

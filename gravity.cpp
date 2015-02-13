#include <iostream>
#include <cmath>

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

}

int main()
{
    auto simulation = NewtonSimulator(sample_planets(), 10);

    for(auto i = 0; i < 100000; i++)
    {
        simulation.simulate(10);

        for(const auto & planet : simulation.result())
        {
            std::cout << planet.x << "," << planet.y << ",";
        }

        std::cout << std::endl;
    }

}

#include <iostream>
#include <cmath>

#include "space.h"
#include "newton_simulator.h"

Planet on_others_orbit(Planet& central, double distance, double mass)
{
    auto planet = Planet(mass, Vector(central.x, central.y + distance), Vector(sqrt((G*central.mass)/distance)+central.vx,central.vy));
    return planet;
}

int main()
{
    auto planets = Planets();
    planets.push_back(Planet(10000000,Vector(0,0),Vector(0,0)));
    planets.push_back(on_others_orbit(planets[0], 1000, 10000));
    planets.push_back(on_others_orbit(planets[1], 20, 10));

    auto simulation = NewtonSimulator(planets, 10);

    auto i = 0;
    while(i++ < 100000)
    {
        simulation.simulate(10);
        for(const auto & planet : simulation.result())
        {
            std::cout << planet.x << "," << planet.y << ",";
        }
    std::cout << std::endl;
    }

}

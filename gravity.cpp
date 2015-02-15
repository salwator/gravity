#include <iostream>
#include <cmath>
#include <ratio>

#include "space.h"
#include "newton_simulator.h"
#include "GlViz.h"

namespace
{

Planet on_others_orbit(Planet& central, units::base_space distance, units::base_type mass)
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
    planets.push_back(on_others_orbit(planets[0], au, M));
    planets.push_back(on_others_orbit(planets[1], 384403 * kilometer, 0.0123 * M));
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

} // namespace

int main()
{
    auto viz = GlViz(1.2*units::au, 1.2*units::au);

    const auto time_delta = units::second * 10;
    const auto print_interval = units::day;
    const auto simulation_time = units::year * 10;
    const auto verbose = false;


    auto simulation = NewtonSimulator(sample_planets(), time_delta);

    while(simulation.time() < simulation_time)
    {
        simulation.simulate(print_interval);

        viz.print(simulation.result());
        if(verbose)
            print_results(simulation);
    }
}


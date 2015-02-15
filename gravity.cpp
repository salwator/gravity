#include <iostream>
#include <cmath>
#include <ratio>
#include <random>

#include "space.h"
#include "newton_simulator.h"
#include "GlViz.h"

namespace
{

constexpr double pi() { return std::atan(1)*4; }

Planet on_others_orbit(Planet& central, units::base_space distance, units::base_type mass, float angle)
{
    auto position = central.position() + Vector(cos(angle), sin(angle)) * distance;
    auto orbital_speed = sqrt((G*central.mass) / distance);
    auto normal_v = Vector(position);
    normal_v.normalize();

    auto speed = central.speed() + Vector(-1.0*normal_v.y,normal_v.x) * orbital_speed;

    return Planet(mass, position, speed);
}

Planets sample_planets()
{
    using namespace units;

    auto planets = Planets();
    planets.push_back(Planet(sun_mass,Vector(0,0),Vector(0,0)));
    planets.push_back(on_others_orbit(planets[0], au, M, 0));
    planets.push_back(on_others_orbit(planets[1], 384403 * kilometer, 0.0123 * M, 0));
    return planets;
}

void add_random_planetoids(Planets & planets, units::base_space max_distance, units::base_type max_mass, int count)
{
    std::random_device rd;

    std::default_random_engine e1(rd());
    std::uniform_real_distribution<units::base_space> distance(0.1*units::au, max_distance);
    std::uniform_real_distribution<units::base_type> mass(0.0001 * units::M, max_mass);
    std::uniform_real_distribution<float> angle(0, 2*pi());

    for(auto i = 0; i < count; i++)
    {
        planets.push_back(on_others_orbit(planets[0], distance(e1), mass(e1), angle(e1)));
    }

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
    auto viz = GlViz(2*units::au, 2*units::au);

    const auto time_delta = units::minute * 15;
    const auto print_interval = units::hour;
    const auto simulation_time = units::year;
    const auto verbose = false;

    auto world = sample_planets();
    add_random_planetoids(world, 1.5*units::au, units::M, 200);

    auto simulation = NewtonSimulator(world, time_delta);

    while(simulation.time() < simulation_time)
    {
        simulation.simulate(print_interval);

        viz.print(simulation.result());

        if(verbose)
            print_results(simulation);
    }
}


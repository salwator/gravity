#include <iostream>
#include <functional>
#include <algorithm>

#include "simulate.h"

namespace
{

bool same_planet(const Planet & first, const Planet & second)
{
    return (&first == &second);
}

template <typename T>
T square(T x)
{
    return x*x;
}

auto calc_single_accel(const Planet & calculated_planet, const Planet & second_planet)
{
    return normal(calculated_planet.distance_to(second_planet))
           * (G * second_planet.mass)
           / square(calculated_planet.distance_to(second_planet).length());
}

auto calc_all_accel(const Planets & planets, const Planet & planet)
{
    auto accel_to_other_planets = [&planet](auto acc, const auto & second)
                        {
                            return  (same_planet(planet, second)
                                     ? acc
                                     : acc += calc_single_accel(planet, second));
                        };

    return std::accumulate(planets.begin(),
                           planets.end(),
                           Vector(0,0),
                           accel_to_other_planets);
}

} // namespace

template <typename T>
Planets calculate_planets_in_range(const Planets & planets, const T & planet_range, units::base_time dt)
{
    auto planets_new = Planets();

    std::transform(planet_range.begin(),
                   planet_range.end(),
                   std::back_inserter(planets_new),
                   [&planets,dt](const Planet & planet)
                   {
                       return Planet(planet.mass,
                                     planet.position() + planet.speed() * dt,
                                     planet.speed() + calc_all_accel(planets, planet) * dt);
                   });

    return planets_new;
}

Planets simulate(const Planets & planets, units::base_time dt)
{
    return calculate_planets_in_range(planets, planets, dt);
}

Planets simulate_range(const Planets & planets, const PlanetRefList & range, units::base_time dt)
{
    return calculate_planets_in_range(planets, range, dt);
}


#include <iostream>
#include <cmath>
#include <functional>
#include <algorithm>

#include "simulate.h"

namespace
{

bool same_planet(const Planet & first, const Planet & second)
{
    return (&first == &second);
}

Vector distance_vector(const Planet & first, const Planet & second)
{
    return first.distance_to(second);
}

units::base_space distance(const Planet & first, const Planet & second)
{
    return distance_vector(first,second).length();
}

Vector normal_vector(const Planet & first, const Planet & second)
{
    auto distance = distance_vector(first,second);
    distance.normalize();
    return distance;
}

Vector calc_single_accel(const Planet & calculated_planet, const Planet & second_planet)
{
    return Vector(normal_vector(calculated_planet, second_planet) *
                  ((G * second_planet.mass) /
                    pow(distance(calculated_planet,
                                 second_planet),
                        2)));
}

Vector calc_all_accel(const Planets & planets, const Planet & planet)
{
    auto accel_to_other_planets = [&planet](Vector acc, const Planet & second)
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

Planets simulate_range(const Planets & planets, const std::vector<std::reference_wrapper<const Planet>> & range, units::base_time dt)
{
    return calculate_planets_in_range(planets, range, dt);
}


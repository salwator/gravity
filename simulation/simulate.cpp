#include <iostream>
#include <cmath>
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

double distance(const Planet & first, const Planet & second)
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
    auto planet_accel = [&planet](Vector acc, const Planet & second)
                        {
                            return  (same_planet(planet, second)
                                     ? acc
                                     : acc += calc_single_accel(planet, second));
                        };

    return std::accumulate(planets.begin(),
                           planets.end(),
                           Vector(0,0),
                           planet_accel);
}

}


Planets simulate(const Planets & planets, double dt)
{
    auto planets_new = Planets();

    std::transform(planets.begin(),
                   planets.end(),
                   std::back_inserter(planets_new),
                   [&planets,dt](const Planet & planet)
                   {
                       return Planet(planet.mass,
                                     planet.position() + planet.speed() * dt,
                                     planet.speed() + calc_all_accel(planets, planet) * dt);
                   });

    return planets_new;
}

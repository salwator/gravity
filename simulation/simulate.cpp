#include <iostream>
#include <cmath>
#include <algorithm>
#include <functional>
#include <list>
#include <memory>

#include "tinythread.h"
#include "fast_mutex.h"
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


Planets simulate(const Planets & planets, units::base_time dt)
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

Planets simulate_range(const Planets & planets, const std::vector<std::reference_wrapper<const Planet>> & range, units::base_time dt)
{
    auto range_new = Planets();

    std::transform(range.begin(),
                   range.end(),
                   std::back_inserter(range_new),
                   [&planets,dt](const Planet & planet)
                   {
                       return Planet(planet.mass,
                                     planet.position() + planet.speed() * dt,
                                     planet.speed() + calc_all_accel(planets, planet) * dt);
                   });

    return range_new;
}

struct worker_args
{
    tthread::fast_mutex & lock;
    const Planets & planets;
    const std::vector<std::reference_wrapper<const Planet>> & range;
    const units::base_time dt;
    Planets & result;
};

void worker(void * aArg)
{
    auto args = (worker_args *) aArg;

    auto result = simulate_range(args->planets, args->range, args->dt);

    args->lock.lock();
    args->result = result;
    args->lock.unlock();
}

Planets simulate_threaded(const Planets & planets, const units::base_time dt, int threads_num)
{
    auto partial_planets = std::vector<std::vector<std::reference_wrapper<const Planet>>>();
    for(auto i = 0; i<threads_num; i++)
       partial_planets.push_back({});

    auto count = 0;
    for(const auto & planet : planets)
        partial_planets[count++ % threads_num].push_back(std::cref(planet));

    auto results = std::vector<Planets>(threads_num);
    std::list<std::shared_ptr<tthread::thread>> threadList;
    std::vector<std::shared_ptr<worker_args>> argList;

    tthread::fast_mutex lock;

    for(auto i = 0; i<threads_num; i++)
    {
        argList.push_back(std::make_shared<worker_args>(worker_args({lock, planets, partial_planets[i], dt, results[i]})));
        threadList.push_back(std::make_shared<tthread::thread>(worker, (void *) argList[i].get()));
    }

    for(auto & thread : threadList)
    {
        thread->join();
    }


    auto planets_new = Planets();
    for(auto const result : results)
    {
        planets_new.insert(planets_new.end(),result.begin(),result.end());
    }
    return planets_new;

}

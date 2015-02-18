#include "simulate.h"

#include <future>
#include <iostream>


namespace
{

typedef std::vector<std::reference_wrapper<const Planet>> PlanetRefList;

std::vector<PlanetRefList> distribute_all_planets(const Planets & planets, int threads_num)
{
    auto distributed = std::vector<PlanetRefList>(threads_num);

    auto count = 0;
    for(const auto & planet : planets)
        distributed[count++ % threads_num].push_back(std::cref(planet));

    return distributed;
}

Planets join_result_after_calculated(std::vector<std::future<Planets>> & futures)
{
    auto planets_new = Planets();

    for(auto & future_result : futures)
    {
        auto result = future_result.get();
        planets_new.insert(planets_new.end(),result.begin(),result.end());
    }

    return planets_new;
}

} // namespace

Planets simulate_threaded(const Planets & planets, const units::base_time dt, int threads_num)
{
    auto partial_planets = distribute_all_planets(planets, threads_num);
    auto futures = std::vector<std::future<Planets>>();

    for(auto i = 0; i < threads_num; i++)
        futures.push_back(std::async(std::launch::async,
                                     [&](int i)
                                     {
                                        return simulate_range(planets, partial_planets[i], dt);
                                     }, i));

    return join_result_after_calculated(futures);
}

#include "simulate.h"

#include <future>
#include <iostream>

Planets simulate_threaded(const Planets & planets, const units::base_time dt, int threads_num)
{
    auto futures = std::vector<std::future<Planets>>();
    auto partial_planets = std::vector<std::vector<std::reference_wrapper<const Planet>>>(threads_num);

    auto count = 0;
    for(const auto & planet : planets)
        partial_planets[count++ % threads_num].push_back(std::cref(planet));

    for(auto i = 0; i < threads_num; i++)
        futures.push_back(std::async(std::launch::async, [&](int i){
                                                                        return simulate_range(planets, partial_planets[i], dt);
                                                                   }, i));

    auto planets_new = Planets();

    for(auto & future_result : futures)
    {
        auto result = future_result.get();
        planets_new.insert(planets_new.end(),result.begin(),result.end());
    }

    return planets_new;
}

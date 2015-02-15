#include "tinythread.h"
#include "fast_mutex.h"
#include "simulate.h"

#include <functional>
#include <list>
#include <memory>

namespace
{

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

} // namespace

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
        planets_new.insert(planets_new.end(),result.begin(),result.end());

    return planets_new;

}

#include <iostream>
#include <cmath>
#include <ratio>
#include <random>
#include <chrono>

#include "space.h"
#include "newton_simulator.hpp"
#include "GlViz.h"

namespace
{

constexpr double pi() { return std::atan(1)*4; }

Planet on_others_orbit(Planet& central, units::base_space distance, units::base_type mass, float angle)
{
    auto position = central.position() + Vector(cos(angle), sin(angle)) * distance;
    auto orbital_speed = sqrt((G*central.mass()) / distance);
    auto normal_v = Vector(position);
    normal_v.normalize();

    auto speed = central.speed() + Vector(-1.0*normal_v.y,normal_v.x) * orbital_speed;

    return Planet(mass, position, speed);
}

Planets sample_planets()
{
    using namespace units;

    auto planets = Planets();
    planets.push_back(Planet(sun_mass,Vector(0,0),Vector(0,0))); // Sun
    planets.push_back(on_others_orbit(planets[0], au, M, 0)); // Earth
    planets.push_back(on_others_orbit(planets[1], 384403 * kilometer, 0.0123 * M, 0)); // Moon
    planets.push_back(on_others_orbit(planets[0], 5.3*au, 317 * M, pi())); // Jupiter
    return planets;
}

void add_random_planetoids(Planets & planets, int planet, units::base_space max_distance, units::base_type max_mass, int count)
{
    std::random_device rd;

    std::default_random_engine e1(rd());
    std::uniform_real_distribution<units::base_space> distance(2*units::au, max_distance);
    std::uniform_real_distribution<units::base_type> mass(0.001 * units::M, max_mass);
    std::uniform_real_distribution<float> angle(0, 2*pi());

    for(auto i = 0; i < count; i++)
    {
        planets.push_back(on_others_orbit(planets[planet], distance(e1), mass(e1), angle(e1)));
    }

}

class FpsCounter
{
    public:
        using clock = std::chrono::high_resolution_clock;

        void tick()
        {
            auto now = clock::now();

            if(count++ == 0)
            {
                last_time = now;
                return;
            }

            auto duration = std::chrono::duration<float>(now - last_time);
            if(duration >= std::chrono::seconds(1))
            {
                auto fps = count / std::chrono::duration_cast<std::chrono::seconds>(duration).count();
                count = 0;
                std::cout << "fps: " << fps << std::endl;

                if(fps >= high_fps && high_fps_callback)
                    high_fps_callback();
            }
        }

        void set_high_fps_callback(int thr_level, std::function<void()> callback)
        {
            high_fps = thr_level;
            high_fps_callback = callback;
        }

    private:
        std::chrono::time_point<clock> last_time;
        std::function<void()> high_fps_callback;
        int high_fps = 0;
        int count = 0;
};

} // namespace

int main()
{
    const auto window_width = 1280;
    const auto window_height = 1024;

    const auto time_delta = units::minute * 60;
    const auto simulation_time = units::year;

    auto print_interval = time_delta;

    auto world = sample_planets();
    add_random_planetoids(world, 0, 0.5*units::au, units::M, 1000);

    auto simulation = NewtonSimulator(world, time_delta);

    auto viz = GlViz(1.1*units::au, window_width, window_height);

    auto fpsControl = FpsCounter();
    fpsControl.set_high_fps_callback(30, [&print_interval,time_delta](){ print_interval += time_delta; });

    while(simulation.time() < simulation_time)
    {
        fpsControl.tick();
        simulation.simulate(print_interval);

        viz.print(simulation.result());
    }
}


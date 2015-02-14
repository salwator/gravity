#pragma once

#include "simulate.h"
#include "ISimulator.hpp"

/*
 * Interface for simulator module using newton physics.
 * No logic facade, no tests.
 */

class NewtonSimulator : public ISimulator
{
public:
    NewtonSimulator(Planets, double);
    void simulate(units::base_time) override;
    const Planets & result() override;
    units::base_time time() override;

private:
    Planets planets;
    units::base_time dt;
    units::base_time total_time = 0.0;
};



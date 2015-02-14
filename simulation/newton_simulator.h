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
    void simulate(double) override;
    const Planets & result() override;
    double time() override;

private:
    Planets planets;
    double dt;
    double total_time = 0.0;
};



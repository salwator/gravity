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
    void simulate(int) override;
    const Planets & result() override;

private:
    Planets planets;
    double dt;
};



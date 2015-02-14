#pragma once

#include <memory>
#include "space.h"

class ISimulator
{
    public:
        virtual void simulate(double) = 0;
        virtual const Planets & result() = 0;
        virtual double time() = 0;
};

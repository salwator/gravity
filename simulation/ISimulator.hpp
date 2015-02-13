#pragma once

#include <memory>
#include "space.h"

class ISimulator
{
    public:
        virtual void simulate(int) = 0;
        virtual const Planets & result() = 0;
};

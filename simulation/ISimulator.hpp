#pragma once

#include <memory>
#include "space.h"

class ISimulator
{
    public:
        virtual void simulate(units::base_time) = 0;
        virtual const Planets & result() = 0;
        virtual units::base_time time() = 0;
};

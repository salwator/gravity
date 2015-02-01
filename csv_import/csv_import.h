#pragma once

#include <vector>
#include <string>

#include "space.h"

enum csv_position
{
    mass = 0,
    x = 1,
    y = 2,
    vx = 3,
    vy = 4
};

std::vector<double> split_string(const std::string&, const char);
Planet create_planet_from_csv_string(const std::string);

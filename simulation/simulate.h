#pragma once

#include "space.h"

const auto DELTA_TIME = 1.0L;
const auto G = 6.67384E-11;

Planets simulate(Planets&, double dt = DELTA_TIME);

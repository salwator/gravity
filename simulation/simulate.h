#pragma once

#include "space.h"

const auto DELTA_TIME = 1.0L;
const auto G = 6.67384E-11;

typedef std::vector<std::reference_wrapper<const Planet>> PlanetRefList;

Planets simulate(const Planets&, units::base_time = DELTA_TIME);
Planets simulate_threaded(const Planets &, const units::base_time, int);
Planets simulate_range(const Planets &, const PlanetRefList &, units::base_time);

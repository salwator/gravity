#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include "space.h"

template<typename T, typename... Args>
class IFunctor
{
    public:
        virtual T operator() (Args... args) = 0;
};

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

typedef IFunctor<Planet, std::string> PlanetCreationFunctor;

class CsvImporter
{
public:
    CsvImporter(PlanetCreationFunctor&);
    Planets import(std::istream& input);

private:
    PlanetCreationFunctor & factory_func;
};


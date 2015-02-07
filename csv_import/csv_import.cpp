#include "csv_import.h"

std::vector<double> split_string(const std::string & str, const char delimiter)
{
    auto str_splitted = std::vector<double>();
    auto add_number = [&](std::string str){str_splitted.push_back(std::stod(str));};

    std::string str_number;

    for(auto c : str)
    {
        if(c != delimiter)
        {
            str_number += c;
        }
        else
        {
            add_number(str_number);
            str_number.clear();
        }

    }

    if(str_number.size())
        add_number(str_number);

    return str_splitted;
}

Planet create_planet_from_csv_string(const std::string csv_string)
{
    auto str_splitted = split_string(csv_string, ',');

    while(str_splitted.size() < 5)
        str_splitted.push_back(0.0);

    return Planet(str_splitted[csv_position::mass],
                  str_splitted[csv_position::x],
                  str_splitted[csv_position::y],
                  str_splitted[csv_position::vx],
                  str_splitted[csv_position::vy]);
}


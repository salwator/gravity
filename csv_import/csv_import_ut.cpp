#include <gtest/gtest.h>
#include "space.h"

#include <string>

std::vector<std::string> split_string(const std::string & str, const char delimiter)
{
    auto string_splitted = std::vector<std::string>();

    std::string number;

    for(auto c : str)
    {
        if(c != delimiter)
            number += c;
        else
        {
            string_splitted.push_back(number);
            number.clear();
        }

    }

    if(number.size())
        string_splitted.push_back(number);

    return string_splitted;
}

Planet create_planet_from_csv_string(std::string csv_string)
{
    auto string_splitted = split_string(csv_string, ','); 

    if(string_splitted.size() > 0)
    {
        while(string_splitted.size() < 5)
            string_splitted.push_back("0.0");

        return Planet(std::stod(string_splitted[0]),
                      std::stod(string_splitted[1]),
                      std::stod(string_splitted[2]),
                      std::stod(string_splitted[3]),
                      std::stod(string_splitted[4]));
    }

    return Planet(0,0);
}

class CsvImportTest : public ::testing::Test
{
};

TEST_F(CsvImportTest, FirstNumberIsMass)
{
    ASSERT_DOUBLE_EQ(create_planet_from_csv_string("10.1").mass, 10.1);
}

TEST_F(CsvImportTest, SecondNumberIsX)
{
    ASSERT_DOUBLE_EQ(create_planet_from_csv_string("2.0,100.2").x, 100.2);
}

TEST_F(CsvImportTest, ThirdNumberIsY)
{
    ASSERT_DOUBLE_EQ(create_planet_from_csv_string("2.0,100.2,200.5").y, 200.5);
}

TEST_F(CsvImportTest, FourthNumberIsVx)
{
    ASSERT_DOUBLE_EQ(create_planet_from_csv_string("3.0,50.3,100.1,22.4").vx, 22.4);
}

TEST_F(CsvImportTest, FifthNumberIsVy)
{
    ASSERT_DOUBLE_EQ(create_planet_from_csv_string("3.0,50.3,100.1,22.4,99.5").vy, 99.5);
}

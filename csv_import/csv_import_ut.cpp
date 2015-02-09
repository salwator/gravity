#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <functional>
#include "csv_import.h"


using namespace testing;

class CsvImportTest : public Test
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



class PlanetCreationFunctionMock : public PlanetCreationFunctor
{
    public:
       Planet operator() (std::string str)
       {
           return call(str);
       }

       MOCK_METHOD1(call, Planet(std::string));
};

class CsvImporterTest : public Test
{
    public:
        PlanetCreationFunctionMock functionMock;
        CsvImporter importer = CsvImporter(functionMock);
        std::stringstream input_stream;

};

bool planet_match(Planet & p1, Planet & p2)
{
    return (p1.mass == p2.mass &&
            p1.position().x    == p2.position().x &&
            p1.position().y    == p2.position().y);
}

TEST_F(CsvImporterTest, EmptyStreamGivesEmptyPlanetList)
{
    input_stream.str("");
    EXPECT_EQ(importer.import(input_stream).size(), 0);
}

TEST_F(CsvImporterTest, OneLineIsOnePlanet)
{
    input_stream.str("foo");
    auto sample_planet = Planet(100, Vector(10,20));

    EXPECT_CALL(functionMock, call("foo"))
        .WillOnce(Return(sample_planet));
    auto planets = importer.import(input_stream);

    ASSERT_EQ(planets.size(), 1);
    EXPECT_TRUE(planet_match(planets[0], sample_planet));
}

TEST_F(CsvImporterTest, MultipleLinesArePlanets)
{
    input_stream.str("foo\nbar");
    auto first_planet = Planet(100, Vector(10,20));
    auto second_planet = Planet(230, Vector(120,210));

    EXPECT_CALL(functionMock, call("foo"))
        .WillOnce(Return(first_planet));
    EXPECT_CALL(functionMock, call("bar"))
        .WillOnce(Return(second_planet));

    auto planets = importer.import(input_stream);

    ASSERT_EQ(planets.size(), 2);
    EXPECT_TRUE(planet_match(planets[0], first_planet));
    EXPECT_TRUE(planet_match(planets[1], second_planet));
}

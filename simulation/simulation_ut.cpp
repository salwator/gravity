#include <gtest/gtest.h>

#include "space.h"
#include "simulate.h"

class SimulationTest : public ::testing::Test
{
    public:
        Planets planets;
};

TEST_F(SimulationTest, GivenEmptyData_EmptyOutput)
{
    ASSERT_EQ(simulate(planets).size(), 0);
}

TEST_F(SimulationTest, GivenOnePlanet_Unchanged)
{
    planets.push_back(Planet(100, Vector(200,0)));
    auto planets_new = simulate(planets);

    ASSERT_EQ(planets_new.size(), 1);
    EXPECT_EQ(planets_new[0].x, 200);
    EXPECT_EQ(planets_new[0].mass, 100);
}

TEST_F(SimulationTest, GivenTwoPlanetsWithZeroMass_Unchanged)
{
    planets.push_back(Planet(0, Vector(200,0)));
    planets.push_back(Planet(0, Vector(220,0)));

    auto planets_new = simulate(planets);

    ASSERT_EQ(planets_new.size(), 2);
    EXPECT_EQ(planets_new[0].mass, 0);
    EXPECT_EQ(planets_new[0].x, 200);
    EXPECT_EQ(planets_new[1].mass, 0);
    EXPECT_EQ(planets_new[1].x, 220);
}

TEST_F(SimulationTest, GivenTwoPlanets_MassOfSecondIsOne_RadiusIsOne_AfterOneSecondPlanetVelocityIsGConst)
{
    planets.push_back(Planet(10, Vector(0,0)));
    planets.push_back(Planet(1, Vector(1,0)));

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].vx, G);
}

TEST_F(SimulationTest, GivenTwoPlanets_TwiceTheMass_TwiceTheAcceleration)
{
    planets.push_back(Planet(10, Vector(0,0)));
    planets.push_back(Planet(2, Vector(1,0)));

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].vx, 2*G);
}

TEST_F(SimulationTest, GivenTwoPlanets_TwiceTheDistance_FourTimeLessAcceleration)
{
    planets.push_back(Planet(10, Vector(0,0)));
    planets.push_back(Planet(2, Vector(2,0)));

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].vx, 0.5*G);
}

TEST_F(SimulationTest, GivenTwoPlanets_TripleTheDistance_NineTimeLessAcceleration)
{
    planets.push_back(Planet(10, Vector(0,0)));
    planets.push_back(Planet(2, Vector(3,0)));

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].vx, G*2/9);

}

TEST_F(SimulationTest, VelocityChangesPositionDuringTime)
{
    planets.push_back(Planet(0,Vector(0,0)));
    planets[0].vx = 10;

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].x, 10);
}

TEST_F(SimulationTest, DoubleTimeStep_DoublePositionChangeInConstantMove)
{
    planets.push_back(Planet(0,Vector(0,0)));
    planets[0].vx = 10;

    auto planets_new = simulate(planets, 2.0);

    EXPECT_DOUBLE_EQ(planets_new[0].x, 20);
}

TEST_F(SimulationTest, CounterForcesTakeNoEffect)
{
    planets.push_back(Planet(10, Vector(0,0)));
    planets.push_back(Planet(10, Vector(10,0)));
    planets.push_back(Planet(10, Vector(20,0)));

    auto planets_new = simulate(planets, 2.0);

    EXPECT_DOUBLE_EQ(planets_new[1].vx, 0.0);
}

TEST_F(SimulationTest, SecondDimensionAdded_EqualToOneDimensionWhenZero)
{
    planets.push_back(Planet(10, Vector(0, 0)));
    planets.push_back(Planet(1, Vector(1, 0)));

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].vx, G);
    EXPECT_DOUBLE_EQ(planets_new[0].vy, 0);
}

TEST_F(SimulationTest, AttractionIsEqualInDimensions)
{
    planets.push_back(Planet(10, Vector(0, 0)));
    planets.push_back(Planet(1, Vector(0, 1)));

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].vx, 0);
    EXPECT_DOUBLE_EQ(planets_new[0].vy, G);
}

TEST_F(SimulationTest, VelocityChangesPositionDuringTime_InSecondDimension)
{
    planets.push_back(Planet(10, Vector(0, 0)));
    planets[0].vy = 10;

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].y, 10);
    EXPECT_DOUBLE_EQ(planets_new[0].x, 0);
}

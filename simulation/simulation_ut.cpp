#include <gtest/gtest.h>

#include "space.h"
#include "simulate.hpp"

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
    planets.push_back( Planet(100, {200,0}) );
    auto planets_new = simulate(planets);

    ASSERT_EQ(planets_new.size(), 1);
    EXPECT_EQ(planets_new[0].position().x, 200);
    EXPECT_EQ(planets_new[0].mass(), 100);
}

TEST_F(SimulationTest, GivenTwoPlanetsWithZeroMass_Unchanged)
{
    planets.push_back( Planet(0, {200,0}) );
    planets.push_back( Planet(0, {220,0}) );

    auto planets_new = simulate(planets);

    ASSERT_EQ(planets_new.size(), 2);
    EXPECT_EQ(planets_new[0].mass(), 0);
    EXPECT_EQ(planets_new[0].position().x, 200);
    EXPECT_EQ(planets_new[1].mass(), 0);
    EXPECT_EQ(planets_new[1].position().x, 220);
}

TEST_F(SimulationTest, GivenTwoPlanets_MassOfSecondIsOne_RadiusIsOne_AfterOneSecondPlanetVelocityIsGConst)
{
    planets.push_back( Planet(10, {0,0}) );
    planets.push_back( Planet(1, {1,0}) );

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].speed().x, G);
}

TEST_F(SimulationTest, GivenTwoPlanets_TwiceTheMass_TwiceTheAcceleration)
{
    planets.push_back( Planet(10, {0,0}) );
    planets.push_back( Planet(2, {1,0}) );

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].speed().x, 2*G);
}

TEST_F(SimulationTest, GivenTwoPlanets_TwiceTheDistance_FourTimeLessAcceleration)
{
    planets.push_back( Planet(10, {0,0}) );
    planets.push_back( Planet(2, {2,0}) );

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].speed().x, 0.5*G);
}

TEST_F(SimulationTest, GivenTwoPlanets_TripleTheDistance_NineTimeLessAcceleration)
{
    planets.push_back( Planet(10, {0,0}) );
    planets.push_back( Planet(2, {3,0}) );

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].speed().x, G*2/9);

}

TEST_F(SimulationTest, VelocityChangesPositionDuringTime)
{
    planets.push_back( Planet(0, {0,0}, {10,0}) );

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].speed().x, 10);
}

TEST_F(SimulationTest, DoubleTimeStep_DoublePositionChangeInConstantMove)
{
    planets.push_back( Planet(0, {0,0}, {10,0}) );

    auto planets_new = simulate(planets, 2.0);

    EXPECT_DOUBLE_EQ(planets_new[0].position().x, 20);
}

TEST_F(SimulationTest, CounterForcesTakeNoEffect)
{
    planets.push_back( Planet(10, {0,0}) );
    planets.push_back( Planet(10, {10,0}) );
    planets.push_back( Planet(10, {20,0}) );

    auto planets_new = simulate(planets, 2.0);

    EXPECT_DOUBLE_EQ(planets_new[1].speed().x, 0.0);
}

TEST_F(SimulationTest, SecondDimensionAdded_EqualToOneDimensionWhenZero)
{
    planets.push_back( Planet(10, {0,0}) );
    planets.push_back( Planet(1, {1,0}) );

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].speed().x, G);
    EXPECT_DOUBLE_EQ(planets_new[0].speed().y, 0);
}

TEST_F(SimulationTest, AttractionIsEqualInDimensions)
{
    planets.push_back( Planet(10, {0,0}) );
    planets.push_back( Planet(1, {0,1}) );

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].speed().x, 0);
    EXPECT_DOUBLE_EQ(planets_new[0].speed().y, G);
}

TEST_F(SimulationTest, VelocityChangesPositionDuringTime_InSecondDimension)
{
    planets.push_back( Planet(10, {0,0}, {0,10}) );

    auto planets_new = simulate(planets);

    EXPECT_DOUBLE_EQ(planets_new[0].position().y, 10);
    EXPECT_DOUBLE_EQ(planets_new[0].position().x, 0);
}

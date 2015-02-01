#include <gtest/gtest.h>
#include "csv_import.h"

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

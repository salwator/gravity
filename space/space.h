#pragma once

#include <vector>

namespace units
{

typedef long double base_type;

const auto second = base_type(1);
const auto minute = 60 * second;
const auto hour = 60 * minute;
const auto day = 24 * hour;
const auto year = 365.25 * day;

const auto meter = base_type(1);
const auto kilometer = 1000 * meter;
const auto au = 149597871 * kilometer;

const auto kilogram = base_type(1);
const auto tone = 1000 * kilogram;
const auto M = 5.9736E+24 * kilogram;
const auto sun_mass = 300000 * M;

}

struct Vector
{
    Vector(double, double);
    Vector(const Vector &);


    Vector operator+ (const Vector& right) const;
    Vector& operator+= (const Vector& right);
    Vector& operator= (const Vector& ) = default;

    template<typename T>
    Vector operator* (const T& right) const
    {
        return Vector(x * right, y * right);
    }

    template<typename T>
    Vector operator/ (const T& right) const
    {
        return Vector(x / right, y / right);
    }

    double length() const;
    void normalize();

    double x,y;
};

struct Planet{

    Planet(double, Vector, Vector = Vector(0,0));
    Planet(const Planet &);


    Vector position() const;
    Vector speed() const;
    Vector distance_to(const Planet& other) const;

    double mass;
    double x,y;
    double vx, vy;

};

typedef std::vector<Planet> Planets;



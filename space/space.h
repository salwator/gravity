#pragma once

#include <vector>

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



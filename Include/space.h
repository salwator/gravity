#pragma once

#include <vector>
#include <cmath>

struct Vector
{
    Vector(double x, double y)
        : x(x),y(y)
    {}

    Vector(const Vector & other)
        : x(other.x),y(other.y)
    {}


    Vector(Vector && old)
        : x(old.x),y(old.y)
    {}

    Vector operator+ (const Vector& right) const
    {
        return Vector(x + right.x, y + right.y);
    }

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

    Vector& operator+= (const Vector& right)
    {
        x += right.x;
        y += right.y;
        return *this;
    }

    double length() const
    {
        return sqrt(x*x + y*y);
    }

    void normalize()
    {
        auto l = length();
        x = x / l;
        y = y / l;
    }

    double x,y;
};

struct Planet{

    Planet(double mass, double x, double y = 0, double vx = 0, double vy = 0):
        mass(mass),
        x(x), y(y),
        vx(vx), vy(vy)
    {};

    Planet(const Planet & other):
        mass(other.mass),
        x(other.x), y(other.y),
        vx(other.vx), vy(other.vy)
    {};

    void move(double dt)
    {
        x += vx * dt;
        y += vy * dt;
    }

    void change_speed(Vector dv)
    {
        vx += dv.x;
        vy += dv.y;
    }

    Vector distance_to(const Planet& other) const
    {
        return Vector(other.x - x, other.y - y);
    }

    double mass;
    double x,y;
    double vx, vy;

};

typedef std::vector<Planet> Planets;



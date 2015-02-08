#include <cmath>
#include "space.h"

Vector::Vector(double x, double y)
    : x(x),y(y)
{}

Vector::Vector(const Vector & other)
    : x(other.x),y(other.y)
{}

double Vector::length() const
{
    return sqrt(x*x + y*y);
}

Vector Vector::operator+ (const Vector& right) const
{
    return Vector(x + right.x, y + right.y);
}


Vector& Vector::operator+= (const Vector& right)
{
    x += right.x;
    y += right.y;
    return *this;
}

void Vector::normalize()
{
    auto l = length();
    x = x / l;
    y = y / l;
}


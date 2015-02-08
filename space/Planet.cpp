#include "space.h"

Planet::Planet(double mass, Vector position, Vector speed)
    :
    mass(mass),
    x(position.x),y(position.y),
    vx(speed.x),vy(speed.y)
{}

Planet::Planet(const Planet & other)
    :
    mass(other.mass),
    x(other.x), y(other.y),
    vx(other.vx), vy(other.vy)
{}


Vector Planet::position() const
{
    return Vector(x,y);
}

Vector Planet::speed() const
{
    return Vector(vx,vy);
}

Vector Planet::distance_to(const Planet& other) const
{
    return Vector(other.x - x, other.y - y);
}

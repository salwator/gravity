#include "space.h"

Planet::Planet(units::base_type mass, Vector position, Vector speed)
    :
    m(mass),
    x(position.x),y(position.y),
    vx(speed.x),vy(speed.y)
{}

Planet::Planet(const Planet & other)
    :
    m(other.mass()),
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

units::base_type Planet::mass() const
{
    return m;
}

std::unique_ptr<ISimulatedBody> Planet::cloneWithMotion(Vector position, Vector speed) const
{
    return std::make_unique<Planet>(m, position, speed);
}

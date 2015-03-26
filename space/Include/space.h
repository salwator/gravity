#pragma once

#include <vector>
#include <memory>

namespace units
{

typedef double base_type;
typedef base_type base_space;
typedef base_type base_time;

const auto second = base_type(1);
const auto minute = 60 * second;
const auto hour = 60 * minute;
const auto day = 24 * hour;
const auto year = 365.25 * day;

const auto meter = base_space(1);
const auto kilometer = 1'000 * meter;
const auto au = 149'597'871 * kilometer;

const auto kilogram = base_type(1);
const auto tone = 1'000 * kilogram;
const auto M = 5.9736E+24 * kilogram;
const auto sun_mass = 332'946 * M;

}

struct Vector
{
    Vector(units::base_space, units::base_space);
    Vector(const Vector &);


    Vector operator+ (const Vector& right) const;
    Vector operator- (const Vector& right) const;
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

    units::base_space length() const;
    void normalize();

    units::base_space x,y;
};

inline Vector normal(Vector && old)
{
    old.normalize();
    return std::move(old);
}

struct ISimulatedBody
{
    virtual std::unique_ptr<ISimulatedBody> cloneWithMotion(Vector pos, Vector v) const = 0;
    virtual ~ISimulatedBody() {}
};

struct ICelestial
{
    virtual Vector position() const = 0;
    virtual Vector speed() const = 0;
    virtual units::base_type mass() const = 0;
    virtual Vector distance_to(const ICelestial& other) const = 0;

    virtual ~ICelestial() {}
};


class Planet : public ISimulatedBody, public ICelestial
{

public:
    Planet(units::base_space, Vector, Vector = Vector(0,0));
    Planet(const Planet &);

    virtual std::unique_ptr<ISimulatedBody> cloneWithMotion(Vector, Vector) const;

    virtual Vector position() const;
    virtual Vector speed() const;
    virtual units::base_type mass() const;
    virtual Vector distance_to(const ICelestial& other) const;

private:
    units::base_type m;
    units::base_space x,y;
    units::base_space vx, vy;

};

typedef std::vector<Planet> Planets;
typedef std::vector<std::unique_ptr<ISimulatedBody>> Simulated;



#include "Entity.h"

#include "Utils.h"
#include "Random.h"

Entity::Entity(double x, double y, double radius)
    : Entity(x, y, radius, Random::Real(0.0, EoBE::Tau), 0.0)
{
}

Entity::Entity(double x, double y, double radius, double bearing, double speed)
    : radius_(radius)
    , bearing_(bearing)
    , speed_(speed)
    , x_(x)
    , y_(y)
{
}

Entity::~Entity()
{
}

const double& Entity::GetX() const
{
    return x_;
}

const double& Entity::GetY() const
{
    return y_;
}

const double& Entity::GetRadius() const
{
    return radius_;
}

Point Entity::GetLocation() const
{
    return { x_, y_ };
}

bool Entity::Alive() const
{
    assert(energy_ >= 0.0);
    return energy_ > 0.0;
}

bool Entity::Move()
{
    if (speed_ > 0) {
        x_ += std::sin(bearing_) * speed_;
        y_ += -(std::cos(bearing_) * speed_);
        return true;
    }
    return false;
}

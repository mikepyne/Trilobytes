#include "Entity.h"

Entity::Entity(double x, double y)
    : Entity(x, y, Random::Real(0.0, 6.28), 0.0)
{
}

Entity::Entity(double x, double y, double bearing, double speed)
    : x_(x)
    , y_(y)
    , bearing_(bearing)
    , speed_(speed)
{
}

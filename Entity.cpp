#include "Entity.h"

#include "Utils.h"
#include "Random.h"

Entity::Entity(EnergyPool&& energy, double x, double y, double radius)
    : Entity(std::move(energy), x, y, radius, Random::Number(0.0, EoBE::Tau), 0.0)
{
}

Entity::Entity(EnergyPool&& energy, double x, double y, double radius, double bearing, double speed)
    : radius_(radius)
    , bearing_(bearing)
    , speed_(speed)
    , energy_(std::move(energy))
    , x_(x)
    , y_(y)
{
}

Entity::~Entity()
{
}

bool Entity::Alive() const
{
    return energy_.Quantity() > 0.0;
}

void Entity::FeedOn(Entity& other, double quantity)
{
    other.energy_.GiveTo(energy_, quantity);
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

#include "Entity.h"

#include "Utils.h"
#include "Random.h"

#include <QPainter>

Entity::Entity(Energy energy, const Transform& transform, double radius, QColor colour)
    : Entity(energy, transform, radius, 0.0, colour)
{
}

Entity::Entity(Energy energy, const Transform& transform, double radius, double speed, QColor colour)
    : energy_(energy)
    , transform_(transform)
    , radius_(radius)
    , speed_(speed)
    , age_(0)
    , colour_(colour)
{
}

Entity::~Entity()
{
}

bool Entity::Alive() const
{
    return energy_ > 0;
}

void Entity::FeedOn(Entity& other, Energy quantity)
{
    energy_ += other.TakeEnergy(quantity);
}

bool Entity::Tick(EntityContainerInterface& container, const UniverseParameters& universeParameters)
{
    TickImpl(container, universeParameters);
    age_++;
    return Move();
}

void Entity::Draw(QPainter& paint)
{
    paint.save();
    paint.setBrush(colour_);
    DrawImpl(paint);
    paint.restore();
}

Energy Entity::TakeEnergy(Energy quantity)
{
    Energy toGive = std::min(energy_, quantity);
    energy_ -= quantity;
    return toGive;
}

void Entity::SetBearing(double bearing)
{
    if (bearing < 0.0) {
        bearing += EoBE::Tau;
    } else if (bearing > EoBE::Tau) {
        bearing -= EoBE::Tau;
    }
    transform_.rotation = bearing;
}

bool Entity::Move()
{
    if (std::abs(speed_) > 0.05) {
        Point newLocation = ApplyOffset({ transform_.x, transform_.y }, transform_.rotation, speed_);
        transform_.x = newLocation.x;
        transform_.y = newLocation.y;
        speed_ -= speed_ / 4.0;
        return true;
    }
    return false;
}



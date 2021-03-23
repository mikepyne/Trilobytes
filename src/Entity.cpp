#include "Entity.h"

#include "Utility/Random.h"

#include <QPainter>

#include <assert.h>

Entity::Entity(const Transform& transform, double radius, QColor colour, Energy energy, double speed)
    : energy_(energy)
    , transform_(transform)
    , radius_(radius)
    , speed_(speed)
    , age_(0)
    , colour_(colour)
{
    assert(radius_ <= MAX_RADIUS);
}

Entity::~Entity()
{
}

void Entity::FeedOn(Entity& other, Energy quantity)
{
    energy_ += other.TakeEnergy(quantity);
    if (other.GetEnergy() <= 0.0) {
        other.Terminate();
    }
}

bool Entity::Tick(EntityContainerInterface& container, const UniverseParameters& universeParameters)
{
    TickImpl(container, universeParameters);
    age_++;
    return Move(/* TODO user adjustable coefficient of friction */);
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
        bearing += Tril::Tau;
    } else if (bearing > Tril::Tau) {
        bearing -= Tril::Tau;
    }
    transform_.rotation = bearing;
}

bool Entity::Move()
{
    if (std::abs(speed_) > 0.05) {
        Point newLocation = ApplyOffset({ transform_.x, transform_.y }, transform_.rotation, speed_);
        transform_.x = newLocation.x;
        transform_.y = newLocation.y;
        speed_ *= 0.9;
        return true;
    }
    return false;
}



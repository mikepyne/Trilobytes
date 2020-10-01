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

bool Entity::Tick(EntityContainerInterface& container)
{

    TickImpl(container);

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

double Entity::GetTrait(Trait trait)
{
    switch (trait) {
    case Trait::Red :
        return colour_.redF();
    case Trait::Green :
        return colour_.greenF();
    case Trait::Blue :
        return colour_.blueF();
    case Trait::Energy :
        return energy_;
    case Trait::Age :
        return age_;
    case Trait::Size :
        return radius_;
    }
    assert(false && "Entity::GetTrait, unimplemented trait.");
    return 0.0;
}

Energy Entity::TakeEnergy(Energy quantity)
{
    Energy toGive = std::min(energy_, quantity);
    energy_ -= quantity;
    return toGive;
}

void Entity::SetColour(double red, double green, double blue)
{
    colour_.setRgbF(red, green, blue);
    // TODO use energy to do this!
}

void Entity::SetBearing(double bearing)
{
    transform_.rotation = bearing;
    // TODO use energy to do this
}

void Entity::SetSpeed(double speed)
{
    speed_ = speed;
    // TODO use energy to do this
}

bool Entity::Move()
{
    // TODO THIS wont be using energy, BUT the changing of the speed should totally be using energy (proportional to the rate of change)
    // TODO add inertia by reducing speed after movement
    // TODO perhaps overlapping entities could exchange some inertia? penalty for collisions?

    if (speed_ > 0) {
        transform_.x += std::sin(transform_.rotation) * speed_;
        transform_.y += -(std::cos(transform_.rotation) * speed_);
        return true;
    }
    return false;
}



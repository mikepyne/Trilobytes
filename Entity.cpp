#include "Entity.h"

#include "Utils.h"
#include "Random.h"

#include <QPainter>

Entity::Entity(EnergyPool&& energy, double x, double y, double radius, QColor colour)
    : Entity(std::move(energy), x, y, radius, Random::Bearing(), 0.0, colour)
{
}

Entity::Entity(EnergyPool&& energy, double x, double y, double radius, double bearing, double speed, QColor colour)
    : energy_(std::move(energy))
    , x_(x)
    , y_(y)
    , radius_(radius)
    , bearing_(bearing)
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
    return energy_.Quantity() > 0;
}

void Entity::FeedOn(Entity& other, uint64_t quantity)
{
    other.energy_.GiveTo(energy_, quantity);
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
        return energy_.Quantity();
    case Trait::Age :
        return age_;
    case Trait::Size :
        return radius_;
    }
    assert(false && "Entity::GetTrait, unimplemented trait.");
    return 0.0;
}

void Entity::SetColour(double red, double green, double blue)
{
    colour_.setRgbF(red, green, blue);
    // TODO use energy to do this!
}

void Entity::SetBearing(double bearing)
{
    bearing_ = bearing;
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
        x_ += std::sin(bearing_) * speed_;
        y_ += -(std::cos(bearing_) * speed_);
        return true;
    }
    return false;
}

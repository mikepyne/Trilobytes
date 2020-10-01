#ifndef ENTITY_H
#define ENTITY_H

#include "Shape.h"
#include "Energy.h"
#include "Transform.h"
#include "EntityContainerInterface.h"

#include <QColor>

#include <string_view>
#include <array>

class QPainter;

enum class Trait {
    Red,
    Green,
    Blue,
    Energy,
    Age,
    Size,
};

constexpr std::array traits = { Trait::Red, Trait::Green, Trait::Blue, Trait::Energy, Trait::Age, Trait::Size };

class Entity {
public:
    /**
     * Places stationary entity at coordinates with random bearing.
     */
    Entity(Energy energy, const Transform& transform, double radius, QColor colour);
    Entity(Energy energy, const Transform& transform, double radius, double speed, QColor colour);
    virtual ~Entity();

    virtual std::string_view GetName() const = 0;

    const Transform& GetTransform() const { return transform_; }
    [[deprecated]]Point GetLocation() const { return { transform_.x, transform_.y }; } // TODO get rid of this
    const double& GetRadius() const { return radius_; }
    double GetEnergy() const { return energy_; }
    const QColor& GetColour() const { return colour_; }

    bool Alive() const;
    void FeedOn(Entity& other, Energy quantity);

    // returns true if the entity has moved
    bool Tick(EntityContainerInterface& container);
    void Draw(QPainter& paint);

    /**
     * This allows senses to respond to any trait easily, it is up to the caller
     * to normalise the value.
     */
    double GetTrait(Trait trait);

protected:
    virtual void TickImpl(EntityContainerInterface& container) = 0;
    virtual void DrawImpl(QPainter& paint) = 0;

    void UseEnergy(Energy quantity) { energy_ -= quantity; }
    Energy TakeEnergy(Energy quantity);
    void Terminate() { energy_ = 0; }

    // TODO consider these "Set" functions might be better as "Adjust" functions?
    void SetColour(double red, double green, double blue);
    void SetBearing(double bearing);
    void SetSpeed(double speed);

private:
    // Instance variables
    Energy energy_; // TODO consider tracking energy used recenty via some sort of low pass filtered heat variable that decays over time
    Transform transform_;
    double radius_;
    double speed_;
    uint64_t age_;
    QColor colour_;

    // returns true if the entity has moved
    bool Move();
};

#endif // ENTITY_H

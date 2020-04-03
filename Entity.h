#ifndef ENTITY_H
#define ENTITY_H

#include "Shape.h"
#include "EnergyPool.h"
#include "EntityContainerInterface.h"

#include <QColor>

#include <string_view>

class QPainter;

enum class Trait {
    Red,
    Green,
    Blue,
    Energy,
    Age,
};

class Entity {
public:
    /**
     * Places stationary entity at coordinates with random bearing.
     */
    Entity(EnergyPool&& energy, double x, double y, double radius, QColor colour);
    Entity(EnergyPool&& energy, double x, double y, double radius, double bearing, double speed, QColor colour);
    virtual ~Entity();

    virtual std::string_view GetName() const = 0;

    const double& GetX() const { return x_; }
    const double& GetY() const { return y_; }
    const double& GetRadius() const { return radius_; }
    const double& GetBearing() const { return bearing_; }
    Point GetLocation() const { return { x_, y_ }; }
    uint64_t GetEnergy() const { return energy_.Quantity(); }

    bool Alive() const;
    void FeedOn(Entity& other, uint64_t quantity);

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

    void UseEnergy(uint64_t quantity) { energy_.Decay(quantity); }
    EnergyPool TakeEnergy(uint64_t quantity) { return energy_.CreateChild(quantity); }

    // TODO consider these "Set" functions might be better as "Adjust" functions?
    void SetColour(double red, double green, double blue);
    void SetBearing(double bearing);
    void SetSpeed(double speed);

private:
    // Instance variables
    EnergyPool energy_; // TODO consider tracking energy used recenty via some sort of low pass filtered heat variable that decays over time
    double x_;
    double y_;
    double radius_;
    double bearing_;
    double speed_;
    uint64_t age_;

    // Inheritable traits
    QColor colour_;

    // returns true if the entity has moved
    bool Move();
};

#endif // ENTITY_H

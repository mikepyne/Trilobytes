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

class Entity {
public:
    /**
     * Places stationary entity at coordinates with random bearing.
     */
    Entity(Energy energy, const Transform& transform, double radius, QColor colour);
    Entity(Energy energy, const Transform& transform, double radius, double speed, QColor colour);
    virtual ~Entity();

    virtual std::string_view GetName() const = 0;

    const uint64_t& GetAge() const { return age_; }
    const Transform& GetTransform() const { return transform_; }
    [[deprecated]]Point GetLocation() const { return { transform_.x, transform_.y }; }
    const double& GetRadius() const { return radius_; }
    double GetEnergy() const { return energy_; }
    const QColor& GetColour() const { return colour_; }
    const double& GetVelocity() const { return speed_; }

    bool Alive() const;
    void FeedOn(Entity& other, Energy quantity);

    // returns true if the entity has moved
    bool Tick(EntityContainerInterface& container);
    void Draw(QPainter& paint);

protected:
    virtual void TickImpl(EntityContainerInterface& container) = 0;
    virtual void DrawImpl(QPainter& paint) = 0;

    void UseEnergy(Energy quantity) { energy_ -= quantity; }
    Energy TakeEnergy(Energy quantity);
    void Terminate() { energy_ = 0; }

    void SetColour(double red, double green, double blue) { colour_.setRgbF(red, green, blue); }
    void SetBearing(double bearing);
    void SetVelocity(double speed) { speed_ = speed; }
    void SetRadius(double radius) { radius_ = radius; }

private:
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

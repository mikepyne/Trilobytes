#ifndef ENTITY_H
#define ENTITY_H

#include "Shape.h"
#include "Energy.h"
#include "Transform.h"
#include "EntityContainerInterface.h"
#include "UniverseParameters.h"

#include <QColor>

#include <string_view>
#include <array>

class QPainter;

class Entity {
public:
    static constexpr double MAX_RADIUS = 12.0;

    Entity(const Transform& transform, double radius, QColor colour, Energy energy = 0_j, double speed = 0.0);
    virtual ~Entity();

    virtual std::string_view GetName() const = 0;

    const uint64_t& GetAge() const { return age_; }
    const Transform& GetTransform() const { return transform_; }
    [[deprecated]]Point GetLocation() const { return { transform_.x, transform_.y }; }
    const double& GetRadius() const { return radius_; }
    double GetEnergy() const { return energy_; }
    const QColor& GetColour() const { return colour_; }
    const double& GetVelocity() const { return speed_; }
    bool Exists() const { return !terminated_; }
    Circle GetCollide() const { return Circle{ transform_.x, transform_.y, radius_ }; };

    void SetLocation(const Point& location) { transform_.x = location.x; transform_.y = location.y; }
    void FeedOn(Entity& other, Energy quantity);

    // returns true if the entity has moved
    bool Tick(EntityContainerInterface& container, const UniverseParameters& universeParameters);
    void Draw(QPainter& paint);

protected:
    virtual void TickImpl(EntityContainerInterface& container, const UniverseParameters& universeParameters) = 0;
    virtual void DrawImpl(QPainter& paint) = 0;

    void UseEnergy(Energy quantity) { energy_ -= quantity; }
    Energy TakeEnergy(Energy quantity);
    void Terminate() { terminated_ = true; }

    void SetColour(double red, double green, double blue) { colour_.setRgbF(red, green, blue); }
    void SetBearing(double bearing);
    void SetVelocity(double speed) { speed_ = speed; }
    void SetRadius(double radius) { radius_ = radius; }

private:
    Energy energy_; // TODO consider tracking energy used recenty via some sort of low pass filtered heat variable that decays over time
    bool terminated_ = false;
    Transform transform_;
    double radius_;
    double speed_;
    uint64_t age_;
    QColor colour_;

    // returns true if the entity has moved
    bool Move();
};

#endif // ENTITY_H

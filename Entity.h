#ifndef ENTITY_H
#define ENTITY_H

#include "Shape.h"
#include "EnergyPool.h"
#include "EntityContainerInterface.h"

#include <string_view>

class QPainter;

class Entity {
public:
    /**
     * Places stationary entity at coordinates with random bearing.
     */
    Entity(EnergyPool&& energy, double x, double y, double radius);
    Entity(EnergyPool&& energy, double x, double y, double radius, double bearing, double speed);
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
    virtual bool Tick(EntityContainerInterface& container) = 0;
    virtual void Draw(QPainter& paint) = 0;

protected:
    double radius_;
    double bearing_;
    double speed_;
    EnergyPool energy_;

    // returns true if the entity has moved
    bool Move();

private:
    double x_;
    double y_;
};

#endif // ENTITY_H

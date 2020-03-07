#ifndef ENTITY_H
#define ENTITY_H

#include "Shape.h"
#include "EntityContainerInterface.h"

#include <string_view>

class QPainter;

class Entity {
public:
    /**
     * Places stationary entity at coordinates with random bearing.
     */
    Entity(double x, double y, double radius);
    Entity(double x, double y, double radius, double bearing, double speed);
    virtual ~Entity();

    virtual std::string_view GetName() const = 0;

    const double& GetX() const;
    const double& GetY() const;
    const double& GetRadius() const;
    const double& GetBearing() const;
    Point GetLocation() const;

    bool Alive() const;

    // returns true if the entity has moved
    virtual bool Tick(EntityContainerInterface& container) = 0;
    virtual void Draw(QPainter& paint) = 0;

protected:
    double radius_;
    double bearing_;
    double speed_;
    double energy_;

    // returns true if the entity has moved
    bool Move();

private:
    double x_;
    double y_;
};

#endif // ENTITY_H

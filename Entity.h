#ifndef ENTITY_H
#define ENTITY_H

#include "Shape.h"

#include <memory>
#include <functional>

class QPainter;
class Entity;
class EntityContainer {
public:
    virtual ~EntityContainer();
    virtual void AddEntity(const std::shared_ptr<Entity>& entity) = 0;
    virtual void ForEachIn(const Rect& collide, const std::function<void(Entity&)>& action) const = 0;
    virtual void ForEachIn(const Circle& collide, const std::function<void(Entity&)>& action) const = 0;
};

class Entity {
public:
    /**
     * Places stationary entity at coordinates with random bearing.
     */
    Entity(double x, double y, double radius);
    Entity(double x, double y, double radius, double bearing, double speed);
    virtual ~Entity();

    const double& GetX() const;
    const double& GetY() const;
    const double& GetRadius() const;
    Point GetLocation() const;

    bool Alive() const;

    // returns true if the entity has moved
    virtual bool Tick(EntityContainer& container) = 0;
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

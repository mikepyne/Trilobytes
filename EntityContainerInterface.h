#ifndef ENTITYCONTAINERINTERFACE_H
#define ENTITYCONTAINERINTERFACE_H

#include "Shape.h"

#include <memory>
#include <functional>

class Entity;
class EntityContainerInterface {
public:
    virtual ~EntityContainerInterface(){}
    virtual void AddEntity(const std::shared_ptr<Entity>& entity) = 0;
    virtual void ForEachCollidingWith(const Point& collide, const std::function<void(Entity&)>& action) const = 0;
    virtual void ForEachCollidingWith(const Line& collide, const std::function<void(Entity&)>& action) const = 0;
    virtual void ForEachCollidingWith(const Rect& collide, const std::function<void(Entity&)>& action) const = 0;
    virtual void ForEachCollidingWith(const Circle& collide, const std::function<void(Entity&)>& action) const = 0;
};

#endif // ENTITYCONTAINERINTERFACE_H

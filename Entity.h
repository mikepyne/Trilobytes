#ifndef ENTITY_H
#define ENTITY_H

#include "Random.h"

class Entity {
public:
    /**
     * Places stationary entity at coordinates with random bearing.
     */
    Entity(double x, double y);
    Entity(double x, double y, double bearing, double speed);

protected:
    double x_;
    double y_;

    double bearing_;
    double speed_;
};

#endif // ENTITY_H

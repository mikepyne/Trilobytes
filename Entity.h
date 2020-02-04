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

    double DistanceSq(const  Entity& other)
    {
        return std::pow(std::abs(x_ - other.x_), 2) + std::pow(std::abs(y_ - other.y_), 2);
    }

protected:
    double x_;
    double y_;

    double bearing_;
    double speed_;
};

#endif // ENTITY_H

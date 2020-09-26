#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Shape.h"

/**
 * @brief Placeholder for a proper matrix based coordinate system
 */
struct Transform {
public:
    double x;
    double y;
    double rotation;

    Transform operator+(const Transform& other)
    {
        return { x + other.x, y + other.y, rotation + other.rotation };
    }
};

#endif // TRANSFORM_H

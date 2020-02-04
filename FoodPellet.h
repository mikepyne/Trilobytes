#ifndef FOODPELLET_H
#define FOODPELLET_H

#include "Entity.h"

#include <QPainter>

class FoodPellet : public Entity {
public:
    double radius_;

    FoodPellet(double x, double y)
        : Entity(x, y)
        , radius_(2.5)
    {
    }

    void Draw(QPainter& paint)
    {
        paint.setBrush(QColor(15, 235, 15));
        paint.drawEllipse(QPointF{x_, y_}, radius_, radius_);
    }
};

#endif // FOODPELLET_H

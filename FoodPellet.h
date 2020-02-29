#ifndef FOODPELLET_H
#define FOODPELLET_H

#include "Entity.h"

#include <QPainter>

class FoodPellet : public Entity {
public:
    FoodPellet(double x, double y)
        : Entity(x, y, 2.5)
    {
        energy_ = 1.0;
    }

    virtual std::string GetName() const override { return "FoodPellet"; }

    virtual bool Tick(EntityContainerInterface& /*container*/) override final
    {
        return false;
    }

    virtual void Draw(QPainter& paint) override final
    {
        paint.setBrush(QColor(15, 235, 15));
        paint.drawEllipse(QPointF{GetX(), GetY()}, radius_, radius_);
    }

    double Eat()
    {
        double toGive = energy_;
        energy_ = 0.0;
        return toGive;
    }
};

#endif // FOODPELLET_H

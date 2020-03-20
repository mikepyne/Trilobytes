#ifndef FOODPELLET_H
#define FOODPELLET_H

#include "Entity.h"

#include <QPainter>
class FoodPellet : public Entity {
public:
    FoodPellet(EnergyPool&& energy, double x, double y)
        : Entity(std::move(energy), x, y, 2.5)
    {
    }

    virtual ~FoodPellet() override final;

    virtual std::string_view GetName() const override { return "FoodPellet"; }

    virtual bool Tick(EntityContainerInterface& /*container*/) override final
    {
        return false;
    }

    virtual void Draw(QPainter& paint) override final
    {
        paint.setBrush(QColor(15, 235, 15));
        paint.drawEllipse(QPointF{GetX(), GetY()}, radius_, radius_);
    }

};

#endif // FOODPELLET_H

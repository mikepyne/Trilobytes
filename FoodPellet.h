#ifndef FOODPELLET_H
#define FOODPELLET_H

#include "Entity.h"

#include <QPainter>
class FoodPellet : public Entity {
public:
    FoodPellet(EnergyPool&& energy, double x, double y)
        : Entity(std::move(energy), x, y, 2.5, QColor::fromRgb(15, 235, 15))
    {
    }

    virtual ~FoodPellet() override final;

    virtual std::string_view GetName() const override { return "FoodPellet"; }

private:
    virtual void TickImpl(EntityContainerInterface& /*container*/) override final
    {
    }

    virtual void DrawImpl(QPainter& paint) override final
    {
        paint.drawEllipse(QPointF{GetX(), GetY()}, GetRadius(), GetRadius());
    }

};

#endif // FOODPELLET_H

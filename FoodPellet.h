#ifndef FOODPELLET_H
#define FOODPELLET_H

#include "Entity.h"

#include <QPainter>

class FeedDispenser;

class FoodPellet : public Entity {
public:
    FoodPellet(FeedDispenser& spawner, EnergyPool&& energy, double x, double y)
        : Entity(std::move(energy), x, y, 2.5, QColor::fromRgb(15, 235, 15))
        , spawner_(spawner)
    {
    }

    // TODO create feeder class that spawns a finite number of food pellets in a specified range, allowing multiple feeders to create an interesting food spawn topology
    // notify parent feeder that it can decrement its food pellet count
    virtual ~FoodPellet() override final;

    virtual std::string_view GetName() const override { return "FoodPellet"; }

private:
    FeedDispenser& spawner_;

    virtual void TickImpl(EntityContainerInterface& /*container*/) override final
    {
    }

    virtual void DrawImpl(QPainter& paint) override final
    {
        paint.drawEllipse(QPointF{GetX(), GetY()}, GetRadius(), GetRadius());
    }

};

#endif // FOODPELLET_H

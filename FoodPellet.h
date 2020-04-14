#ifndef FOODPELLET_H
#define FOODPELLET_H

#include "Entity.h"

class FeedDispenser;

class QPainter;

class FoodPellet : public Entity {
public:
    FoodPellet(FeedDispenser& spawner, EnergyPool&& energy, double x, double y);
    virtual ~FoodPellet() override final;

    virtual std::string_view GetName() const override { return "FoodPellet"; }

private:
    FeedDispenser& spawner_;

    virtual void TickImpl(EntityContainerInterface& container) override final;
    virtual void DrawImpl(QPainter& paint) override final;
};

#endif // FOODPELLET_H

#ifndef FOODPELLET_H
#define FOODPELLET_H

#include "Entity.h"

class FeedDispenser;

class QPainter;

class FoodPellet : public Entity {
public:
    FoodPellet(FeedDispenser& spawner, Energy energy, const Transform& transform);
    virtual ~FoodPellet() override final;

    virtual std::string_view GetName() const override { return "FoodPellet"; }

private:
    FeedDispenser& spawner_;

    virtual void TickImpl(EntityContainerInterface& /*container*/) override final {}
    virtual void DrawImpl(QPainter& paint) override final;
};

#endif // FOODPELLET_H

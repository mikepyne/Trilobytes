#ifndef FOODPELLET_H
#define FOODPELLET_H

#include "Entity.h"

class FeedDispenser;

class QPainter;

class FoodPellet : public Entity {
public:
    FoodPellet(const std::shared_ptr<FeedDispenser>& spawner, Energy energy, const Transform& transform);
    ~FoodPellet() override;

    virtual std::string_view GetName() const override { return "FoodPellet"; }

private:
    const std::shared_ptr<FeedDispenser> spawner_;

    virtual void TickImpl(EntityContainerInterface& /*container*/, const UniverseParameters& /*universeParameters*/) override final {}
    virtual void DrawImpl(QPainter& paint) override final;
};

#endif // FOODPELLET_H

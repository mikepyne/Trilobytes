#ifndef SPIKE_H
#define SPIKE_H

#include "Entity.h"

class Spike : public Entity {
public:
    Spike(const Transform& transform);
    ~Spike() override;

    virtual std::string_view GetName() const override { return "Spike"; }

protected:
    virtual void TickImpl(EntityContainerInterface& container, const UniverseParameters& universeParameters) override;
    virtual void DrawImpl(QPainter& paint) override;
};

#endif // SPIKE_H

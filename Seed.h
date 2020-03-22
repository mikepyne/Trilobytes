#ifndef SEED_H
#define SEED_H

#include "Entity.h"

class Seed : public Entity {
public:
    Seed(EnergyPool&& energy, double x, double y, unsigned germinationDelay);

    virtual std::string_view GetName() const override { return "Seed"; }

    virtual bool Tick(EntityContainerInterface& container) override;
    virtual void Draw(QPainter& paint) override;

private:
    unsigned germinationDelay_;
};

#endif // SEED_H

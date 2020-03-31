#ifndef SEED_H
#define SEED_H

#include "Entity.h"

class Seed : public Entity {
public:
    Seed(EnergyPool&& energy, double x, double y, unsigned germinationDelay);

    virtual std::string_view GetName() const override { return "Seed"; }


protected:
    virtual void TickImpl(EntityContainerInterface& container) override;
    virtual void DrawImpl(QPainter& paint) override;

private:
    unsigned germinationDelay_;
};

#endif // SEED_H

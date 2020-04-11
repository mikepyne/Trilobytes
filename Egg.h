#ifndef EGG_H
#define EGG_H

#include "Entity.h"
#include "EnergyPool.h"
#include "Genome/Genome.h"
#include "NeuralNetwork.h"

class Egg : public Entity {
public:
    Egg(EnergyPool&& energy, double x, double y, NeuralNetwork&& brain, std::shared_ptr<Genome> genome, unsigned hatchingDelay);

    virtual std::string_view GetName() const override { return "Egg"; }

protected:
    virtual void TickImpl(EntityContainerInterface& container) override;
    virtual void DrawImpl(QPainter& paint) override;

private:
    std::shared_ptr<Genome> genome_;
    NeuralNetwork brain_;
    unsigned hatchingDelay_;
};

#endif // EGG_H

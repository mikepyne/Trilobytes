#ifndef EGG_H
#define EGG_H

#include "Entity.h"
#include "Genome/Genome.h"
#include "NeuralNetwork.h"

class Egg : public Entity {
public:
    Egg(Energy energy, const Transform& transform, std::shared_ptr<Genome> genomeOne, std::shared_ptr<Genome> genomeTwo, unsigned hatchingDelay);

    virtual std::string_view GetName() const override { return "Egg"; }

protected:
    virtual void TickImpl(EntityContainerInterface& container) override;
    virtual void DrawImpl(QPainter& paint) override;

private:
    std::shared_ptr<Genome> genomeOne_;
    std::shared_ptr<Genome> genomeTwo_;
    unsigned hatchingDelay_;
};

#endif // EGG_H

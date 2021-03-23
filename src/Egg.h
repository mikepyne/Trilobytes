#ifndef EGG_H
#define EGG_H

#include "Entity.h"
#include "Genome/Genome.h"
#include "Utility/NeuralNetwork.h"

class Egg : public Entity {
public:
    Egg(std::shared_ptr<Swimmer>&& parent, Energy energy, const Transform& transform, std::shared_ptr<Genome> genomeOne, std::shared_ptr<Genome> genomeTwo, unsigned hatchingDelay);
    virtual ~Egg() override;

    virtual std::string_view GetName() const override { return "Egg"; }

protected:
    virtual void TickImpl(EntityContainerInterface& container, const UniverseParameters& universeParameters) override;
    virtual void DrawImpl(QPainter& paint) override;

private:
    std::shared_ptr<Swimmer> mother_;
    std::shared_ptr<Genome> genomeOne_;
    std::shared_ptr<Genome> genomeTwo_;
    unsigned hatchingDelay_;
};

#endif // EGG_H

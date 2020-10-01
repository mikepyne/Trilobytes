#ifndef SWIMMER_H
#define SWIMMER_H

#include "Entity.h"
#include "NeuralNetwork.h"
#include "Genome/Genome.h"

#include <memory>

class Swimmer : public Entity {
public:
    Swimmer(Energy energy, const Transform& transform);
    Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome);
    virtual ~Swimmer() override final;

    virtual std::string_view GetName() const override { return "Swimmer"; }

    std::shared_ptr<Entity> GiveBirth();
    unsigned GetBrainInputCount() const { return brain_->GetInputCount(); }
    unsigned GetBrainOutputCount() const { return brain_->GetOutputCount(); }

    const NeuralNetwork& InspectBrain() { return *brain_; };
    const std::vector<std::shared_ptr<Sense>>& InspectSenses() { return senses_; };

protected:
    virtual void TickImpl(EntityContainerInterface& container) override final;
    virtual void DrawImpl(QPainter& paint) override final;

private:
    std::shared_ptr<Genome> genome_;
    std::shared_ptr<NeuralNetwork> brain_;
    std::vector<std::shared_ptr<Sense>> senses_;

    static std::vector<std::shared_ptr<Gene>> CreateDefaultGenome();

};

#endif // SWIMMER_H

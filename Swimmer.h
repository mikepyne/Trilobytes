#ifndef SWIMMER_H
#define SWIMMER_H

#include "Entity.h"
#include "NeuralNetwork.h"
#include "Genome/Genome.h"
#include "Genome/GenePigment.h"
#include "Sensors/SenseEntityRaycast.h"
#include "Sensors/SenseEntitiesTouching.h"
#include "Sensors/SenseEntitiesInArea.h"
#include "Sensors/SenseMagneticField.h"
#include "Sensors/SenseRandom.h"

class Swimmer : public Entity {
public:
    Swimmer(EnergyPool&& energy, double x, double y);
    Swimmer(EnergyPool&& energy, double x, double y, NeuralNetwork&& brain, std::shared_ptr<Genome> genome);
    virtual ~Swimmer() override final;

    virtual std::string_view GetName() const override { return "Swimmer"; }

    std::shared_ptr<Entity> GiveBirth();
    unsigned GetBrainInputCount() const { return brain_.GetInputCount(); }
    unsigned GetBrainOutputCount() const { return brain_.GetOutputCount(); }

    const NeuralNetwork& InspectBrain() { return brain_; };
    const std::vector<std::shared_ptr<Sense>>& InspectSenses() { return senses_; };

protected:
    virtual void TickImpl(EntityContainerInterface& container) override final;
    virtual void DrawImpl(QPainter& paint) override final;

private:
    std::shared_ptr<Genome> genome_;
    NeuralNetwork brain_;
    std::vector<std::shared_ptr<Sense>> senses_;

    // TODO pre-calculate colour and set colour in Entity base class
    QColor tempPigments_;

    static std::vector<std::shared_ptr<Gene>> CreateDefaultGenome();

    void ApplyGenome();
};

#endif // SWIMMER_H

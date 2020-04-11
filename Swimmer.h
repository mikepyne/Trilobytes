#ifndef SWIMMER_H
#define SWIMMER_H

#include "Entity.h"
#include "NeuralNetwork.h"
#include "Genome/Genome.h"
#include "Genome/GenePigment.h"
#include "Sensors/SenseEntityRaycast.h"
#include "Sensors/SenseEntityPresence.h"
#include "Sensors/SenseEntityDistance.h"
#include "Sensors/SenseMagneticField.h"
#include "Sensors/SenseRandom.h"

class Swimmer : public Entity {
public:
    Swimmer(EnergyPool&& energy, double x, double y);
    Swimmer(EnergyPool&& energy, double x, double y, NeuralNetwork&& brain, std::shared_ptr<Genome> genome);
    virtual ~Swimmer() override final;

    virtual std::string_view GetName() const override { return "Swimmer"; }

    std::shared_ptr<Entity> GiveBirth();

protected:
    virtual void TickImpl(EntityContainerInterface& container) override final;
    virtual void DrawImpl(QPainter& paint) override final;

private:
    std::shared_ptr<Genome> genome_;
    NeuralNetwork brain_;
    SenseEntityPresence taste_;
    SenseEntityDistance leftAntenna_;
    SenseEntityDistance rightAntenna_;
    SenseEntityRaycast echoLocator_;
    SenseMagneticField compass_;
    SenseRandom rand_;

    // TODO pre-calculate colour and remove from Entity base class
    QColor tempPigments_;

    static std::vector<std::shared_ptr<Gene>> CreateDefaultGenome();

    void ApplyGenome();
};

#endif // SWIMMER_H

#ifndef SWIMMER_H
#define SWIMMER_H

#include "Entity.h"
#include "NeuralNetwork.h"
#include "Sensors/SenseEntityPresence.h"
#include "Sensors/SenseEntityDistance.h"
#include "Sensors/SenseMagneticField.h"
#include "Sensors/SenseRandom.h"

class Swimmer : public Entity {
public:
    Swimmer(EnergyPool&& energy, double x, double y);
    Swimmer(EnergyPool&& energy, double x, double y, NeuralNetwork&& brain);
    virtual ~Swimmer() override final;

    virtual std::string_view GetName() const override { return "Swimmer"; }

    virtual bool Tick(EntityContainerInterface& container) override final;
    virtual void Draw(QPainter& paint) override final;

    std::shared_ptr<Swimmer> GiveBirth();

private:
    NeuralNetwork brain_;
    SenseEntityPresence taste_;
    SenseEntityDistance leftAntenna_;
    SenseEntityDistance rightAntenna_;
    SenseMagneticField compass_;
    SenseRandom rand_;
};

#endif // SWIMMER_H

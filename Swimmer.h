#ifndef SWIMMER_H
#define SWIMMER_H

#include "Entity.h"
#include "Random.h"
#include "NeuralNetwork.h"
#include "FoodPellet.h"
#include "Sensors/SenseEntityPresence.h"
#include "Sensors/SenseMagneticField.h"
#include "Sensors/SenseRandom.h"

#include <QPainter>

#include <iostream>
#include <math.h>

class Swimmer : public Entity {
public:
    Swimmer(double x, double y);
    Swimmer(double x, double y, NeuralNetwork&& brain);

    virtual std::string GetName() const override { return "Swimmer"; }

    virtual bool Tick(EntityContainerInterface& container) override final;
    virtual void Draw(QPainter& paint) override final;

    std::shared_ptr<Swimmer> GiveBirth();

private:
    NeuralNetwork brain_;
    SenseEntityPresence taste_;
    SenseMagneticField compass_;
    SenseRandom rand_;
};

#endif // SWIMMER_H

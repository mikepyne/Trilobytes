#ifndef SWIMMER_H
#define SWIMMER_H

#include "Entity.h"
#include "Random.h"
#include "NeuralNetwork.h"

#include <QPainter>

#include <iostream>
#include <math.h>

class Swimmer : public Entity {
public:
    Swimmer(double x, double y);
    Swimmer(double x, double y, NeuralNetwork&& brain);

    virtual bool Tick(EntityContainerInterface& container) override final;
    virtual void Draw(QPainter& paint) override final;

    std::shared_ptr<Swimmer> GiveBirth();

private:
    NeuralNetwork brain_;
};

#endif // SWIMMER_H

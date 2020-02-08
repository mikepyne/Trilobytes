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
    double energy_ = 1.0;
    double radius_;

    Swimmer(double x, double y);

    void Tick();

    void Draw(QPainter& paint);

    Swimmer GiveBirth();

private:
    NeuralNetwork brain_;

    Swimmer(double x, double y, NeuralNetwork&& brain);
};

#endif // SWIMMER_H

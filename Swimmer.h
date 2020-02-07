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
    double radius_;

    Swimmer(double x, double y);

    void Tick();

    void Draw(QPainter& paint);

private:
    NeuralNetwork brain_;
};

#endif // SWIMMER_H

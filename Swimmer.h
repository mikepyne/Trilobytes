#ifndef SWIMMER_H
#define SWIMMER_H

#include "Random.h"

#include <QPainter>

#include <iostream>
#include <math.h>

class Swimmer {
public:
    Swimmer(double x, double y)
        : x_(x)
        , y_(y)
        , bearing_(0.0)
        , speed_(0.1)
    {
    }

    void Tick()
    {
        x_ += std::sin(bearing_) * speed_;
        y_ += -(std::cos(bearing_) * speed_);

        bearing_ += Random(-0.1, 0.1);
    }

    void Draw(QPainter& paint)
    {
        paint.setBrush(QColor(15, 15, 235));
        paint.drawEllipse({x_, y_}, 12.5, 12.5);
    }
private:
    double x_;
    double y_;

    double bearing_;
    double speed_;
};

#endif // SWIMMER_H

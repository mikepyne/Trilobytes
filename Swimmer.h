#ifndef SWIMMER_H
#define SWIMMER_H

#include "Random.h"

#include <QPainter>

class Swimmer {
public:
    Swimmer(double x, double y)
        : x_(x)
        , y_(y)
    {
    }

    void Tick()
    {
        x_ += Random(-10.0, 10.0);
        y_ += Random(-10.0, 10.0);
    }

    void Draw(QPainter& paint)
    {
        paint.setBrush(QColor(15, 15, 235));
        paint.drawEllipse({x_, y_}, 12.5, 12.5);
    }
private:
    double x_;
    double y_;
};

#endif // SWIMMER_H

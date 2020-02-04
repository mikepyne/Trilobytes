#include "Swimmer.h"

Swimmer::Swimmer(double x, double y)
    : Entity(x, y)
    , radius_(6.0)
{
    speed_ = 0.5;
}

void Swimmer::Tick()
{
    x_ += std::sin(bearing_) * speed_;
    y_ += -(std::cos(bearing_) * speed_);

    bearing_ += Random(-0.1, 0.1);
}

void Swimmer::Draw(QPainter& paint)
{
    paint.setBrush(QColor(15, 15, 235));
    paint.drawEllipse({x_, y_}, radius_, radius_);
}

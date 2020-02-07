#include "Swimmer.h"

#include <cmath>

Swimmer::Swimmer(double x, double y)
    : Entity(x, y)
    , radius_(6.0)
    , brain_(3, 3)
{
    speed_ = 0.5;
}

void Swimmer::Tick()
{
    auto& out = brain_.ForwardPropogate({std::fmod(x_, 2.0), std::fmod(y_, 2.0), std::fmod(bearing_, 2.0)});

    bearing_ += (out[1]);

    x_ += std::sin(bearing_) * speed_;
    y_ += -(std::cos(bearing_) * speed_);

}

void Swimmer::Draw(QPainter& paint)
{
    paint.setBrush(QColor(15, 15, 235));
    paint.drawEllipse({x_, y_}, radius_, radius_);
}

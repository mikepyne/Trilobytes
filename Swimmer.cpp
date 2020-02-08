#include "Swimmer.h"

#include <cmath>

Swimmer::Swimmer(double x, double y)
    : Swimmer(x, y, NeuralNetwork(3, 3))
{
}

Swimmer::Swimmer(double x, double y, NeuralNetwork&& brain)
    : Entity(x, y)
    , radius_(6.0)
    , brain_(std::move(brain))
{
    speed_ = 0.5;
}

void Swimmer::Tick()
{
    auto& out = brain_.ForwardPropogate({Random::Real(-2.0, 2.0), energy_ - 2.0, (bearing_ / 3.14) - 1});

    bearing_ += (out[1]);
    if (bearing_ < 0.0) {
        bearing_ += 6.28;
    } else if (bearing_ > 6.28) {
        bearing_ -= 6.28;
    }

    x_ += std::sin(bearing_) * speed_;
    y_ += -(std::cos(bearing_) * speed_);

}

void Swimmer::Draw(QPainter& paint)
{
    paint.setBrush(QColor(15, 15, 235));
    paint.drawEllipse({x_, y_}, radius_, radius_);
}

Swimmer Swimmer::GiveBirth()
{
    return Swimmer(x_, y_, brain_.Mutated());
}


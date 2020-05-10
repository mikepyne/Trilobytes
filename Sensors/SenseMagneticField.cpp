#include "SenseMagneticField.h"

#include "Swimmer.h"
#include "Utils.h"
#include "Shape.h"

#include <QPainter>

SenseMagneticField::SenseMagneticField(Swimmer& owner)
    : Sense(owner, 2, 1)
    , lastVector_(0.0)
    , lastDistance_(0.0)
{
}

void SenseMagneticField::Draw(QPainter& paint) const
{
    auto endX = owner_.GetX();
    auto endY = owner_.GetY();
    auto distance = (lastDistance_ + 1) * 20;
    auto bearing = (lastVector_ + 1) * EoBE::Pi;

    endX += std::sin(bearing) * distance;
    endY += std::cos(bearing) * distance;

    paint.setPen(QColor::fromRgb(0, 0, 0));
    paint.drawLine(owner_.GetX(), owner_.GetY(), endX, endY);
}

void SenseMagneticField::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface&, const Sense::UniverseInfoStructRef&)
{
    lastVector_ = GetBearing(Point{ owner_.GetX(), owner_.GetY() }, Point{ 0, 0 }) / EoBE::Pi;
    lastDistance_ = 2 * std::tanh(GetDistanceSquare(Point{ owner_.GetX(), owner_.GetY() }, Point{ 0, 0 }) / 500000) - 1.0;
    inputs[0] = lastVector_;
    inputs[1] = lastDistance_;
}

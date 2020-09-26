#include "SenseMagneticField.h"

#include "Swimmer.h"
#include "Utils.h"
#include "Shape.h"

#include <QPainter>

SenseMagneticField::SenseMagneticField(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner)
    : Sense(network, outputConnections, owner)
    , lastVector_(0.0)
    , lastDistance_(0.0)
{
}

void SenseMagneticField::Draw(QPainter& paint) const
{
    auto endX = owner_.GetTransform().x;
    auto endY = owner_.GetTransform().y;
    auto distance = (lastDistance_ + 1) * 20;
    auto bearing = (lastVector_ + 1) * EoBE::Pi;

    endX += std::sin(bearing) * distance;
    endY += std::cos(bearing) * distance;

    paint.setPen(QColor::fromRgb(0, 0, 0));
    paint.drawLine(owner_.GetTransform().x, owner_.GetTransform().y, endX, endY);
}

void SenseMagneticField::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface&, const Sense::UniverseInfoStructRef&)
{
    lastVector_ = GetBearing(Point{ owner_.GetTransform().x, owner_.GetTransform().y }, Point{ 0, 0 }) / EoBE::Pi;
    lastDistance_ = 2 * std::tanh(GetDistanceSquare(Point{ owner_.GetTransform().x, owner_.GetTransform().y }, Point{ 0, 0 }) / 500000) - 1.0;
    inputs[0] = lastVector_;
    inputs[1] = lastDistance_;
}

#include "SenseMagneticField.h"

#include "Swimmer.h"
#include "Utils.h"
#include "Shape.h"

#include <QPainter>

SenseMagneticField::SenseMagneticField(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner)
    : Sense(network, outputConnections, owner)
{
}

void SenseMagneticField::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface&) const
{
    inputs.at(0) = GetBearing(Point{ owner_.GetTransform().x, owner_.GetTransform().y }, Point{ 0, 0 }) / EoBE::Pi;
    inputs.at(1) = 2 * std::tanh(GetDistanceSquare(Point{ owner_.GetTransform().x, owner_.GetTransform().y }, Point{ 0, 0 }) / 500000) - 1.0;
}

void SenseMagneticField::Draw(QPainter& paint) const
{
    auto endX = owner_.GetTransform().x;
    auto endY = owner_.GetTransform().y;
    auto bearing = 1 + (GetBearing(Point{ owner_.GetTransform().x, owner_.GetTransform().y }, Point{ 0, 0 }) / EoBE::Pi);
    auto distance = 1 + (2 * std::tanh(GetDistanceSquare(Point{ owner_.GetTransform().x, owner_.GetTransform().y }, Point{ 0, 0 }) / 500000) - 1.0);
    distance *= 20;
    bearing *= EoBE::Pi;

    endX += std::sin(bearing) * distance;
    endY += std::cos(bearing) * distance;

    paint.setPen(QColor::fromRgb(0, 0, 0));
    paint.drawLine(owner_.GetTransform().x, owner_.GetTransform().y, endX, endY);
}

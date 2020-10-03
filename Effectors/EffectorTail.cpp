#include "EffectorTail.h"

#include "Swimmer.h"

#include <QPainter>

EffectorTail::EffectorTail(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, Swimmer& owner)
    : Effector(network, inputConnections, owner)
{
}

void EffectorTail::Draw(QPainter& /*paint*/) const
{
    // TODO
}

void EffectorTail::PerformActions(const std::vector<double>& actionValues, const EntityContainerInterface& /*entities*/)
{
    owner_.AdjustVelocity(actionValues.at(0));
    owner_.AdjustBearing(actionValues.at(1) / EoBE::Tau);
    owner_.AdjustBearing(actionValues.at(2) / EoBE::Tau);
}

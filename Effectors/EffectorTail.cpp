#include "EffectorTail.h"

#include "Swimmer.h"

#include <QPainter>

EffectorTail::EffectorTail(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, Swimmer& owner)
    : Effector(network, inputConnections, owner)
{
}

EffectorTail::~EffectorTail()
{
}

void EffectorTail::Draw(QPainter& /*paint*/) const
{
    // TODO
}

Energy EffectorTail::PerformActions(const std::vector<double>& actionValues, const EntityContainerInterface& /*entities*/, const UniverseParameters& /*universeParameters*/)
{
    const double& acceleration = actionValues.at(0);
    const double& clockwiseTorque = actionValues.at(1);
    const double& antiClockwiseTorque = actionValues.at(2);

    owner_.AdjustVelocity(acceleration);
    owner_.AdjustBearing(clockwiseTorque / EoBE::Tau);
    owner_.AdjustBearing(antiClockwiseTorque / EoBE::Tau);

    /*
     * We want to make acceleration costlier as an organism gets faster
     */
    Energy accelerationCost = std::abs(owner_.GetVelocity() * acceleration) * 75_uj;
    Energy clockwiseRotationCost = std::abs(clockwiseTorque) * 60_uj;
    Energy antiClockwiseRotationCost = std::abs(antiClockwiseTorque) * 60_uj;
    return accelerationCost + clockwiseRotationCost + antiClockwiseRotationCost;
}

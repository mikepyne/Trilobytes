#include "SenseLunarCycle.h"

SenseLunarCycle::SenseLunarCycle(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner)
    : Sense(network, outputConnections, owner)
{
}

SenseLunarCycle::~SenseLunarCycle()
{
}

void SenseLunarCycle::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& /*entities*/, const UniverseParameters& universeParameters) const
{
    inputs.at(0) = universeParameters.lunarCycle_;
}

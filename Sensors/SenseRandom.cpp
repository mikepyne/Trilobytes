#include "SenseRandom.h"

#include "Random.h"

SenseRandom::SenseRandom(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner)
    : Sense(network, outputConnections, owner)
{
}

void SenseRandom::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& /*entities*/, const Sense::UniverseInfoStructRef& /*environment*/) const
{
    auto rand = Random::Numbers(inputs.size(), -1.0, 1.0);
    inputs.swap(rand);
}

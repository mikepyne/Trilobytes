#include "GeneEffectorTail.h"

#include "Swimmer.h"
#include "Effectors/EffectorTail.h"

GeneEffectorTail::GeneEffectorTail(unsigned hiddenLayers, unsigned inputCount)
    : GeneEffectorBase(hiddenLayers, inputCount, 3)
{
}

GeneEffectorTail::GeneEffectorTail(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance, double mutationProbability)
    : GeneEffectorBase(network, inputConnections, dominance, mutationProbability)
{
}

std::shared_ptr<Gene> GeneEffectorTail::Mutate() const
{
    auto [ network, inputs, dominance, mutationProbability ] = GetMutated({});
    return std::make_shared<GeneEffectorTail>(std::move(network), std::move(inputs), dominance, mutationProbability);
}

void GeneEffectorTail::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.effectors.push_back(std::make_shared<EffectorTail>(network_, inputs_, owner));
}

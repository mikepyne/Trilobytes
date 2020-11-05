#include "GeneEffectorTail.h"

#include "Swimmer.h"
#include "Effectors/EffectorTail.h"

GeneEffectorTail::GeneEffectorTail(unsigned hiddenLayers, unsigned inputCount)
    : GeneEffectorBase(hiddenLayers, inputCount, 3)
{
    // No additional mutations
}

GeneEffectorTail::GeneEffectorTail(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance, double mutationProbability)
    : GeneEffectorBase(network, inputConnections, dominance, mutationProbability)
{
    // No additional mutations
}

void GeneEffectorTail::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.effectors.push_back(std::make_shared<EffectorTail>(GetNetwork(), GetInputConnections(), owner));
}

std::shared_ptr<Gene> GeneEffectorTail::Copy() const
{
    return std::make_shared<GeneEffectorTail>(GetNetwork(), GetInputConnections(), GetDominance(), GetMutationProbability());
}

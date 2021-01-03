#include "GeneEffectorFilterMouth.h"

#include "Swimmer.h"
#include "Phenotype.h"
#include "Effectors/EffectorFilterMouth.h"

GeneEffectorFilterMouth::GeneEffectorFilterMouth(unsigned hiddenLayers, unsigned inputCount)
    : GeneEffectorBase(hiddenLayers, inputCount, 1)
{
}

GeneEffectorFilterMouth::GeneEffectorFilterMouth(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance)
    : GeneEffectorBase(network, inputConnections, dominance)
{
}

GeneEffectorFilterMouth::~GeneEffectorFilterMouth()
{
}

void GeneEffectorFilterMouth::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.effectors.push_back(std::make_shared<EffectorFilterMouth>(GetNetwork(), GetInputConnections(), owner));
    target.baseMetabolism += GetMetabolicCost();
}

std::shared_ptr<Gene> GeneEffectorFilterMouth::Copy() const
{
    return std::make_shared<GeneEffectorFilterMouth>(GetNetwork(), GetInputConnections(), GetDominance());
}

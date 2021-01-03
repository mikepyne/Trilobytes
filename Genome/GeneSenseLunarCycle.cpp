#include "GeneSenseLunarCycle.h"

#include "Phenotype.h"
#include "Sensors/SenseLunarCycle.h"

GeneSenseLunarCycle::GeneSenseLunarCycle(unsigned hiddenLayers, unsigned outputCount)
    : GeneSenseBase(hiddenLayers, 1, outputCount)
{
}

GeneSenseLunarCycle::GeneSenseLunarCycle(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, double dominance)
    : GeneSenseBase(network, outputConnections, dominance)
{
}

GeneSenseLunarCycle::~GeneSenseLunarCycle()
{
}

void GeneSenseLunarCycle::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseLunarCycle>(GetNetwork(), GetOutputConnections(), owner));
    target.baseMetabolism += GetMetabolicCost();
}

std::shared_ptr<Gene> GeneSenseLunarCycle::Copy() const
{
    return std::make_shared<GeneSenseLunarCycle>(GetNetwork(), GetOutputConnections(), GetDominance());
}

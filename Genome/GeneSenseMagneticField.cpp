#include "GeneSenseMagneticField.h"

#include "Phenotype.h"
#include "Sensors/SenseMagneticField.h"

GeneSenseMagneticField::GeneSenseMagneticField(unsigned outputCount)
    : GeneSenseMagneticField(std::make_shared<NeuralNetwork>(0, 2, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(2, outputCount), Random::Number(0.0, 100.0), Random::Number(0.0, 1.0))
{
}

GeneSenseMagneticField::GeneSenseMagneticField(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, double dominance, double mutationProbability)
    : GeneSenseBase(network, outputConnections, dominance, mutationProbability)
{
}

void GeneSenseMagneticField::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseMagneticField>(GetNetwork(), GetOutputConnections(), owner));
}

std::shared_ptr<Gene> GeneSenseMagneticField::Copy() const
{
    return std::make_shared<GeneSenseMagneticField>(GetNetwork(), GetOutputConnections(), GetDominance(), GetMutationProbability());
}

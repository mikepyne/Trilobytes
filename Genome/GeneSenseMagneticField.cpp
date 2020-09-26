#include "GeneSenseMagneticField.h"

#include "Phenotype.h"
#include "Sensors/SenseMagneticField.h"

GeneSenseMagneticField::GeneSenseMagneticField(unsigned outputCount)
    : GeneSenseMagneticField(std::make_shared<NeuralNetwork>(0, 2, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(2, outputCount), Random::Number(0.0, 100.0), Random::Number(0.0, 1.0))
{
}

GeneSenseMagneticField::GeneSenseMagneticField(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , network_(network)
    , outputConnections_(outputConnections)
{
}

std::shared_ptr<Gene> GeneSenseMagneticField::Mutate() const
{
    return std::make_shared<GeneSenseMagneticField>(network_->Mutated(), outputConnections_->Mutated(), GetMutatedDominance(), GetMutatedMutationProbability());
}

void GeneSenseMagneticField::ExpressGene(const Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseMagneticField>(network_, outputConnections_, owner));
}

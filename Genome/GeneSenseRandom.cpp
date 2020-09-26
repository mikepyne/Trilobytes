#include "GeneSenseRandom.h"

#include "Phenotype.h"
#include "Sensors/SenseRandom.h"

GeneSenseRandom::GeneSenseRandom(unsigned inputCount, unsigned outputCount)
    : GeneSenseRandom(std::make_shared<NeuralNetwork>(0, inputCount, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(NeuralNetworkConnector(inputCount, outputCount)), Random::Number(0.0, 100.0), Random::Number(0.0, 0.01))
{
}

GeneSenseRandom::GeneSenseRandom(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , network_(network)
    , outputConnections_(outputConnections)
{
}

std::shared_ptr<Gene> GeneSenseRandom::Mutate() const
{
    // TODO allow changing of number of random inputs
    return std::make_shared<GeneSenseRandom>(network_->Mutated(), outputConnections_->Mutated(), GetMutatedDominance(), GetMutatedMutationProbability());
}

void GeneSenseRandom::ExpressGene(const Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseRandom>(network_, outputConnections_, owner));
}

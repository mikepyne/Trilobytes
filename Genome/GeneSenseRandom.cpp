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
    std::shared_ptr<NeuralNetwork> network = network_;
    std::shared_ptr<NeuralNetworkConnector> outputConnections = outputConnections_;

    switch(Random::Number(0, 1)) {
    case 0 :
        network = network->Mutated();
        break;
    case 1 :
        outputConnections = outputConnections->Mutated();
        break;
    }

    return std::make_shared<GeneSenseRandom>(network, outputConnections, GetMutatedDominance(), GetMutatedMutationProbability());
}

void GeneSenseRandom::ExpressGene(const Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseRandom>(network_, outputConnections_, owner));
}

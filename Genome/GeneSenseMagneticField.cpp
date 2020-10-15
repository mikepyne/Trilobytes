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
    // TODO allow the Sense to evolve exactly where it points to
    // TODO perhaps threee nodes, 2 for direction (-1 to 1 facing point to not facing point, 
    // -1 to 1 left side facing to right side facing, 
    // & configurable range for configurable distance)
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

    return std::make_shared<GeneSenseMagneticField>(network, outputConnections, GetMutatedDominance(), GetMutatedMutationProbability());
}

void GeneSenseMagneticField::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseMagneticField>(network_, outputConnections_, owner));
}

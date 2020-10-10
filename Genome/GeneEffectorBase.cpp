#include "GeneEffectorBase.h"

GeneEffectorBase::GeneEffectorBase(unsigned hiddenLayers, unsigned inputCount, unsigned outputCount)
    : GeneEffectorBase(std::make_shared<NeuralNetwork>(hiddenLayers, outputCount, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(inputCount, outputCount), Random::Number(0.0, 100.0), Random::Number(0.0, 1.0))
{
}

GeneEffectorBase::GeneEffectorBase(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , network_(network)
    , inputs_(inputConnections)
{
}

std::tuple<std::shared_ptr<NeuralNetwork>, std::shared_ptr<NeuralNetworkConnector>, double, double> GeneEffectorBase::GetMutated(std::vector<std::function<void ()> > additionalMutations) const
{
    std::shared_ptr<NeuralNetwork> network = network_;
    std::shared_ptr<NeuralNetworkConnector> inputs = inputs_;
    double dominance = GetDominance();
    double mutationProbability = GetMutationProbability();

    switch(Random::Number<int>(0, 3 + additionalMutations.size())) {
    case 0 :
        network = network->Mutated();
        break;
    case 1 :
        inputs = inputs->Mutated();
        break;
    case 2 :
        dominance = GetMutatedDominance();
        break;
    case 3 :
        mutationProbability = GetMutatedMutationProbability();
        break;
    default:
        // If none of the standard mutations were selected, do a custom one
        Random::Item(additionalMutations)();
        break;
    }
    return { std::move(network), std::move(inputs), dominance, mutationProbability };
}

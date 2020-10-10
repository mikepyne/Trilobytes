#include "GeneSenseTraitsBase.h"

#include "Random.h"

#include <functional>

GeneSenseTraitsBase::GeneSenseTraitsBase(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform)
    : GeneSenseTraitsBase(std::move(toDetect), std::make_shared<NeuralNetwork>(hiddenLayers, toDetect.size(), NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(toDetect.size(), outputCount), transform, Random::Number(0.0, 100.0), Random::Number(0.0, 1.0))
{
}

GeneSenseTraitsBase::GeneSenseTraitsBase(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , toDetect_(std::move(toDetect))
    , network_(network)
    , outputs_(outputConnections)
    , transform_(transform)
{
}

std::tuple<std::vector<SenseTraitsBase::TraitNormaliser>, std::shared_ptr<NeuralNetwork>, std::shared_ptr<NeuralNetworkConnector>, Transform, double, double> GeneSenseTraitsBase::GetMutated(std::vector<std::function<void ()> > additionalMutations) const
{
    std::vector<SenseTraitsBase::TraitNormaliser> toDetect = toDetect_;
    std::shared_ptr<NeuralNetwork> network = network_;
    std::shared_ptr<NeuralNetworkConnector> outputs = outputs_;
    Transform transform = transform_;
    double dominance = GetDominance();
    double mutationProbability = GetMutationProbability();

    switch(Random::Number<int>(0, 5 + additionalMutations.size())) {
    case 0 :
        network = network->Mutated();
        break;
    case 1 :
        outputs = outputs->Mutated();
        break;
    case 2 :
        transform = {
            Random::GaussianAdjustment<double>(transform.x, 0.1),
            Random::GaussianAdjustment<double>(transform.y, 0.1),
            Random::GaussianAdjustment<double>(transform.rotation, 0.1),
        };
        break;
    case 3 :
        dominance = GetMutatedDominance();
        break;
    case 4 :
        mutationProbability = GetMutatedMutationProbability();
        break;
    case 5 :
        switch(Random::Number(0, 3)) {
        case 0 :
            // Adjust trait strength
            if (!toDetect.empty()) {
                auto& item = Random::Item(toDetect);
                double newMin = Random::GaussianAdjustment<double>(item.range.Min(), 0.1);
                double newMax = Random::GaussianAdjustment<double>(item.range.Max(), 0.1);
                item.range = { newMin, newMax };
            }
            break;
        case 1 :
            // Switch trait
            if (!toDetect.empty()) {
                auto& item = Random::Item(toDetect);
                item.trait = Random::Item(SenseTraitsBase::ALL_TRAITS);
            }
            break;
        case 2 :
            // Add trait
            // TODO requires ability to add/remove columns in network & connectors
            break;
        case 3 :
            // Remove trait
            // TODO requires ability to add/remove columns in network & connectors
            break;
        }
        break;
    default:
        // If none of the standard mutations were selected, do a custom one
        Random::Item(additionalMutations)();
        break;
    }
    return { std::move(toDetect), std::move(network), std::move(outputs), std::move(transform), dominance, mutationProbability };
}

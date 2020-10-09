#include "GeneSenseEntitiesTouching.h"

#include "Random.h"
#include "Swimmer.h"
#include "Sensors/SenseTraitsBase.h"
#include "Sensors/SenseEntitiesTouching.h"

GeneSenseEntitiesTouching::GeneSenseEntitiesTouching(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform)
    : GeneSenseEntitiesTouching(std::move(toDetect), std::make_shared<NeuralNetwork>(hiddenLayers, toDetect.size(), NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(toDetect.size(), outputCount), transform, Random::Number(0.0, 100.0), Random::Number(0.0, 1.0))
{
}

GeneSenseEntitiesTouching::GeneSenseEntitiesTouching(std::vector<SenseTraitsBase::TraitNormaliser>&& traitWeights, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , network_(network)
    , outputConnections_(outputConnections)
    , transform_(transform)
    , toDetect_(std::move(traitWeights))
{
}

std::shared_ptr<Gene> GeneSenseEntitiesTouching::Mutate() const
{
    std::shared_ptr<NeuralNetwork> network = network_;
    std::shared_ptr<NeuralNetworkConnector> outputConnections = outputConnections_;
    Transform transform = transform_;
    std::vector<SenseTraitsBase::TraitNormaliser> toDetect = toDetect_;

    switch(Random::Number(0, 3)) {
    case 0 :
        network = network->Mutated();
        break;
    case 1 :
        outputConnections = outputConnections->Mutated();
        break;
    case 2 :
        transform = {
            transform.x + Random::Gaussian(0.0, 0.05 + transform.x / 10),
            transform.y + Random::Gaussian(0.0, 0.05 + transform.y / 10),
            transform.rotation,
        };
        break;
    case 3 :
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
    }

    return std::make_shared<GeneSenseEntitiesTouching>(std::move(toDetect),
                                                       network,
                                                       outputConnections,
                                                       transform,
                                                       GetMutatedDominance(),
                                                       GetMutatedMutationProbability());
}

void GeneSenseEntitiesTouching::ExpressGene(const Swimmer& owner, Phenotype& target) const
{
    auto traitWeights = toDetect_;
    target.senses.push_back(std::make_shared<SenseEntitiesTouching>(network_, outputConnections_, owner, transform_, std::move(traitWeights)));
}

#include "GeneSenseEntityRaycast.h"

#include "Random.h"
#include "Swimmer.h"
#include "Sensors/SenseEntityRaycast.h"

GeneSenseEntityRaycast::GeneSenseEntityRaycast(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform)
    : GeneSenseEntityRaycast(std::move(toDetect), std::make_shared<NeuralNetwork>(hiddenLayers, toDetect.size(), NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(toDetect.size(), outputCount), transform, Random::Number(0.0, 100.0), Random::Number(0.0, 1.0))
{
}

GeneSenseEntityRaycast::GeneSenseEntityRaycast(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , network_(network)
    , outputConnections_(outputConnections)
    , transform_(transform)
    , toDetect_(std::move(toDetect))
{
}

std::shared_ptr<Gene> GeneSenseEntityRaycast::Mutate() const
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
            Random::GaussianAdjustment<double>(transform.x, 0.1),
            Random::GaussianAdjustment<double>(transform.y, 0.1),
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

    return std::make_shared<GeneSenseEntityRaycast>(std::move(toDetect),
                                                    network,
                                                    outputConnections,
                                                    transform,
                                                    GetMutatedDominance(),
                                                    GetMutatedMutationProbability());
}

void GeneSenseEntityRaycast::ExpressGene(const Swimmer& owner, Phenotype& target) const
{
    double distance = std::sqrt(GetDistanceSquare({ 0, 0 }, { transform_.x, transform_.y }));
    double angle = GetBearing({ 0, 0 }, { transform_.x, transform_.y });
    auto toDetect = toDetect_;
    target.senses.push_back(std::make_shared<SenseEntityRaycast>(network_, outputConnections_, owner, std::move(toDetect), distance, angle));
}

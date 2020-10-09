#include "GeneSenseEntitiesInArea.h"

#include "Phenotype.h"
#include "Sensors/SenseEntitiesInArea.h"

GeneSenseEntitiesInArea::GeneSenseEntitiesInArea(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform, double radius)
    : GeneSenseEntitiesInArea(std::move(toDetect), std::make_shared<NeuralNetwork>(hiddenLayers, toDetect.size(), NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(toDetect.size(), outputCount), transform, radius, Random::Number(0.0, 100.0), Random::Number(0.0, 1.0))
{
}

GeneSenseEntitiesInArea::GeneSenseEntitiesInArea(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double radius, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , network_(network)
    , outputConnections_(outputConnections)
    , radius_(radius)
    , transform_(transform)
    , toDetect_(std::move(toDetect))
{
}

std::shared_ptr<Gene> GeneSenseEntitiesInArea::Mutate() const
{
    std::shared_ptr<NeuralNetwork> network = network_;
    std::shared_ptr<NeuralNetworkConnector> outputConnections = outputConnections_;
    Transform transform = transform_;
    double radius = radius_;
    std::vector<SenseTraitsBase::TraitNormaliser> toDetect = toDetect_;

    switch(Random::Number(0, 4)) {
    case 0 :
        network = network->Mutated();
        break;
    case 1 :
        outputConnections = outputConnections->Mutated();
        break;
    case 2 :
        radius += Random::Gaussian(0.0, 0.05 + radius / 10);
        break;
    case 3 :
        transform = {
            transform_.x + Random::Gaussian(0.0, 0.05 + transform.x / 10),
            transform_.y + Random::Gaussian(0.0, 0.05 + transform.y / 10),
            transform_.rotation,
        };
        break;
    case 4 :
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
    }

    return std::make_shared<GeneSenseEntitiesInArea>(std::move(toDetect),
                                                     network,
                                                     outputConnections,
                                                     transform,
                                                     radius,
                                                     GetMutatedDominance(),
                                                     GetMutatedMutationProbability());
}

void GeneSenseEntitiesInArea::ExpressGene(const Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseEntitiesInArea>(network_, outputConnections_, owner, radius_, transform_, std::vector<SenseTraitsBase::TraitNormaliser>(toDetect_)));
}

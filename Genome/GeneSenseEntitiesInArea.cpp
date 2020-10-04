#include "GeneSenseEntitiesInArea.h"

#include "Phenotype.h"
#include "Sensors/SenseEntitiesInArea.h"

GeneSenseEntitiesInArea::GeneSenseEntitiesInArea(std::vector<std::pair<double, Trait>>&& traitWeights, unsigned hiddenLayers, unsigned outputCount, const Transform& transform, double radius, double genericWeight)
    : GeneSenseEntitiesInArea(std::move(traitWeights), std::make_shared<NeuralNetwork>(hiddenLayers, traitWeights.size() + 1, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(traitWeights.size() + 1, outputCount), transform, radius, genericWeight, Random::Number(0.0, 100.0), Random::Number(0.0, 1.0))
{
}

GeneSenseEntitiesInArea::GeneSenseEntitiesInArea(std::vector<std::pair<double, Trait> >&& traitWeights, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double radius, double genericWeight, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , network_(network)
    , outputConnections_(outputConnections)
    , radius_(radius)
    , transform_(transform)
    , genericWeight_(genericWeight)
    , traitWeights_(std::move(traitWeights))
{
}

std::shared_ptr<Gene> GeneSenseEntitiesInArea::Mutate() const
{
    std::shared_ptr<NeuralNetwork> network = network_;
    std::shared_ptr<NeuralNetworkConnector> outputConnections = outputConnections_;
    Transform transform = transform_;
    double radius = radius_;
    double genericWeight = genericWeight_;
    std::vector<std::pair<double, Trait>> traitWeights = traitWeights_;

    switch(Random::Number(0, 5)) {
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
        genericWeight += Random::Gaussian(0.0, 0.05 + genericWeight / 10);
        break;
    case 5 :
        switch(Random::Number(0, 3)) {
        case 0 :
            // Adjust trait strength
            Random::Item(traitWeights).first += Random::Gaussian(0.0, 0.05 + Random::Item(traitWeights).first / 10);
            break;
        case 1 :
            // Switch trait
            Random::Item(traitWeights).second = Random::Item(ALL_TRAITS);
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

    return std::make_shared<GeneSenseEntitiesInArea>(std::move(traitWeights),
                                                     network,
                                                     outputConnections,
                                                     transform,
                                                     radius,
                                                     genericWeight,
                                                     GetMutatedDominance(),
                                                     GetMutatedMutationProbability());
}

void GeneSenseEntitiesInArea::ExpressGene(const Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseEntitiesInArea>(network_, outputConnections_, owner, radius_, transform_, genericWeight_, std::vector<std::pair<double, Trait> >(traitWeights_)));
}

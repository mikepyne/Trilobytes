#include "GeneSenseEntitiesInArea.h"

#include "Phenotype.h"
#include "Sensors/SenseEntitiesInArea.h"

GeneSenseEntitiesInArea::GeneSenseEntitiesInArea(std::vector<std::pair<double, Trait>>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform, double radius, double distanceWeight)
    : GeneSenseEntitiesInArea(std::move(toDetect), std::make_shared<NeuralNetwork>(hiddenLayers, toDetect.size() + 1, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(toDetect.size() + 1, outputCount), transform, radius, distanceWeight, Random::Number(0.0, 100.0), Random::Number(0.0, 1.0))
{
}

GeneSenseEntitiesInArea::GeneSenseEntitiesInArea(std::vector<std::pair<double, Trait> >&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double radius, double distanceWeight, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , network_(network)
    , outputConnections_(outputConnections)
    , radius_(radius)
    , transform_(transform)
    , distanceWeight_(distanceWeight)
    , toDetect_(std::move(toDetect))
{
}

std::shared_ptr<Gene> GeneSenseEntitiesInArea::Mutate() const
{
    double mutatedRadius = radius_;
    Transform mutatedTransform = transform_;
    double mutatedDistanceWeight = distanceWeight_;
    std::vector<std::pair<double, Trait>> mutatedToDetect = toDetect_;

    mutatedRadius += Random::Gaussian(0.0, 0.5);
    mutatedTransform.y += Random::Gaussian(0.0, 0.5);
    mutatedTransform.rotation += Random::Gaussian(0.0, 0.01);
    mutatedDistanceWeight += Random::Gaussian(0.0, 0.5);
    // mutatedToDetect; TODO

    return std::make_shared<GeneSenseEntitiesInArea>(std::move(mutatedToDetect),
                                                     network_->Mutated(),
                                                     outputConnections_->Mutated(),
                                                     mutatedTransform,
                                                     mutatedRadius,
                                                     mutatedDistanceWeight,
                                                     GetMutatedDominance(),
                                                     GetMutatedMutationProbability());
}

void GeneSenseEntitiesInArea::ExpressGene(const Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseEntitiesInArea>(network_, outputConnections_, owner, radius_, transform_, distanceWeight_, std::vector<std::pair<double, Trait> >(toDetect_)));
}

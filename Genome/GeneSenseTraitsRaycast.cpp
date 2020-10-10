#include "GeneSenseTraitsRaycast.h"

#include "Phenotype.h"
#include "Sensors/SenseTraitsRaycast.h"

GeneSenseTraitsRaycast::GeneSenseTraitsRaycast(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform)
    : GeneSenseTraitsBase(std::move(toDetect), hiddenLayers, outputCount, transform)
{
}

GeneSenseTraitsRaycast::GeneSenseTraitsRaycast(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance, double mutationProbability)
    : GeneSenseTraitsBase(std::move(toDetect), network, outputConnections, transform, dominance, mutationProbability)
{
}

std::shared_ptr<Gene> GeneSenseTraitsRaycast::Mutate() const
{
    auto [ toDetect, network, outputs, transform, dominance, mutationProbability ] = GetMutated({});

    return std::make_shared<GeneSenseTraitsRaycast>(std::move(toDetect),
                                                    network,
                                                    outputs,
                                                    transform,
                                                    dominance,
                                                    mutationProbability);
}

void GeneSenseTraitsRaycast::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    double distance = std::sqrt(GetDistanceSquare({ 0, 0 }, { transform_.x, transform_.y }));
    double angle = GetBearing({ 0, 0 }, { transform_.x, transform_.y });
    auto toDetect = toDetect_;
    target.senses.push_back(std::make_shared<SenseTraitsRaycast>(network_, outputs_, owner, std::move(toDetect), distance, angle));
}

#include "GeneSenseTraitsRaycast.h"

#include "Phenotype.h"
#include "Sensors/SenseTraitsRaycast.h"

using namespace nlohmann;

GeneSenseTraitsRaycast::GeneSenseTraitsRaycast(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform, const double& distance, const double& rotation)
    : GeneSenseTraitsBase(std::move(toDetect), hiddenLayers, outputCount, transform)
    , distance_(distance)
    , rotation_(rotation)
{
    AddMutations();
}

GeneSenseTraitsRaycast::GeneSenseTraitsRaycast(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, const double& distance, const double& rotation, double dominance)
    : GeneSenseTraitsBase(std::move(toDetect), network, outputConnections, transform, dominance)
    , distance_(distance)
    , rotation_(rotation)
{
    AddMutations();
}

json GeneSenseTraitsRaycast::Serialise() const
{
    return {
        {KEY_DOMINANCE, GetDominance()},
        {KEY_NETWORK, NeuralNetwork::Serialise(GetNetwork()) },
        {KEY_OUTPUT_CONNECTIONS, NeuralNetworkConnector::Serialise(GetOutputConnections()) },
        {KEY_TRANSFORM, Transform::Serialise(transform_)},
        {KEY_TRAIT_NORMALISERS, GetSerialisedTraitNormalisers()},
        {KEY_DISTANCE, distance_},
        {KEY_ROTATION, rotation_},
    };
}

void GeneSenseTraitsRaycast::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseTraitsRaycast>(GetNetwork(), GetOutputConnections(), owner, std::vector(toDetect_), transform_, distance_, rotation_));
    target.baseMetabolism += GetMetabolicCost();
}

std::shared_ptr<Gene> GeneSenseTraitsRaycast::Copy() const
{
    return std::make_shared<GeneSenseTraitsRaycast>(std::vector(toDetect_), GetNetwork(),GetOutputConnections(), transform_, distance_, rotation_, GetDominance());
}

void GeneSenseTraitsRaycast::AddMutations()
{
    AddMutation(BASE_WEIGHT, [&]()
    {
        distance_ += Random::Gaussian(0.0, 5.0);
    });
    AddMutation(BASE_WEIGHT, [&]()
    {
        rotation_ += Random::Gaussian(0.0, Tril::Tau / 50);
    });
}

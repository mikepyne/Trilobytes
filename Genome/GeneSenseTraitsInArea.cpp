#include "GeneSenseTraitsInArea.h"

#include "Phenotype.h"
#include "Sensors/SenseTraitsInArea.h"

GeneSenseTraitsInArea::GeneSenseTraitsInArea(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform, double radius)
    : GeneSenseTraitsBase(std::move(toDetect), hiddenLayers, outputCount, transform)
    , radius_(radius)
{
    AddMutations();
}

GeneSenseTraitsInArea::GeneSenseTraitsInArea(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double radius, double dominance, double mutationProbability)
    : GeneSenseTraitsBase(std::move(toDetect), network, outputConnections, transform, dominance, mutationProbability)
    , radius_(radius)
{
    AddMutations();
}

void GeneSenseTraitsInArea::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseTraitsInArea>(GetNetwork(), GetOutputConnections(), owner, radius_, transform_, std::vector(toDetect_)));
}

std::shared_ptr<Gene> GeneSenseTraitsInArea::Copy() const
{
    return std::make_shared<GeneSenseTraitsInArea>(std::vector(toDetect_), GetNetwork(), GetOutputConnections(), transform_, radius_, GetDominance(), GetMutationProbability());

}

void GeneSenseTraitsInArea::AddMutations()
{
    AddMutation(BASE_WEIGHT, [&]
    {
        radius_ = std::max(0.1, Random::GaussianAdjustment(radius_, 1.0));
    });
}

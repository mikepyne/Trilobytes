#include "GeneSenseTraitsRaycast.h"

#include "Phenotype.h"
#include "Sensors/SenseTraitsRaycast.h"

GeneSenseTraitsRaycast::GeneSenseTraitsRaycast(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform)
    : GeneSenseTraitsBase(std::move(toDetect), hiddenLayers, outputCount, transform)
{
    AddMutations();
}

GeneSenseTraitsRaycast::GeneSenseTraitsRaycast(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance, double mutationProbability)
    : GeneSenseTraitsBase(std::move(toDetect), network, outputConnections, transform, dominance, mutationProbability)
{
    AddMutations();
}

void GeneSenseTraitsRaycast::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    double distance = GetDistance({ 0, 0 }, { transform_.x, transform_.y });
    double angle = GetBearing({ 0, 0 }, { transform_.x, transform_.y });
    target.senses.push_back(std::make_shared<SenseTraitsRaycast>(GetNetwork(), GetOutputConnections(), owner, std::vector(toDetect_), distance, angle));
}

std::shared_ptr<Gene> GeneSenseTraitsRaycast::Copy() const
{
    return std::make_shared<GeneSenseTraitsRaycast>(std::vector(toDetect_), GetNetwork(),GetOutputConnections(), transform_, GetDominance(), GetMutationProbability());
}

void GeneSenseTraitsRaycast::AddMutations()
{
    /*
     * FIXME currently the mutations for this class are all handles by the
     * mutate transform mutation in the SenseTraitsBase class, however this
     * gene ought to have some notion of a Line that it mutates seperately to
     * the transform.
     */
}

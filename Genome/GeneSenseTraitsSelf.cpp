#include "GeneSenseTraitsSelf.h"

#include "Phenotype.h"
#include "Sensors/SenseTraitsSelf.h"

GeneSenseTraitsSelf::GeneSenseTraitsSelf(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount)
    : GeneSenseTraitsBase(std::move(toDetect), hiddenLayers, outputCount, {})
{
    // No additional mutations
}

GeneSenseTraitsSelf::GeneSenseTraitsSelf(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, double dominance, double mutationProbability)
    : GeneSenseTraitsBase(std::move(toDetect), network, outputConnections, {}, dominance, mutationProbability)
{
    // No additional mutations
}

void GeneSenseTraitsSelf::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseTraitsSelf>(GetNetwork(), GetOutputConnections(), owner, std::vector(toDetect_)));
}

std::shared_ptr<Gene> GeneSenseTraitsSelf::Copy() const
{
    return std::make_shared<GeneSenseTraitsSelf>(std::vector(toDetect_), GetNetwork(), GetOutputConnections(), GetDominance(), GetMutationProbability());
}

#include "GeneSenseTraitsSelf.h"

#include "Phenotype.h"
#include "Sensors/SenseTraitsSelf.h"

GeneSenseTraitsSelf::GeneSenseTraitsSelf(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount)
    : GeneSenseTraitsBase(std::move(toDetect), hiddenLayers, outputCount, {})
{
}

GeneSenseTraitsSelf::GeneSenseTraitsSelf(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, double dominance, double mutationProbability)
    : GeneSenseTraitsBase(std::move(toDetect), network, outputConnections, {}, dominance, mutationProbability)
{
}

std::shared_ptr<Gene> GeneSenseTraitsSelf::Mutate() const
{
    auto [ toDetect, network, outputs, transform, dominance, mutationProbability ] = GetMutated({});
    (void) transform; // unused

    return std::make_shared<GeneSenseTraitsSelf>(std::move(toDetect),
                                                 network,
                                                 outputs,
                                                 dominance,
                                                 mutationProbability);
}

void GeneSenseTraitsSelf::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    auto toDetect = toDetect_;
    target.senses.push_back(std::make_shared<SenseTraitsSelf>(network_, outputs_, owner, std::move(toDetect)));
}

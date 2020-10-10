#include "GeneSenseTraitsTouching.h"

#include "Phenotype.h"
#include "Sensors/SenseTraitsTouching.h"

GeneSenseTraitsTouching::GeneSenseTraitsTouching(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform)
    : GeneSenseTraitsBase(std::move(toDetect), hiddenLayers, outputCount, transform)
{
}

GeneSenseTraitsTouching::GeneSenseTraitsTouching(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance, double mutationProbability)
    : GeneSenseTraitsBase(std::move(toDetect), network, outputConnections, transform, dominance, mutationProbability)
{
}

std::shared_ptr<Gene> GeneSenseTraitsTouching::Mutate() const
{
    auto [ toDetect, network, outputs, transform, dominance, mutationProbability ] = GetMutated({});

    return std::make_shared<GeneSenseTraitsTouching>(std::move(toDetect),
                                                       network,
                                                       outputs,
                                                       transform,
                                                       dominance,
                                                       mutationProbability);
}

void GeneSenseTraitsTouching::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    auto traitWeights = toDetect_;
    target.senses.push_back(std::make_shared<SenseTraitsTouching>(network_, outputs_, owner, transform_, std::move(traitWeights)));
}

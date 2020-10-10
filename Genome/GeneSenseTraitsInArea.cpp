#include "GeneSenseTraitsInArea.h"

#include "Phenotype.h"
#include "Sensors/SenseTraitsInArea.h"

GeneSenseTraitsInArea::GeneSenseTraitsInArea(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform, double radius)
    : GeneSenseTraitsBase(std::move(toDetect), hiddenLayers, outputCount, transform)
    , radius_(radius)
{
}

GeneSenseTraitsInArea::GeneSenseTraitsInArea(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double radius, double dominance, double mutationProbability)
    : GeneSenseTraitsBase(std::move(toDetect), network, outputConnections, transform, dominance, mutationProbability)
    , radius_(radius)
{
}

std::shared_ptr<Gene> GeneSenseTraitsInArea::Mutate() const
{
    double radius = radius_;
    auto [ toDetect, network, outputs, transform, dominance, mutationProbability ] = GetMutated({
        [&]()
        {
            radius = std::max(0.1, Random::GaussianAdjustment(radius, 1.0));
        }
    });

    return std::make_shared<GeneSenseTraitsInArea>(std::move(toDetect),
                                                   network,
                                                   outputs,
                                                   transform,
                                                   radius,
                                                   dominance,
                                                   mutationProbability);
}

void GeneSenseTraitsInArea::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseTraitsInArea>(network_, outputs_, owner, radius_, transform_, std::vector<SenseTraitsBase::TraitNormaliser>(toDetect_)));
}

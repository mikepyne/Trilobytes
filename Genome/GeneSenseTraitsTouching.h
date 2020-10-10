#ifndef GENESENSEENTITIESTOUCHING_H
#define GENESENSEENTITIESTOUCHING_H

#include "GeneSenseTraitsBase.h"

class GeneSenseTraitsTouching final : public GeneSenseTraitsBase {
public:
    GeneSenseTraitsTouching(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform);
    GeneSenseTraitsTouching(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance, double mutationProbability);
    virtual ~GeneSenseTraitsTouching() override {}

    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;
};

#endif // GENESENSEENTITIESTOUCHING_H

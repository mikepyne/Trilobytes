#ifndef GENESENSEENTITIESTOUCHING_H
#define GENESENSEENTITIESTOUCHING_H

#include "Gene.h"
#include "Entity.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"
#include "Sensors/SenseTraitsBase.h"

#include <vector>
#include <memory>

class GeneSenseEntitiesTouching final : public Gene {
public:
    GeneSenseEntitiesTouching(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform);
    GeneSenseEntitiesTouching(std::vector<SenseTraitsBase::TraitNormaliser>&& traitWeights, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance, double mutationProbability);
    virtual ~GeneSenseEntitiesTouching() override {}

    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(const Swimmer& owner, Phenotype& target) const override;

private:
    const std::shared_ptr<NeuralNetwork> network_;
    const std::shared_ptr<NeuralNetworkConnector> outputConnections_;

    const Transform transform_;
    const std::vector<SenseTraitsBase::TraitNormaliser> toDetect_;
};

#endif // GENESENSEENTITIESTOUCHING_H

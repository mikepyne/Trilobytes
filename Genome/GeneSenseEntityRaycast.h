#ifndef GENESENSEENTITYRAYCAST_H
#define GENESENSEENTITYRAYCAST_H

#include "Gene.h"
#include "Entity.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"
#include "Sensors/SenseTraitsBase.h"

#include <vector>
#include <memory>

class GeneSenseEntityRaycast : public Gene {
public:
    GeneSenseEntityRaycast(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform);
    GeneSenseEntityRaycast(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance, double mutationProbability);
    virtual ~GeneSenseEntityRaycast() override final {}

    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(const Swimmer& owner, Phenotype& target) const override;

private:
    const std::shared_ptr<NeuralNetwork> network_;
    const std::shared_ptr<NeuralNetworkConnector> outputConnections_;

    const Transform transform_;
    const std::vector<SenseTraitsBase::TraitNormaliser> toDetect_;
};

#endif // GENESENSEENTITYRAYCAST_H

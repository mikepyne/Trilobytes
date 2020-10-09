#ifndef GENESENSEENTITIESINAREA_H
#define GENESENSEENTITIESINAREA_H

#include "Gene.h"
#include "Entity.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"
#include "Sensors/SenseTraitsBase.h"

#include <memory>

class GeneSenseEntitiesInArea final : public Gene {
public:
    GeneSenseEntitiesInArea(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform, double radius);
    GeneSenseEntitiesInArea(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double radius, double dominance, double mutationProbability);
    virtual ~GeneSenseEntitiesInArea() override {}

    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(const Swimmer& owner, Phenotype& target) const override;

private:
    const std::shared_ptr<NeuralNetwork> network_;
    const std::shared_ptr<NeuralNetworkConnector> outputConnections_;

    const double radius_;
    const Transform transform_;
    const std::vector<SenseTraitsBase::TraitNormaliser> toDetect_;
};

#endif // GENESENSEENTITIESINAREA_H

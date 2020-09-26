#ifndef GENESENSEENTITIESINAREA_H
#define GENESENSEENTITIESINAREA_H

#include "Gene.h"
#include "Entity.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"

#include <memory>

class GeneSenseEntitiesInArea : public Gene {
public:
    GeneSenseEntitiesInArea(std::vector<std::pair<double, Trait>>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform, double radius, double distanceWeight);
    GeneSenseEntitiesInArea(std::vector<std::pair<double, Trait>>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double radius, double distanceWeight, double dominance, double mutationProbability);

    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(const Swimmer& owner, Phenotype& target) const override;

private:
    const std::shared_ptr<NeuralNetwork> network_;
    const std::shared_ptr<NeuralNetworkConnector> outputConnections_;

    const double radius_;
    const Transform transform_;
    const double distanceWeight_;
    const std::vector<std::pair<double, Trait>> toDetect_;

};

#endif // GENESENSEENTITIESINAREA_H

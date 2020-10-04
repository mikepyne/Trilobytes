#ifndef GENESENSEENTITYRAYCAST_H
#define GENESENSEENTITYRAYCAST_H

#include "Gene.h"
#include "Entity.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"

#include <vector>
#include <memory>

class GeneSenseEntityRaycast : public Gene {
public:
    GeneSenseEntityRaycast(std::vector<std::pair<double, Trait>>&& traitWeights, unsigned hiddenLayers, unsigned outputCount, const Transform& transform, double genericWeight);
    GeneSenseEntityRaycast(std::vector<std::pair<double, Trait>>&& traitWeights, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double genericWeight, double dominance, double mutationProbability);

    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(const Swimmer& owner, Phenotype& target) const override;

private:
    const std::shared_ptr<NeuralNetwork> network_;
    const std::shared_ptr<NeuralNetworkConnector> outputConnections_;

    const Transform transform_;
    const double genericWeight_;
    const std::vector<std::pair<double, Trait>> traitWeights_;
};

#endif // GENESENSEENTITYRAYCAST_H

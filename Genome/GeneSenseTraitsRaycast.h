#ifndef GENESENSEENTITYRAYCAST_H
#define GENESENSEENTITYRAYCAST_H

#include "GeneSenseTraitsBase.h"

class GeneSenseTraitsRaycast : public GeneSenseTraitsBase {
public:
    GeneSenseTraitsRaycast(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform, const double& distance, const double& rotation);
    GeneSenseTraitsRaycast(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, const double& distance, const double& rotation, double dominance, double mutationProbability);
    virtual ~GeneSenseTraitsRaycast() override {}

    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;

    void AddMutations();

private:
     double distance_;
     double rotation_;
};

#endif // GENESENSEENTITYRAYCAST_H

#ifndef GENESENSEENTITIESINAREA_H
#define GENESENSEENTITIESINAREA_H

#include "GeneSenseTraitsBase.h"

class GeneSenseTraitsInArea final : public GeneSenseTraitsBase {
public:
    GeneSenseTraitsInArea(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform, double radius);
    GeneSenseTraitsInArea(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double radius, double dominance);
    virtual ~GeneSenseTraitsInArea() override {}

    virtual std::string Name() const override { return "GeneSenseTraitsInArea"; }
    virtual nlohmann::json Serialise() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;

private:
    static const inline std::string KEY_RADIUS = "Radius";

    double radius_;

    void AddMutations();
};

#endif // GENESENSEENTITIESINAREA_H

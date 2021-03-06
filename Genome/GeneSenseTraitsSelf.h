#ifndef GENESENSETRAITSSELF_H
#define GENESENSETRAITSSELF_H

#include "GeneSenseTraitsBase.h"

class GeneSenseTraitsSelf : public GeneSenseTraitsBase {
public:
    GeneSenseTraitsSelf(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount);
    GeneSenseTraitsSelf(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, double dominance);
    virtual ~GeneSenseTraitsSelf() {}

    virtual std::string Name() const override { return "GeneSenseTraitsSelf"; }
    virtual nlohmann::json Serialise() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;
};

#endif // GENESENSETRAITSSELF_H

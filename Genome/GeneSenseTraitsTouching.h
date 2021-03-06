#ifndef GENESENSEENTITIESTOUCHING_H
#define GENESENSEENTITIESTOUCHING_H

#include "GeneSenseTraitsBase.h"
#include "Utils.h"

class GeneSenseTraitsTouching final : public GeneSenseTraitsBase {
public:
    GeneSenseTraitsTouching(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform);
    GeneSenseTraitsTouching(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance);
    virtual ~GeneSenseTraitsTouching() override {}

    virtual std::string Name() const override { return std::string(Tril::TypeName<GeneSenseTraitsTouching>()); }
    virtual nlohmann::json Serialise() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;

private:
    void AddMutations();
};

#endif // GENESENSEENTITIESTOUCHING_H

#ifndef GENEEFFECTORTAIL_H
#define GENEEFFECTORTAIL_H

#include "GeneEffectorBase.h"

#include <memory>

class GeneEffectorTail : public GeneEffectorBase {
public:
    GeneEffectorTail(unsigned hiddenLayers, unsigned inputCount);
    GeneEffectorTail(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance);
    virtual ~GeneEffectorTail() {}

    virtual std::string Name() const override { return "GeneEffectorTail"; }
    virtual nlohmann::json Serialise() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;
};

#endif // GENEEFFECTORTAIL_H

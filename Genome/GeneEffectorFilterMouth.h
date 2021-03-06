#ifndef GENEEFFECTORFILTERMOUTH_H
#define GENEEFFECTORFILTERMOUTH_H

#include "GeneEffectorBase.h"

class GeneEffectorFilterMouth : public GeneEffectorBase {
public:
    GeneEffectorFilterMouth(unsigned hiddenLayers, unsigned inputCount);
    GeneEffectorFilterMouth(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance);
    virtual ~GeneEffectorFilterMouth();

    virtual std::string Name() const override { return "GeneEffectorFilterMouth"; }
    virtual nlohmann::json Serialise() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;
};

#endif // GENEEFFECTORFILTERMOUTH_H

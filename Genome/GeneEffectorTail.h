#ifndef GENEEFFECTORTAIL_H
#define GENEEFFECTORTAIL_H

#include "GeneEffectorBase.h"

#include <memory>

class GeneEffectorTail : public GeneEffectorBase {
public:
    GeneEffectorTail(unsigned hiddenLayers, unsigned inputCount);
    GeneEffectorTail(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance, double mutationProbability);
    virtual ~GeneEffectorTail() {}

    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;
};

#endif // GENEEFFECTORTAIL_H

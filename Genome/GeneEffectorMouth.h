#ifndef GENEEFFECTORMOUTH_H
#define GENEEFFECTORMOUTH_H

#include "GeneEffectorBase.h"
#include "Transform.h"

class GeneEffectorMouth : public GeneEffectorBase {
public:
    GeneEffectorMouth(unsigned hiddenLayers, unsigned inputCount, Transform transform);
    GeneEffectorMouth(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, Transform transform, double dominance, double mutationProbability);

    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

private:
    Transform transform_;
};

#endif // GENEEFFECTORMOUTH_H

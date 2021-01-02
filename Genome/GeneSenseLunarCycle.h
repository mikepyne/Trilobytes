#ifndef GENESENSELUNARCYCLE_H
#define GENESENSELUNARCYCLE_H

#include "GeneSenseBase.h"

class GeneSenseLunarCycle : public GeneSenseBase {
public:
    GeneSenseLunarCycle(unsigned hiddenLayers, unsigned outputCount);
    GeneSenseLunarCycle(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, double dominance);

    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;
};

#endif // GENESENSELUNARCYCLE_H

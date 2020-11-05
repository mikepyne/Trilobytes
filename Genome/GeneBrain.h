#ifndef GENEBRAIN_H
#define GENEBRAIN_H

#include "Gene.h"
#include "NeuralNetwork.h"

#include <memory>

class GeneBrain : public Gene {
public:
    GeneBrain(unsigned layerCount, unsigned width, double mutationProbability);
    GeneBrain(const std::shared_ptr<NeuralNetwork>& brain, double dominance, double mutationProbability);

    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;

private:
    std::shared_ptr<NeuralNetwork> brain_;
};

#endif // GENEBRAIN_H

#ifndef GENEBRAIN_H
#define GENEBRAIN_H

#include "Gene.h"
#include "NeuralNetwork.h"

#include <memory>

class GeneBrain : public Gene {
public:
    GeneBrain(unsigned layerCount, unsigned width, double mutationProbability);
    GeneBrain(const std::shared_ptr<NeuralNetwork>& brain, double dominance, double mutationProbability);

    virtual std::shared_ptr<Gene> Mutate() const override;

    virtual void ExpressGene(const Swimmer& owner, Phenotype& target) const override;

private:
    std::shared_ptr<NeuralNetwork> brain_;
};

#endif // GENEBRAIN_H

#ifndef GENESENSEMAGNETICFIELD_H
#define GENESENSEMAGNETICFIELD_H

#include "Gene.h"

#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"

class GeneSenseMagneticField : public Gene {
public:
    GeneSenseMagneticField(unsigned outputCount);
    GeneSenseMagneticField(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, double dominance, double mutationProbability);

    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(const Swimmer& owner, Phenotype& target) const override;

private:
    std::shared_ptr<NeuralNetwork> network_;
    std::shared_ptr<NeuralNetworkConnector> outputConnections_;
};

#endif // GENESENSEMAGNETICFIELD_H

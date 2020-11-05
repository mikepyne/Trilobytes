#ifndef GENESENSEMAGNETICFIELD_H
#define GENESENSEMAGNETICFIELD_H

#include "GeneSenseBase.h"

class GeneSenseMagneticField : public GeneSenseBase {
public:
    GeneSenseMagneticField(unsigned outputCount);
    GeneSenseMagneticField(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, double dominance, double mutationProbability);
    virtual ~GeneSenseMagneticField() override {}

    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;
};

#endif // GENESENSEMAGNETICFIELD_H

#ifndef GENEEFFECTORSPIKE_H
#define GENEEFFECTORSPIKE_H

#include "GeneEffectorBase.h"

class GeneEffectorSpike : public GeneEffectorBase {
public:
    GeneEffectorSpike(unsigned inputCount, double bearing, double length);
    GeneEffectorSpike(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance, double bearing, double length);
    ~GeneEffectorSpike() override;

    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;

private:
    double bearing_;
    double length_;

    void AddMutations();
};

#endif // GENEEFFECTORSPIKE_H

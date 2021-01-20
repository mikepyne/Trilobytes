#ifndef GENEEFFECTORSPRINGTAIL_H
#define GENEEFFECTORSPRINGTAIL_H

#include "GeneEffectorBase.h"

class GeneEffectorSpringTail : public GeneEffectorBase {
public:
    GeneEffectorSpringTail(unsigned hiddenLayers, unsigned inputCount, const EoBE::RangeConverter& neuronExcitementToChargeRate, const Energy& storedEnergyCap, const double& triggerThreshold);
    GeneEffectorSpringTail(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance, const EoBE::RangeConverter& neuronExcitementToChargeRate, const Energy& storedEnergyCap, const double& triggerThreshold);
    ~GeneEffectorSpringTail() override;

    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;

private:
    EoBE::RangeConverter neuronExcitementToChargeRate_;
    Energy storedEnergyCap_;
    double triggerThreshold_;

    void AddMutations();
};

#endif // GENEEFFECTORSPRINGTAIL_H

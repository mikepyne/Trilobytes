#ifndef GENEEFFECTORSPRINGTAIL_H
#define GENEEFFECTORSPRINGTAIL_H

#include "Utility/RangeConverter.h"

#include "GeneEffectorBase.h"

class GeneEffectorSpringTail : public GeneEffectorBase {
public:
    GeneEffectorSpringTail(unsigned hiddenLayers, unsigned inputCount, const Tril::RangeConverter& neuronExcitementToChargeRate, const Energy& storedEnergyCap, const double& triggerThreshold);
    GeneEffectorSpringTail(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance, const Tril::RangeConverter& neuronExcitementToChargeRate, const Energy& storedEnergyCap, const double& triggerThreshold);
    ~GeneEffectorSpringTail() override;

    virtual std::string Name() const override { return "GeneEffectorSpringTail"; }
    virtual nlohmann::json Serialise() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;

private:
    static const inline std::string KEY_CHARGE_RATE = "ChargeRate";
    static const inline std::string KEY_ENERGY_CAP = "EnergyCap";
    static const inline std::string KEY_TRIGGER = "Trigger";

    Tril::RangeConverter neuronExcitementToChargeRate_;
    Energy storedEnergyCap_;
    double triggerThreshold_;

    void AddMutations();
};

#endif // GENEEFFECTORSPRINGTAIL_H

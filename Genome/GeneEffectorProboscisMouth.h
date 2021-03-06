#ifndef GENEEFFECTORPROBOSCISMOUTH_H
#define GENEEFFECTORPROBOSCISMOUTH_H

#include "GeneEffectorBase.h"

class GeneEffectorProboscisMouth : public GeneEffectorBase {
public:
    GeneEffectorProboscisMouth(unsigned inputCount, double proboscisLength);
    GeneEffectorProboscisMouth(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance, double proboscisLength);
    ~GeneEffectorProboscisMouth() override;

    virtual std::string Name() const override { return "GeneEffectorProboscisMouth"; }
    virtual nlohmann::json Serialise() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;

private:
    static const inline std::string KEY_LENGTH = "Length";

    constexpr static double minProboscisLength_ = 1.0;
    double proboscisLength_;

    void AddMutations();
};

#endif // GENEEFFECTORPROBOSCISMOUTH_H

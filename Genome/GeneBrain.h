#ifndef GENEBRAIN_H
#define GENEBRAIN_H

#include "Gene.h"
#include "NeuralNetwork.h"

#include <memory>

class GeneBrain : public Gene {
public:
    GeneBrain(unsigned layerCount, unsigned width);
    GeneBrain(const std::shared_ptr<NeuralNetwork>& brain, double dominance);

    virtual std::string Name() const override { return "GeneBrain"; }
    virtual nlohmann::json Serialise() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;

private:
    static const inline std::string KEY_BRAIN = "Brain";

    std::shared_ptr<NeuralNetwork> brain_;
};

#endif // GENEBRAIN_H

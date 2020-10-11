#ifndef GENESENSERANDOM_H
#define GENESENSERANDOM_H

#include "Gene.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"
#include "Sensors/SenseRandom.h"

class GeneSenseRandom : public Gene {
public:
    GeneSenseRandom(unsigned inputCount, unsigned outputCount);
    GeneSenseRandom(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, std::vector<SenseRandom::FilteredRandom>&& filteredRandoms, double dominance, double mutationProbability);
    virtual ~GeneSenseRandom() override final {}

    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

private:
    std::shared_ptr<NeuralNetwork> network_;
    std::shared_ptr<NeuralNetworkConnector> outputConnections_;

    std::vector<SenseRandom::FilteredRandom> filteredRandoms_;

    static std::vector<SenseRandom::FilteredRandom> CreateRandomFilteredRandoms(unsigned count);
};

#endif // GENESENSERANDOM_H

#ifndef GENESINE_H
#define GENESINE_H

#include "Gene.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"
#include "Sensors/SenseSine.h"

class GeneSenseSine : public Gene {
public:
    GeneSenseSine(unsigned inputCount, unsigned outputCount);
    GeneSenseSine(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, std::vector<SenseSine::SineWave>&& sineWaves, double dominance, double mutationProbability);
    virtual ~GeneSenseSine() override {}

    virtual std::shared_ptr<Gene> Mutate() const override;
    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

private:
    std::shared_ptr<NeuralNetwork> network_;
    std::shared_ptr<NeuralNetworkConnector> outputConnections_;
    std::vector<SenseSine::SineWave> sineWaves_;

    static std::vector<SenseSine::SineWave> CreateRandomWaves(unsigned count);
};

#endif // GENESINE_H

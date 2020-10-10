#ifndef GENEEFFECTORBASE_H
#define GENEEFFECTORBASE_H

#include "Gene.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"

#include <memory>

class GeneEffectorBase : public Gene {
public:
    GeneEffectorBase(unsigned hiddenLayers, unsigned inputCount, unsigned outputCount);
    GeneEffectorBase(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance, double mutationProbability);
    virtual ~GeneEffectorBase() {}

protected:
    std::shared_ptr<NeuralNetwork> network_;
    std::shared_ptr<NeuralNetworkConnector> inputs_;

    std::tuple<std::shared_ptr<NeuralNetwork>, std::shared_ptr<NeuralNetworkConnector>, double, double> GetMutated(std::vector<std::function<void()>> additionalMutations) const;

};

#endif // GENEEFFECTORBASE_H

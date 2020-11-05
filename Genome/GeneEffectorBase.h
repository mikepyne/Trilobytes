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
    void AddColumnInsertedMutation(double mutationWeight, std::function<void(unsigned index)>&& onColumnAdded);
    void AddColumnRemovedMutation(double mutationWeight, std::function<void(unsigned index)>&& onColumnRemoved);

    const std::shared_ptr<NeuralNetwork> GetNetwork() const { return network_; }
    const std::shared_ptr<NeuralNetworkConnector> GetInputConnections() const { return inputConnections_; }

private:
    std::shared_ptr<NeuralNetwork> network_;
    std::shared_ptr<NeuralNetworkConnector> inputConnections_;
};

#endif // GENEEFFECTORBASE_H

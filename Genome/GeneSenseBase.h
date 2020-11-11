#ifndef GENESENSEBASE_H
#define GENESENSEBASE_H

#include "Gene.h"
#include "Energy.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"

#include <memory>

class GeneSenseBase : public Gene {
public:
    GeneSenseBase(unsigned inputCount, unsigned hiddenLayers, unsigned outputCount);
    GeneSenseBase(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, double dominance, double mutationProbability);
    virtual ~GeneSenseBase() override {}

protected:
    Energy GetMetabolicCost() const;

    void AddColumnInsertedMutation(double mutationWeight, std::function<void(unsigned index)>&& onColumnAdded);
    void AddColumnRemovedMutation(double mutationWeight, std::function<void(unsigned index)>&& onColumnRemoved);

    const std::shared_ptr<NeuralNetwork> GetNetwork() const { return network_; }
    const std::shared_ptr<NeuralNetworkConnector> GetOutputConnections() const { return outputConnections_; }

private:
    std::shared_ptr<NeuralNetwork> network_;
    std::shared_ptr<NeuralNetworkConnector> outputConnections_;
};

#endif // GENESENSEBASE_H

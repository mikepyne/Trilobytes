#ifndef GENEEFFECTORBASE_H
#define GENEEFFECTORBASE_H

#include "Gene.h"
#include "Utility/Energy.h"
#include "Utility/NeuralNetwork.h"
#include "Utility/NeuralNetworkConnector.h"

#include <memory>

class GeneEffectorBase : public Gene {
public:
    GeneEffectorBase(unsigned hiddenLayers, unsigned inputCount, unsigned outputCount);
    GeneEffectorBase(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance);
    virtual ~GeneEffectorBase() {}

protected:
    static const inline std::string KEY_NETWORK = "Network";
    static const inline std::string KEY_INPUT_CONNECTIONS = "Inputs";

    Energy GetMetabolicCost() const;

    void AddColumnInsertedAndRemovedMutations(double addedMutationWeight, std::function<void(unsigned index)>&& onColumnAdded, double removedMutationWeight, std::function<void(unsigned index)>&& onColumnRemoved);

    const std::shared_ptr<NeuralNetwork> GetNetwork() const { return network_; }
    const std::shared_ptr<NeuralNetworkConnector> GetInputConnections() const { return inputConnections_; }

private:
    std::shared_ptr<NeuralNetwork> network_;
    std::shared_ptr<NeuralNetworkConnector> inputConnections_;
};

#endif // GENEEFFECTORBASE_H

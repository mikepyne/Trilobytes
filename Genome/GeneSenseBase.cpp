#include "GeneSenseBase.h"

GeneSenseBase::GeneSenseBase(unsigned hiddenLayers, unsigned inputCount, unsigned outputCount)
    : GeneSenseBase(std::make_shared<NeuralNetwork>(hiddenLayers, inputCount, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(inputCount, outputCount), Random::Number(0.0, 100.0))
{
}

GeneSenseBase::GeneSenseBase(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, double dominance)
    : Gene(dominance)
    , network_(network)
    , outputConnections_(outputConnections)
{
    // Insert a new row
    AddMutation(0.15 * BASE_WEIGHT, [&]()
    {
        unsigned newRowIndex = Random::Number(size_t{ 0 }, network_->GetLayerCount());
        network_ = network_->WithRowAdded(newRowIndex, NeuralNetwork::InitialWeights::PassThrough);
    });

    // Remove a row
    AddMutation(0.30 * BASE_WEIGHT, [&]()
    {
        // Don't allow mutation to remove final row
        if (network_->GetLayerCount() > 1) {
            unsigned newRowIndex = Random::Number(size_t{ 0 }, network_->GetLayerCount());
            network_ = network_->WithRowRemoved(newRowIndex);
        }
    });

    // Modify internal network connections
    AddMutation(BASE_WEIGHT, [&]()
    {
        network_ = network_->WithMutatedConnections();
    });

    // Modify output connections
    AddMutation(BASE_WEIGHT, [&]()
    {
        outputConnections_ = outputConnections_->WithMutatedConnections();
    });
}

Energy GeneSenseBase::GetMetabolicCost() const
{
    return ( 1 + network_->GetLayerCount()) * network_->GetLayerWidth() * 0.05_uj;
}

void GeneSenseBase::AddColumnInsertedAndRemovedMutations(double addedMutationWeight, std::function<void (unsigned)>&& onColumnAdded, double removedMutationWeight, std::function<void (unsigned)>&& onColumnRemoved)
{
    // Insert a new column
    AddMutation(addedMutationWeight, [&, onColumnAdded = std::move(onColumnAdded)]()
    {
        unsigned newColIndex = Random::Number(0u, network_->GetInputCount());
        network_ = network_->WithColumnAdded(newColIndex, NeuralNetwork::InitialWeights::Random);
        outputConnections_ = outputConnections_->WithInputAdded(newColIndex);
        onColumnAdded(newColIndex);
    });

    // Remove a column
    AddMutation(removedMutationWeight, [&, onColumnRemoved = std::move(onColumnRemoved)]()
    {
        // Don't allow mutation to remove final column
        if (network_->GetInputCount() > 1) {
            unsigned index = Random::Number(0u, network_->GetInputCount() - 1);
            network_ = network_->WithColumnRemoved(index);
            outputConnections_ = outputConnections_->WithInputRemoved(index);
            onColumnRemoved(index);
        }
    });
}

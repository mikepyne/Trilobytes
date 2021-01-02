#include "GeneEffectorBase.h"

GeneEffectorBase::GeneEffectorBase(unsigned hiddenLayers, unsigned inputCount, unsigned outputCount)
    : GeneEffectorBase(std::make_shared<NeuralNetwork>(hiddenLayers, outputCount, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(inputCount, outputCount), Random::Number(0.0, 100.0))
{
}

GeneEffectorBase::GeneEffectorBase(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance)
    : Gene(dominance)
    , network_(network)
    , inputConnections_(inputConnections)
{
    // Insert a new row
    AddMutation(0.15 * BASE_WEIGHT, [&]()
    {
        unsigned newRowIndex = Random::Number(0u, network_->GetLayerCount());
        network_ = network_->WithRowAdded(newRowIndex, NeuralNetwork::InitialWeights::PassThrough);
    });

    // Remove a row
    AddMutation(0.15 * BASE_WEIGHT, [&]()
    {
        // Don't allow mutation to remove final row
        if (network_->GetLayerCount() > 1) {
            unsigned newRowIndex = Random::Number(0u, network_->GetLayerCount());
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
        inputConnections_ = inputConnections_->WithMutatedConnections();
    });
}

Energy GeneEffectorBase::GetMetabolicCost() const
{
    return (1 + network_->GetLayerCount()) * network_->GetLayerWidth() * 0.05_uj;
}

void GeneEffectorBase::AddColumnInsertedMutation(double mutationWeight, std::function<void (unsigned)>&& onColumnAdded)
{
    // Insert a new column
    AddMutation(mutationWeight, [&, onColumnAdded = std::move(onColumnAdded)]()
    {
        unsigned newColIndex = Random::Number(0u, network_->GetInputCount());
        network_ = network_->WithColumnAdded(newColIndex, NeuralNetwork::InitialWeights::Random);
        inputConnections_ = inputConnections_->WithOutputAdded(newColIndex);
        onColumnAdded(newColIndex);
    });
}

void GeneEffectorBase::AddColumnRemovedMutation(double mutationWeight, std::function<void(unsigned index)>&& onColumnRemoved)
{
    // Remove a column
    AddMutation(mutationWeight, [&, onColumnRemoved = std::move(onColumnRemoved)]()
    {
        // Don't allow mutation to remove final column
        if (network_->GetInputCount() > 1) {
            unsigned index = Random::Number(0u, network_->GetInputCount() - 1);
            network_ = network_->WithColumnRemoved(index);
            inputConnections_ = inputConnections_->WithOutputRemoved(index);
            onColumnRemoved(index);
        }
    });
}

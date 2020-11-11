#include "GeneBrain.h"

#include "Energy.h"
#include "Phenotype.h"

GeneBrain::GeneBrain(unsigned layerCount, unsigned width, double mutationProbability)
    : GeneBrain(std::make_shared<NeuralNetwork>(layerCount, width, NeuralNetwork::InitialWeights::Random), Random::Number(0.0, 100.0), mutationProbability)
{
}

GeneBrain::GeneBrain(const std::shared_ptr<NeuralNetwork>& brain, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , brain_(brain)
{
    // Modify connections
    AddMutation(BASE_WEIGHT, [&]()
    {
        brain_ = brain_->WithMutatedConnections();
    });

    // Insert a new row
    AddMutation(0.01 * BASE_WEIGHT, [&]()
    {
        unsigned newRowIndex = Random::Number(0u, brain_->GetLayerCount());
        brain_ = brain_->WithRowAdded(newRowIndex, NeuralNetwork::InitialWeights::PassThrough);
    });

    // Remove a row
    AddMutation(0.01 * BASE_WEIGHT, [&]()
    {
        // Don't allow mutation to remove final row
        if (brain_->GetLayerCount() > 1) {
            unsigned newRowIndex = Random::Number(0u, brain_->GetLayerCount());
            brain_ = brain_->WithRowRemoved(newRowIndex);
        }
    });

    // MAYBE add mutations for adding/removing columns (would require modifications to all senses and effectors connections, and knowledge of which index was effected by the mutation)
}

void GeneBrain::ExpressGene(Swimmer& /*owner*/, Phenotype& target) const
{
    target.brain = brain_;
    target.baseMetabolism += (1 + brain_->GetLayerCount()) * brain_->GetLayerWidth() * 0.15_uj;
}

std::shared_ptr<Gene> GeneBrain::Copy() const
{
    return std::make_shared<GeneBrain>(brain_, GetDominance(), GetMutationProbability());
}

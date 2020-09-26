#include "GeneBrain.h"

#include "Phenotype.h"

GeneBrain::GeneBrain(unsigned layerCount, unsigned width, double mutationProbability)
    : GeneBrain(std::make_shared<NeuralNetwork>(layerCount, width, NeuralNetwork::InitialWeights::Random), Random::Number(0.0, 100.0), mutationProbability)
{
}

GeneBrain::GeneBrain(const std::shared_ptr<NeuralNetwork>& brain, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , brain_(brain)
{
}

std::shared_ptr<Gene> GeneBrain::Mutate() const
{
    return std::make_shared<GeneBrain>(brain_->Mutated(), GetDominance(), GetMutationProbability() + Random::Number(-0.01, 0.01));
}

void GeneBrain::ExpressGene(const Swimmer& /*owner*/, Phenotype& target) const
{
    target.brain = brain_;
}

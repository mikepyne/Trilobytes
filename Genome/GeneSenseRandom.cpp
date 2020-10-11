#include "GeneSenseRandom.h"

#include "Phenotype.h"
#include "Sensors/SenseRandom.h"

GeneSenseRandom::GeneSenseRandom(unsigned inputCount, unsigned outputCount)
    : GeneSenseRandom(std::make_shared<NeuralNetwork>(0, inputCount, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(NeuralNetworkConnector(inputCount, outputCount)), CreateRandomFilteredRandoms(inputCount), Random::Number(0.0, 100.0), Random::Number(0.0, 0.01))
{
}

GeneSenseRandom::GeneSenseRandom(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, std::vector<SenseRandom::FilteredRandom>&& filteredRandoms, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , network_(network)
    , outputConnections_(outputConnections)
    , filteredRandoms_(std::move(filteredRandoms))
{
}

std::shared_ptr<Gene> GeneSenseRandom::Mutate() const
{
    std::shared_ptr<NeuralNetwork> network = network_;
    std::shared_ptr<NeuralNetworkConnector> outputConnections = outputConnections_;
    std::vector<SenseRandom::FilteredRandom> randoms = filteredRandoms_;
    double dominance = GetDominance();
    double mutationProbability = GetMutationProbability();

    switch(Random::Number(0, 4)) {
    case 0 :
        network = network->Mutated();
        break;
    case 1 :
        outputConnections = outputConnections->Mutated();
        break;
    case 2 :
        switch (Random::Number(0, 4)) {
        case 0 :
            // Modify one
            if (!randoms.empty()) {
                auto& random = Random::Item(randoms);
                random = { Random::GaussianAdjustment(random.min_, 0.1), Random::GaussianAdjustment(random.max_, 0.1), std::clamp(Random::GaussianAdjustment(random.alpha_, 0.1), 0.0, 1.0) };
            }
            break;
        case 1 :
            // TODO Add one
            break;
        case 2 :
            // TODO Remove one
            break;
        case 3 :
            // Shuffle
            if (randoms.size() > 1) {
                Random::Shuffle(randoms);
            }
            break;
        case 4 :
            // Swap
            if (randoms.size() > 1) {
                std::swap(Random::Item(randoms), Random::Item(randoms));
            }
            break;
        }
        break;
    case 3 :
        dominance = GetMutatedDominance();
        break;
    case 4 :
        mutationProbability = GetMutatedMutationProbability();
        break;
    }

    return std::make_shared<GeneSenseRandom>(network, outputConnections, std::move(randoms), dominance, mutationProbability);
}

void GeneSenseRandom::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseRandom>(network_, outputConnections_, owner, std::vector(filteredRandoms_)));
}

std::vector<SenseRandom::FilteredRandom> GeneSenseRandom::CreateRandomFilteredRandoms(unsigned count)
{
    std::vector<SenseRandom::FilteredRandom> randoms;
    for (unsigned i = 0; i < count; ++i) {
        randoms.push_back({ -1.0, 1.0, std::clamp(Random::Gaussian(0.5, 0.1), 0.0, 1.0) });
    }
    return randoms;
}

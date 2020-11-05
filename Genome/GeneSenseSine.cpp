#include "GeneSenseSine.h"

#include "Swimmer.h"
#include "Phenotype.h"

GeneSenseSine::GeneSenseSine(unsigned inputCount, unsigned outputCount)
    : GeneSenseSine(std::make_shared<NeuralNetwork>(0, inputCount, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(NeuralNetworkConnector(inputCount, outputCount)), CreateRandomWaves(inputCount), Random::Number(0.0, 100.0), Random::Number(0.0, 0.01))
{
}

GeneSenseSine::GeneSenseSine(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, std::vector<SenseSine::SineWave>&& sineWaves, double dominance, double mutationProbability)
    : GeneSenseBase(network, outputConnections, dominance, mutationProbability)
    , sineWaves_(std::move(sineWaves))
{
    // Modify one
    AddMutation(BASE_WEIGHT, [&]()
    {
        if (!sineWaves_.empty()) {
            auto& wave = Random::Item(sineWaves_);
            wave = { Random::GaussianAdjustment(wave.amplitude_, 0.1), Random::GaussianAdjustment(wave.frequency_, 0.1) };
        }
    });

    // Swap
    AddMutation(0.25 * BASE_WEIGHT, [&]()
    {
        if (sineWaves_.size() > 1) {
            std::swap(Random::Item(sineWaves_), Random::Item(sineWaves_));
        }
    });

    // Shuffle
    AddMutation(0.05 * BASE_WEIGHT, [&]()
    {
        if (sineWaves_.size() > 1) {
            Random::Shuffle(sineWaves_);
        }
    });

    // Add wave
    AddColumnInsertedMutation(0.15 * BASE_WEIGHT, [&](unsigned index)
    {
        auto iter = sineWaves_.begin();
        std::advance(iter, index);
        sineWaves_.insert(iter, { Random::Gaussian(0.75, 0.1), Random::Gaussian(0.01, 0.1) });
    });

    // Remove wave
    AddColumnInsertedMutation(0.15 * BASE_WEIGHT, [&](unsigned index)
    {
        auto iter = sineWaves_.begin();
        std::advance(iter, index);
        sineWaves_.erase(iter);
    });
}

void GeneSenseSine::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseSine>(GetNetwork(), GetOutputConnections(), owner, std::vector(sineWaves_)));
}

std::shared_ptr<Gene> GeneSenseSine::Copy() const
{
    return std::make_shared<GeneSenseSine>(GetNetwork(), GetOutputConnections(), std::vector(sineWaves_), GetDominance(), GetMutationProbability());
}

std::vector<SenseSine::SineWave> GeneSenseSine::CreateRandomWaves(unsigned count)
{
    std::vector<SenseSine::SineWave> waves;
    for (unsigned i = 0; i < count ; ++i) {
        waves.push_back({ Random::Gaussian(0.75, 0.1), Random::Gaussian(0.01, 0.1) });
    }
    return waves;
}

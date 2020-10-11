#include "GeneSenseSine.h"

#include "Swimmer.h"
#include "Phenotype.h"

GeneSenseSine::GeneSenseSine(unsigned inputCount, unsigned outputCount)
    : GeneSenseSine(std::make_shared<NeuralNetwork>(0, inputCount, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(NeuralNetworkConnector(inputCount, outputCount)), CreateRandomWaves(inputCount), Random::Number(0.0, 100.0), Random::Number(0.0, 0.01))
{
}

GeneSenseSine::GeneSenseSine(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, std::vector<SenseSine::SineWave>&& sineWaves, double dominance, double mutationProbability)
    : Gene(dominance, mutationProbability)
    , network_(network)
    , outputConnections_(outputConnections)
    , sineWaves_(std::move(sineWaves))
{
}

std::shared_ptr<Gene> GeneSenseSine::Mutate() const
{
    auto network = network_;
    auto outputs = outputConnections_;
    auto waves = sineWaves_;
    double dominance = GetDominance();
    double mutationProbabitlity = GetMutatedMutationProbability();

    switch (Random::Number(0, 4)) {
    case 0 :
        network = network->Mutated();
        break;
    case 1 :
        outputs = outputs->Mutated();
        break;
    case 2 :
        switch (Random::Number(0, 4)) {
        case 0 :
            // Modify one
            if (!waves.empty()) {
                auto& wave = Random::Item(waves);
                wave = { Random::GaussianAdjustment(wave.amplitude_, 0.1), Random::GaussianAdjustment(wave.frequency_, 0.1) };
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
            if (waves.size() > 1) {
                Random::Shuffle(waves);
            }
            break;
        case 4 :
            // Swap
            if (waves.size() > 1) {
                std::swap(Random::Item(waves), Random::Item(waves));
            }
            break;
        }
        break;
    case 3 :
        dominance = GetMutatedDominance();
        break;
    case 4 :
        mutationProbabitlity = GetMutatedMutationProbability();
        break;
    }
    return std::make_shared<GeneSenseSine>(network, outputs, std::move(waves), dominance, mutationProbabitlity);
}

void GeneSenseSine::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseSine>(network_, outputConnections_, owner, std::vector(sineWaves_)));
}

std::vector<SenseSine::SineWave> GeneSenseSine::CreateRandomWaves(unsigned count)
{
    std::vector<SenseSine::SineWave> waves;
    for (unsigned i = 0; i < count ; ++i) {
        waves.push_back({ Random::Gaussian(0.75, 0.1), Random::Gaussian(0.01, 0.1) });
    }
    return waves;
}

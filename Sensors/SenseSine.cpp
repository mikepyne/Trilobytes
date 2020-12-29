#include "SenseSine.h"

SenseSine::SenseSine(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, std::vector<SenseSine::SineWave>&& sineWaves)
    : Sense(network, outputConnections, owner)
    , sineWaves_(sineWaves)
{
}

void SenseSine::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& /*entities*/, const UniverseParameters& /*universeParameters*/) const
{
    EoBE::IterateBoth<double, SineWave>(inputs, sineWaves_, [](double& input, const SineWave& sine)
    {
        input = std::clamp(sine.amplitude_ * std::sin(sine.x_), -1.0, 1.0);
    });
}

void SenseSine::PrepareToPrime()
{
    for (auto& sine : sineWaves_) {
        sine.x_ += sine.frequency_;
    }
}

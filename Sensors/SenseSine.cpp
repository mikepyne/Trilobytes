#include "SenseSine.h"

SenseSine::SenseSine(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, std::vector<SenseSine::SineWave>&& sineWaves)
    : Sense(network, outputConnections, owner)
    , sineWaves_(sineWaves)
{
}

std::string SenseSine::GetDescription() const
{
    std::stringstream desc;

    desc << "<p>This sense acts like an internal rythm, instead of detecting "
            "something external. Each input is its own sine wave, and the value"
            " is recalculated each simulation tick.</p>"
            "<p>A wave's amplitude is not limited, however the inputs are "
            "clamped to between -1 and 1, leading to squared off peaks.</p>";

    desc << "<p>Waves:<ol>";

    for (const SineWave& wave : sineWaves_) {
        desc << "<li> Amplitude " << wave.amplitude_ << ", " << "Frequency " << wave.frequency_ << "</li>";
    }

    desc << "</p></ol>";

    return desc.str();
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

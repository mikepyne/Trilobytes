#include "NeuralNetworkConnector.h"

NeuralNetworkConnector::NeuralNetworkConnector(unsigned inputs, unsigned outputs)
    : weights_(inputs, std::vector<double>(outputs, 0.0))
{
    for (auto& input : weights_) {
        input.at(Random::Number(0u, input.size() - 1)) = 1.0;
    }
}

void NeuralNetworkConnector::PassForward(const std::vector<double>& inputValues, std::vector<double>& outputValues)
{
    assert(inputValues.size() == weights_.size() && outputValues.size() == weights_.at(0).size());
    EoBE::IterateBoth<double, std::vector<double>>(inputValues, weights_, [&outputValues](const double& input, const std::vector<double>& inputWeights) -> void
    {
        EoBE::IterateBoth<double, double>(inputWeights, outputValues, [&input](const double& inputWeight, double& output) -> void
        {
            output += input * inputWeight;
        });
    });
}

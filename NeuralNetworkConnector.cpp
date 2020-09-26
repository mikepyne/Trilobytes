#include "NeuralNetworkConnector.h"

#include "Utils.h"
#include "Random.h"

#include <assert.h>

NeuralNetworkConnector::NeuralNetworkConnector(unsigned inputs, unsigned outputs)
    : weights_(inputs, std::vector<double>(outputs, 0.0))
{
    assert(!weights_.empty());
    std::vector<size_t> inputIndexes = EoBE::CreateSeries<size_t>(0, inputs);
    std::vector<size_t> outputIndexes = EoBE::CreateSeries<size_t>(0, outputs);

    // so we don't just connect input 1 to output 1, and input 2 to output 2 etc
    Random::Shuffle(inputIndexes);
    Random::Shuffle(outputIndexes);

    /*
     * Creates a bunch of 1:1 connections between inputs and outputs. The larger
     * of inputs or outputs will therefore have some unconnected nodes.
     */
    EoBE::IterateBoth<size_t, size_t>(inputIndexes, outputIndexes, [&](const size_t& in, const size_t& out)
    {
        // set the weight of an input to an output to 1 so it is a "direct passthrough" connection
        weights_.at(in).at(out) = 1.0;
    });
}

NeuralNetworkConnector::NeuralNetworkConnector(std::vector<std::vector<double>>&& weights)
    : weights_(std::move(weights))
{
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

std::shared_ptr<NeuralNetworkConnector> NeuralNetworkConnector::Mutated() const
{
    std::vector<std::vector<double>> newWeights = weights_;

    double percent = Random::Percent();
    if (percent > 10) {
        // 90% Chance mutate a single connection
        if (newWeights.size() > 0) {
            unsigned input = Random::Number<size_t>(0, newWeights.size() - 1);
            if (newWeights.at(input).size() > 0) {
                unsigned output = Random::Number<size_t>(0, newWeights.at(input).size() - 1);
                newWeights.at(input).at(output) += Random::Gaussian(0.0, 0.2);
            }
        }
    } else if (percent > 2) {
        // 8% Chance mutate all connections
        for (auto& input : newWeights) {
            for (auto& output : input) {
                output += Random::Gaussian(0.0, 0.1);
            }
        }
    } else {
        // 2% Chance shuffle connections
        for (auto& input : newWeights) {
            Random::Shuffle(input);
        }
    }

    return std::make_shared<NeuralNetworkConnector>(std::move(newWeights));
}

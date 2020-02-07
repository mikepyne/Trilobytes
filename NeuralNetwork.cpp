#include "NeuralNetwork.h"


NeuralNetwork::NeuralNetwork(unsigned layerCount, unsigned width)
    : layers_(layerCount, Layer(width))
{
    assert(layerCount > 0);
    assert(width > 0);

    // First layer doesn't need input weights, as it will be assigned a value by the ForwardPropogate() func
    for (auto& layer : layers_) {
        for (auto& node : layer) {
            node = Random::GaussianArray(width, -0.5, 0.5);
        }
    }
}

const std::vector<double>& NeuralNetwork::ForwardPropogate(const std::vector<double>& inputs)
{
    static std::vector<double> previousNodeValues;

    const size_t width = layers_.front().size();
    assert(inputs.size() == width);

    // about to swap with previousNodeValues so we can return outputs at the end
    outputs_ = inputs;
    for (auto& layer : layers_) {
        std::swap(outputs_, previousNodeValues);
        // We'll reuse this vector for the output of each layer
        outputs_.assign(width, 0.0);

        size_t nodeIndex = 0;
        for (auto& node : layer) {
            double nodeValue = 0.0;
            size_t edgeIndex = 0;
            for (auto& edge : node) {
                nodeValue += edge * previousNodeValues[edgeIndex];
                ++edgeIndex;
            }
            // tanh is our sigma function
            outputs_[nodeIndex] = std::tanh(nodeValue);
            nodeIndex++;
        }
    }
    return outputs_;
}

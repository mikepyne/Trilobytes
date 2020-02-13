#include "NeuralNetwork.h"


NeuralNetwork::NeuralNetwork(unsigned layerCount, unsigned width)
    : NeuralNetwork(CreateRandomLayers(layerCount, width))
{
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

NeuralNetwork NeuralNetwork::Mutated()
{
    size_t layerCount = layers_.size();
    size_t width = layers_.front().size();
    std::vector<Layer> layers;
    for (auto& layer : layers_) {
        layers.push_back(Layer());
        for (auto& node : layer) {
            layers.back().push_back(Node());
            for (auto& edge : node) {
                double newEdge = edge;
                // i.e average 3 mutations per child
                if (Random::Int(0u, layerCount * width * width) < 3) {
                    edge += Random::Gaussian(-0.5, 0.5);
                }
                layers.back().back().push_back(newEdge);
            }
        }
    }
    return NeuralNetwork(std::move(layers));
}

NeuralNetwork::NeuralNetwork(std::vector<NeuralNetwork::Layer>&& layers)
    : layers_(std::move(layers))
{
}

std::vector<NeuralNetwork::Layer> NeuralNetwork::CreateRandomLayers(unsigned layerCount, unsigned width)
{
    assert(layerCount > 0);
    assert(width > 0);

    std::vector<Layer> layers(layerCount, Layer(width, Node(width)));

    // First layer doesn't need input weights, as it will be assigned a value by the ForwardPropogate() func
    for (auto& layer : layers) {
        for (auto& node : layer) {
            node = Random::GaussianArray(width, 0.0, 0.75);
        }
    }

    return layers;
}
